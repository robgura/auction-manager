
#pragma once

#include <QWidget>
#include "view/ui_ownerInfo.h"

struct sqlite3;

class OwnerInfo : public QWidget
{
    public:
        OwnerInfo(QWidget* parent, sqlite3* db, int ownerKey);

    private:
        void setupTransactions(int ownerKey);

    private:
        Ui::Form _form;

        sqlite3* _db;
};
