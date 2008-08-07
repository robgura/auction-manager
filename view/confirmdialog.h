
#pragma once

#include "view/ui_confirm_purchase.h"
#include "view/ownermodel.h"
#include <QDialog>

struct sqlite3;

class ConfirmPurchase : public QDialog
{
    Q_OBJECT

    public:
        ConfirmPurchase(QWidget* parent, sqlite3* db, int ownerKey, int playerKey);
        int getPrice() const;

    public:
        virtual void accept();

    private:
        void load();
        void verifyPlayerNotOwned();

    private slots:
        void verifyPrice(int);

    private:
        void loadNflData();
        void loadOwnerData();

    private:
        Ui::ConfirmPurchase _form;

        sqlite3* _db;

        const int _ownerKey;

        const int _playerKey;

        int _price;

        int _maxBid;
};

inline int ConfirmPurchase::getPrice() const
{
    return _price;
}

