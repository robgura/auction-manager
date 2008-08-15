
#pragma once

#include <QWidget>
#include "view/ui_ownerInfo.h"

struct sqlite3;

class OwnerInfo : public QWidget
{
    Q_OBJECT

    public:
        enum Role
        {
            PlayerKey = Qt::UserRole,
            TransactionKey
        };

    public:
        OwnerInfo(QWidget* parent, sqlite3* db, int ownerKey);

    private slots:
        void deleteButton(bool);

    private:
        void setupTransactions(int ownerKey);
        void setupMoney(int ownerKey);
        void setupOwnerName(int ownerKey);

    private:
        Ui::Form _form;

        sqlite3* _db;
};
