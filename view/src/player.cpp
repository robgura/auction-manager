#include "view/player.h"
#include <fstream>
#include <sstream>
#include <QDebug>

// new item comment void niceInsertColumn(QStandardItem* parent, int column, QStandardItem* item)
// new item comment {
// new item comment     QList<QStandardItem*> items;
// new item comment     items << item;
// new item comment     parent->insertColumn(column, items);
// new item comment }
// new item comment 
// new item comment void Player::loadPlayers(sqlite3* db, QStandardItemModel* model)
// new item comment {
// new item comment     std::ifstream inFile("./getplayers/players.dat");
// new item comment     std::string lineBuf;
// new item comment     while(std::getline(inFile, lineBuf))
// new item comment     {
// new item comment         std::istringstream ist(lineBuf);
// new item comment 
// new item comment         std::string key;
// new item comment         std::getline(ist, key, '#');
// new item comment 
// new item comment         std::string name;
// new item comment         std::getline(ist, name, '#');
// new item comment 
// new item comment         std::string pos;
// new item comment         std::getline(ist, pos, '#');
// new item comment 
// new item comment         std::string team;
// new item comment         std::getline(ist, team, '#');
// new item comment 
// new item comment         Qt::ItemFlags flag(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
// new item comment 
// new item comment         QStandardItem* item = new QStandardItem(key.c_str());
// new item comment 
// new item comment 
// new item comment 
// new item comment //        QStandardItem* nameItem = new QStandardItem(name.c_str());
// new item comment //        nameItem->setFlags(flag);
// new item comment //
// new item comment //        QStandardItem* posItem = new QStandardItem(pos.c_str());
// new item comment //        posItem->setFlags(flag);
// new item comment //
// new item comment //        QStandardItem* teamItem = new QStandardItem(team.c_str());
// new item comment //        teamItem->setFlags(flag);
// new item comment //
// new item comment //        QStandardItem* keyItem = new QStandardItem(key.c_str());
// new item comment 
// new item comment         QList<QStandardItem*> items;
// new item comment         items << nameItem << posItem << teamItem << keyItem;
// new item comment 
// new item comment         model->appendRow(items);
// new item comment     }
// new item comment }
