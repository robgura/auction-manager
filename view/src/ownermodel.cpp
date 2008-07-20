#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "sqlite/sqlite3.h"
#include "view/ownermodel.h"
#include <QDebug>
#include <assert.h>

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
    return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
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
    }

    return QVariant();
}

class GetMax
{
    public:
        static int max;

        static int handle(void*, int columns, char** columnData, char** columnNames)
        {
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
