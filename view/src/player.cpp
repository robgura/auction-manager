#include "view/player.h"
#include <fstream>
#include <sstream>
#include <QDebug>

void niceInsertColumn(QStandardItem* parent, int column, QStandardItem* item)
{
    QList<QStandardItem*> items;
    items << item;
    parent->insertColumn(column, items);
}

void Player::loadPlayers(sqlite3* db, QStandardItemModel* model)
{
    std::ifstream inFile("./getplayers/players.dat");
    std::string lineBuf;
    while(std::getline(inFile, lineBuf))
    {
        std::istringstream ist(lineBuf);

        std::string key;
        std::getline(ist, key, '#');

        std::string name;
        std::getline(ist, name, '#');

        std::string pos;
        std::getline(ist, pos, '#');

        std::string team;
        std::getline(ist, team, '#');

        Qt::ItemFlags flag(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);

        QStandardItem* nameItem = new QStandardItem(name.c_str());
        nameItem->setFlags(flag);

        QStandardItem* posItem = new QStandardItem(pos.c_str());
        posItem->setFlags(flag);

        QStandardItem* teamItem = new QStandardItem(team.c_str());
        teamItem->setFlags(flag);

        QStandardItem* keyItem = new QStandardItem(key.c_str());

        QList<QStandardItem*> items;
        items << nameItem << posItem << teamItem << keyItem;

        model->appendRow(items);
    }
}
