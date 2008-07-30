

#pragma once

#include <string>

struct sqlite3;

class Transaction
{
    public:
        static void buy(sqlite3* db, const int ownerKey, const int playerKey, const int price);

        static int moneySpent(sqlite3* db, const int ownerKey);

        static int moneyLeft(sqlite3* db, const int ownerKey);

        static int rosterSize(sqlite3* db, const int ownerKey);

        static int availableMoneyLeft(sqlite3* db, const int ownerKey);

};
