#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "sqlite/sqlite3.h"
#include "view/ownermodel.h"
#include "view/confirmdialog.h"
#include <QDebug>
#include <assert.h>
#include <QMimeData>
#include <QStringList>

class OwnerModel::LoadOwnerHandler
{
    public:
        static OwnerModel::Owners* _owners;

        static int handle(void*, int columns, char** columnData, char** columnNames)
        {
            _owners->push_back(Owner( atoi(columnData[0])
                                    , columnData[1]
                                    , columnData[2]
                                    ));
            return 0;
        }
};

OwnerModel::OwnerModel::Owners* OwnerModel::LoadOwnerHandler::_owners = 0;

OwnerModel::OwnerModel(sqlite3* db)
    : _db(db)
{
    LoadOwnerHandler lpHandler;
    lpHandler._owners = &_ownerCache;

    std::string sql = "select * from Owners;";
    if(sqlite3_exec(db, sql.c_str(), LoadOwnerHandler::handle, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
    }
}

Qt::ItemFlags OwnerModel::flags(const QModelIndex & index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
}

QModelIndex OwnerModel::index(int row, int column, const QModelIndex& parent) const
{
    if(! parent.isValid())
    {
        return createIndex(row, column, row);
    }
    return QModelIndex();
}

QModelIndex OwnerModel::parent(const QModelIndex& index) const
{
    return QModelIndex();
}

int OwnerModel::rowCount(const QModelIndex& index) const
{
    if(! index.isValid())
    {
        return _ownerCache.size();
    }
    return 0;
}

int OwnerModel::columnCount(const QModelIndex& index) const
{
    if(! index.isValid())
    {
        return 2;
    }
    return 0;
}

QVariant OwnerModel::data(const QModelIndex& index, int role) const
{
    if(index.isValid())
    {
        if(role == Qt::DisplayRole)
        {
            if(index.column() == 0)
            {
                return QVariant(QString(_ownerCache.at(index.row())._name.c_str()));
            }
            else if(index.column() == 1)
            {
                return QVariant(QString(_ownerCache.at(index.row())._teamName.c_str()));
            }
        }
        else if(role == KeyRole)
        {
            return QVariant(_ownerCache.at(index.row())._key);
        }
    }

    return QVariant();
}

bool OwnerModel::dropMimeData(const QMimeData* mimeData, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{
    // only works if user drops directly on parent
    if(row != -1)
    {
        return false;
    }

    const int playerKey = atoi(mimeData->data("application/x-nflplayer").constData());

    const int ownerKey = data(parent, KeyRole).toInt();

    cout << "Dropping Player Key " << playerKey << " on Owner Key " << ownerKey << endl;

    ConfirmPurchase p(0, _db, ownerKey, playerKey);
    if(p.exec() == 1)
    {
        cout << "Paid " << p.getPrice() << endl;
        std::string str_ownerKey = QString::number(ownerKey).toStdString();
        std::string str_playerKey = QString::number(playerKey).toStdString();
        std::string str_price = QString::number(p.getPrice()).toStdString();
        const std::string insert = "insert into OwnerPlayers values ( " + str_ownerKey + ", " + str_playerKey + ", " + str_price + " );";
        cout << insert << endl;
        if(sqlite3_exec(_db, insert.c_str(), 0, 0, 0) != SQLITE_OK)
        {
            qDebug() << sqlite3_errmsg(_db);
        }
    }


    return true;
}

QStringList OwnerModel::mimeTypes() const
{
    QStringList types;
    types << "application/x-nflplayer";
    return types;
}

class GetMax
{
    public:
        static int max;

        static int handle(void*, int columns, char** columnData, char** columnNames)
        {
            max = 0;
            if(columnData[0])
            {
                max = atoi(columnData[0]);
            }
            return 0;
        }
};

int GetMax::max(0);

void OwnerModel::addOwner(const std::string& owner, const std::string& teamName)
{
    beginInsertRows(QModelIndex(), _ownerCache.size(), _ownerCache.size());

    if(sqlite3_exec(_db, "select max(Key) from Owners;", GetMax::handle, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(_db);
    }

    int newKey = GetMax::max + 1;
    QString stringKey = QString::number(newKey);

    std::string insertSql = "insert into Owners values (" + stringKey.toStdString() +  ",\"" + owner + "\"" + "," + "\"" + teamName + "\"" + ");";

    if(sqlite3_exec(_db, insertSql.c_str(), 0, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(_db);
    }

    _ownerCache.push_back(Owner(newKey, owner, teamName));

    endInsertRows();

}
