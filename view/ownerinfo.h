
#pragma once

#include "view/transtypes.h"
#include "view/ui_ownerInfo.h"
#include <QMetaType>
#include <QWidget>

struct sqlite3;

struct TransactionData
{
    TransTypes type;
    int playerKey;
    int rowId;
    std::string playerName;
    std::string pos;
    std::string price;
};

typedef std::vector<TransactionData> Transactions;
Transactions getTransactionData(sqlite3* db, int ownerKey);

Q_DECLARE_METATYPE(TransactionData)

class OwnerInfo : public QWidget
{
    Q_OBJECT

    public:
        enum Role
        {
            Data = Qt::UserRole,
        };

    public:
        OwnerInfo(QWidget* parent, sqlite3* db, int ownerKey);

    private slots:
        void deleteButton(bool);

    private:
        void setupTransactions();
        void setupMoney();
        void setupOwnerName();

    private:
        Ui::Form _form;

        sqlite3* _db;

        const int _ownerKey;
};
