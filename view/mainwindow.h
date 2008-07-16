

#pragma once

#include <QMainWindow>
#include <QSortFilterProxyModel>

struct sqlite3;
class PlayerModel;

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

    private:
        static void initDatabase(sqlite3* db);
        void initPlayerModel();

    private:
        Ui::MainWindow* _window;

        PlayerModel* _playerModel;

        QSortFilterProxyModel* _proxyModel;

        sqlite3* _db;
};
