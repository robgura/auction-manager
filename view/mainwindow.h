

#pragma once

#include "view/ownerinfo.h"
#include <QMainWindow>
#include <QSortFilterProxyModel>

struct sqlite3;
class PlayerModel;
class OwnerModel;

namespace Ui 
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

    private slots:
        void newProject(bool);
        void openProject(bool);
        void playerInputLineEditChange(const QString&);
        void createTeamEditor(bool);
        void ownerClicked(const QModelIndex&);
        void tradePlayer(bool);

    private:
        static void initFootballPlayers(sqlite3* db);
        static void initDatabase(sqlite3* db);
        void initPlayerModel();
        void initOwnerModel();

    private:
        Ui::MainWindow* _window;
        OwnerInfo* _ownerInfo;

        PlayerModel* _playerModel;
        OwnerModel* _ownerModel;

        QSortFilterProxyModel* _playerProxyModel;
        QSortFilterProxyModel* _ownerProxyModel;

        sqlite3* _db;
};
