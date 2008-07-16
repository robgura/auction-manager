
#include "sqlite/sqlite3.h"
#include "view/mainwindow.h"
#include "view/player.h"
#include "view/ui_mainform.h"
#include <QDebug>
#include <QFileDialog>
#include <assert.h>
#include <fstream>
#include <sstream>

MainWindow::MainWindow()
    : _window(new Ui::MainWindow())
    , _standardItemModel(0)
    , _db(0)
{
    _window->setupUi(this);

    bool v = connect(_window->actionNew, SIGNAL(triggered(bool)), this, SLOT(newProject(bool)));
    assert(v);
}

MainWindow::~MainWindow()
{
    delete _standardItemModel;

    sqlite3_close(_db);
}

void MainWindow::newProject(bool)
{
    QFileDialog dialog(this, "Choose File");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);

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
    delete _standardItemModel;
    _standardItemModel = new QStandardItemModel();

    QStringList headers;
    headers << "Player Name" << "Position" << "Team";

    _standardItemModel->setHorizontalHeaderLabels(headers);

    Player::loadPlayers(_db, _standardItemModel);

    _window->playerView->setModel(_standardItemModel);

    _standardItemModel->setColumnCount(3);
}

void MainWindow::initDatabase(sqlite3* db)
{
    std::string createTable = "create table NFLPlayers (Key INTEGER PRIMARY KEY, Name TEXT NOT NULL, Pos TEXT NOT NULL, Team TEXT);";

    int rc1 = sqlite3_exec(db, createTable.c_str(), 0, 0, 0);
    if(rc1 != 0)
    {
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

        qDebug() << insert.c_str();
        int rc2 = sqlite3_exec(db, insert.c_str(), 0, 0, 0);
        if(rc2 != 0);
        {
            qDebug() << sqlite3_errmsg(db);
        }
    }
}

