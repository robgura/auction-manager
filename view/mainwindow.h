

#pragma once

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

    private:
        static void initDatabase(sqlite3* db);
        void initPlayerModel();
        void initOwnerModel();

    private:
        Ui::MainWindow* _window;

        PlayerModel* _playerModel;
        OwnerModel* _ownerModel;

        QSortFilterProxyModel* _playerProxyModel;
        QSortFilterProxyModel* _ownerProxyModel;

        sqlite3* _db;
};
