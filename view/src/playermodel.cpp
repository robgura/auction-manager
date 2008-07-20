

#include "sqlite/sqlite3.h"
#include "view/playermodel.h"
#include <QDebug>
#include <QMimeData>

class LoadPlayerHandler
{
    public:
        static PlayerModel::Players* _players;

        static int handle(void*, int columns, char** columnData, char** columnNames)
        {
            _players->push_back(Player( atoi(columnData[0])
                                      , columnData[1]
                                      , columnData[2]
                                      , columnData[3]
                                      ));
            return 0;
        }
};

PlayerModel::Players* LoadPlayerHandler::_players = 0;

PlayerModel::PlayerModel(sqlite3* db)
{
    LoadPlayerHandler lpHandler;
    lpHandler._players = &_playerCache;

    std::string sql = "select * from NFLPlayers;";
    int rc1 = sqlite3_exec(db, sql.c_str(), LoadPlayerHandler::handle, 0, 0);
    if(rc1 != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
    }
}

Qt::ItemFlags PlayerModel::flags(const QModelIndex & index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
}

QModelIndex PlayerModel::index(int row, int column, const QModelIndex& parent) const
{
    if(! parent.isValid())
    {
        return createIndex(row, column, row);
    }
    return QModelIndex();
}

QModelIndex PlayerModel::parent(const QModelIndex&) const
{
    return QModelIndex();
}

int PlayerModel::rowCount(const QModelIndex& index) const
{
    if(! index.isValid())
    {
        return _playerCache.size();
    }
    return 0;
}

int PlayerModel::columnCount(const QModelIndex& index) const
{
    if(! index.isValid())
    {
        return 3;
    }
    return 0;
}

QVariant PlayerModel::data(const QModelIndex& index, int role) const
{
    if(index.isValid())
    {
        if(role == Qt::DisplayRole)
        {
            if(index.column() == 0)
            {
                return QVariant(QString(_playerCache.at(index.row())._name.c_str()));
            }
            else if(index.column() == 1)
            {
                return QVariant(QString(_playerCache.at(index.row())._pos.c_str()));
            }
            else if(index.column() == 2)
            {
                return QVariant(QString(_playerCache.at(index.row())._team.c_str()));
            }
        }
        else if(role == KeyRole)
        {
            return QVariant(_playerCache.at(index.row())._key);
        }
    }
    return QVariant();
}

QMimeData* PlayerModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* mime = 0;
    if(! indexes.empty())
    {
        QVariant key = data(indexes.at(0), KeyRole);
        mime = new QMimeData();
        mime->setData("application/x-nflplayer", QString::number(key.toInt()).toAscii());
    }
    return mime;
}
