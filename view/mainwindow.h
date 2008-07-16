

#pragma once

#include <QMainWindow>

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

    private:
        static void initDatabase(sqlite3* db);
        void initPlayerModel();

    private:
        Ui::MainWindow* _window;

        PlayerModel* _playerModel;

        sqlite3* _db;
};
