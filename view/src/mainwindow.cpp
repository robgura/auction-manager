
#include "sqlite/sqlite3.h"
#include "view/mainwindow.h"
#include "view/playermodel.h"
#include "view/ui_mainform.h"
#include <QDebug>
#include <QFileDialog>
#include <assert.h>
#include <fstream>
#include <sstream>

MainWindow::MainWindow()
    : _window(new Ui::MainWindow())
    , _playerModel(0)
    , _db(0)
{
    _window->setupUi(this);

    bool v = connect(_window->actionNew, SIGNAL(triggered(bool)), this, SLOT(newProject(bool)));
    assert(v);
}

MainWindow::~MainWindow()
{
    delete _playerModel;

    sqlite3_close(_db);
}

void MainWindow::newProject(bool)
{
    QFileDialog dialog(this, "Choose File");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFilter("*.acx");
    dialog.setDefaultSuffix("acx");

    if(dialog.exec())
    {
        QString fileName = dialog.selectedFiles().at(0);

        if(_db != 0)
        {
            sqlite3_close(_db);
        }

        sqlite3_open(fileName.toStdString().c_str(), &_db);

        initDatabase(_db);

        initPlayerModel();
    }
}

void MainWindow::initPlayerModel()
{
    delete _playerModel;
    _playerModel = new PlayerModel(_db);

    _window->playerView->setModel(_playerModel);
}

void MainWindow::initDatabase(sqlite3* db)
{
    std::string createTable = "create table NFLPlayers (Key INTEGER PRIMARY KEY, Name TEXT NOT NULL, Pos TEXT NOT NULL, Team TEXT);";

    int rc1 = sqlite3_exec(db, createTable.c_str(), 0, 0, 0);
    if(rc1 != SQLITE_OK)
    {
            qDebug() << "aAAA\n";
        qDebug() << sqlite3_errmsg(db);
    }

    std::ifstream inFile("./getplayers/players.dat");
    std::string lineBuf;
    while(std::getline(inFile, lineBuf))
    {
        std::istringstream ist(lineBuf);

        std::string key;
        std::getline(ist, key, '#');

        std::string name;
        std::getline(ist, name, '#');

        std::string pos;
        std::getline(ist, pos, '#');

        std::string team;
        std::getline(ist, team, '#');

        std::string insert("insert into NFLPlayers values (");
        insert += "\"" + key + "\",";
        insert += "\"" + name + "\",";
        insert += "\"" + pos + "\",";
        insert += "\"" + team + "\"";
        insert += ");";

        int rc2 = sqlite3_exec(db, insert.c_str(), 0, 0, 0);
        if(rc2 != SQLITE_OK)
        {
            qDebug() << sqlite3_errmsg(db);
        }
    }
}

