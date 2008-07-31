
#pragma once

#include "view/ui_playertrade.h"
#include <QDialog>
#include <QRadioButton>

struct sqlite3;

class TradePlayerDialog : public QDialog
{
    Q_OBJECT

    public:
        TradePlayerDialog(QWidget* parent, sqlite3* db);

    public:
        virtual void accept();

    private:
        typedef std::map<int, QRadioButton*> RBMap;

        void loadTeams();

        void loadOwnersPlayers();

        static int findSelected(const RBMap& map);

        void connectButtons();

    private slots:

        void clicked(bool);

    private:
        Ui::TradeDialog _form;

        sqlite3* _db;

        RBMap _sellers;
        RBMap _buyers;
};
