#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "sqlite/sqlite3.h"
#include "view/transaction.h"
#include "view/transtypes.h"
#include "view/parsesql.h"
#include "view/settings.h"
#include "view/shouter.h"
#include <QDebug>
#include <QString>


void Transaction::sell(sqlite3* db, const int ownerKey, const int playerKey, const int price)
{
    std::string str_ownerKey = QString::number(ownerKey).toStdString();
    std::string str_playerKey = QString::number(playerKey).toStdString();
    std::string str_price = QString::number(price).toStdString();
    std::string str_trans = QString::number(Sell).toStdString();

    const std::string insert = "insert into OwnerPlayers values ( 0, " + str_ownerKey + ", " + str_playerKey + ", " + str_trans + ", " + str_price + " );";
    if(sqlite3_exec(db, insert.c_str(), 0, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
    }

    Shouter::instance()->trans();
}

void Transaction::buy(sqlite3* db, const int ownerKey, const int playerKey, const int price)
{
    std::string str_ownerKey = QString::number(ownerKey).toStdString();
    std::string str_playerKey = QString::number(playerKey).toStdString();
    std::string str_price = QString::number(price).toStdString();
    std::string str_trans = QString::number(Buy).toStdString();

cout << "aAAA\n";
    const std::string insert = "insert into OwnerPlayers values ( 0, " + str_ownerKey + ", " + str_playerKey + ", " + str_trans + ", " + str_price + " );";
    if(sqlite3_exec(db, insert.c_str(), 0, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
    }
cout << "bAAA\n";

    Shouter::instance()->trans();
}

int Transaction::moneySpent(sqlite3* db, const int ownerKey)
{
    int spent = 0;
    std::string ownerKey_str = QString::number(ownerKey).toStdString();
    Rows rows = ParseSQL::exec(db, "select TransType, Price FROM OwnerPlayers WHERE OwnerKey=" + ownerKey_str + ";");
    for(Rows::const_iterator iter = rows.begin(); iter != rows.end(); ++iter)
    {
        if(iter->at(0) == "0")
        {
            spent += atoi(iter->at(1).c_str());
        }
        else if(iter->at(0) == "1")
        {
            spent -= atoi(iter->at(1).c_str());
        }
    }

    return spent;
}

int Transaction::moneyLeft(sqlite3* db, const int ownerKey)
{
    return Settings::getStartingMoney() - moneySpent(db, ownerKey);
}

int Transaction::rosterSize(sqlite3* db, const int ownerKey)
{
    int size = 0;
    std::string ownerKey_str = QString::number(ownerKey).toStdString();
    Rows rows = ParseSQL::exec(db, "select TransType FROM OwnerPlayers WHERE OwnerKey=" + ownerKey_str + ";");
    for(Rows::const_iterator iter = rows.begin(); iter != rows.end(); ++iter)
    {
        if(iter->at(0) == "0")
        {
            size++;
        }
        else if(iter->at(0) == "1")
        {
            size--;
        }
    }

    return size;
}

int Transaction::availableMoneyLeft(sqlite3* db, const int ownerKey)
{
    return moneyLeft(db, ownerKey) - Settings::getRosterPositions() + rosterSize(db, ownerKey) + 1;
}
