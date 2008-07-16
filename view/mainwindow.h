

#pragma once

#include <QMainWindow>
#include <QStandardItemModel>

struct sqlite3;

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

        QStandardItemModel* _standardItemModel;

        sqlite3* _db;
};
