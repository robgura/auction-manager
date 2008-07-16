
#pragma once

#include "view/player.h"
#include <vector>
#include <QAbstractItemModel>

struct sqlite3;

class PlayerModel : public QAbstractItemModel
{
    public:
        PlayerModel(sqlite3* db);

        virtual Qt::ItemFlags flags(const QModelIndex & index) const;
        virtual QModelIndex index(int, int, const QModelIndex&) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex&) const;
        virtual int columnCount(const QModelIndex&) const;
        virtual QVariant data(const QModelIndex&, int) const;

        typedef std::vector<Player> Players;
    private:
        Players _playerCache;
};

