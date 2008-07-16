

#pragma once

#include <QStandardItem>
#include <QStandardItemModel>

struct sqlite3;

class Player : public QStandardItem
{
    public:
        static void loadPlayers(sqlite3* db, QStandardItemModel* model);

//        Player( const std::string& key
//              , const std::string& name
//              , const std::string & team
//              );

    public:
};
