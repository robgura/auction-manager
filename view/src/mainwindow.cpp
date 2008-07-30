
#include "sqlite/sqlite3.h"
#include "view/createteamdialog.h"
#include "view/mainwindow.h"
#include "view/ownermodel.h"
#include "view/parsesql.h"
#include "view/playermodel.h"
#include "view/ui_mainform.h"
#include <QDebug>
#include <QFileDialog>
#include <QTemporaryFile>
#include <assert.h>
#include <fstream>
#include <sstream>

MainWindow::MainWindow()
    : _window(new Ui::MainWindow())
    , _ownerInfo(0)
    , _playerModel(0)
    , _ownerModel(0)
    , _playerProxyModel(0)
    , _ownerProxyModel(0)
    , _db(0)
{
    _window->setupUi(this);

    bool v = connect(_window->actionNew, SIGNAL(triggered(bool)), this, SLOT(newProject(bool)));
    assert(v);

    v = connect(_window->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openProject(bool)));
    assert(v);

    v = connect(_window->actionOwner, SIGNAL(triggered(bool)), this, SLOT(createTeamEditor(bool)));
    assert(v);

    v = connect(_window->playerInput, SIGNAL(textChanged(const QString&)), this, SLOT(playerInputLineEditChange(const QString&)));
    assert(v);

    v = connect(_window->ownerView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(ownerClicked(const QModelIndex&)));
    assert(v);

}

MainWindow::~MainWindow()
{
    sqlite3_close(_db);
}

void MainWindow::openProject(bool)
{
    QFileDialog dialog(this, "Choose File To Open");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFilter("*.acx");

    if(dialog.exec())
    {
        QString fileName = dialog.selectedFiles().at(0);

        if(_db != 0)
        {
            sqlite3_close(_db);
        }

        sqlite3_open(fileName.toStdString().c_str(), &_db);

        initPlayerModel();

        initOwnerModel();
    }
}

void MainWindow::newProject(bool)
{
    QFileDialog dialog(this, "Choose File To Create");
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

        bool weAreCool = true;
        QFile file(fileName);
        if(file.exists())
        {
            weAreCool = file.remove();
        }

        if(weAreCool)
        {
            sqlite3_open(fileName.toStdString().c_str(), &_db);

            initDatabase(_db);

            initPlayerModel();

            initOwnerModel();
        }
    }
}

void MainWindow::initPlayerModel()
{
    delete _playerModel;
    _playerModel = new PlayerModel(_db);

    delete _playerProxyModel;
    _playerProxyModel = new QSortFilterProxyModel();
    _playerProxyModel->setSourceModel(_playerModel);

    _window->playerView->setModel(_playerProxyModel);
}

void MainWindow::initOwnerModel()
{
    delete _ownerModel;
    _ownerModel = new OwnerModel(_db);

    delete _ownerProxyModel;
    _ownerProxyModel = new QSortFilterProxyModel();
    _ownerProxyModel->setSourceModel(_ownerModel);

    _window->ownerView->setModel(_ownerProxyModel);
}

void MainWindow::playerInputLineEditChange(const QString& newFilter)
{
    _playerProxyModel->setFilterRegExp(QRegExp(newFilter, Qt::CaseInsensitive));
}

void MainWindow::initDatabase(sqlite3* db)
{
    initFootballPlayers(db);

    const std::string createOwners= "create table Owners (Key INTEGER PRIMARY KEY, Name TEXT NOT NULL, Team_Name TEXT NOT NULL);";
    if(sqlite3_exec(db, createOwners.c_str(), 0, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
    }

    const std::string createOwnerPlayer= "create table OwnerPlayers (OwnerKey INTEGER NOT NULL, Playerkey INTEGER NOT NULL, TransType INTEGER NON NULL, Price INTEGER NOT NULL);";
    if(sqlite3_exec(db, createOwnerPlayer.c_str(), 0, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
    }
}

void MainWindow::initFootballPlayers(sqlite3* db)
{
    const std::string createTable = "create table NFLPlayers (Key INTEGER PRIMARY KEY, Name TEXT NOT NULL, Pos TEXT NOT NULL, Team TEXT);";

    if(sqlite3_exec(db, createTable.c_str(), 0, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
    }

    QTemporaryFile* tempFile = QTemporaryFile::createLocalFile(":/res/players.dat");

    std::ifstream inFile(tempFile->fileName().toStdString().c_str());
    std::string lineBuf;


    if(sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
    }

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

        if(sqlite3_exec(db, insert.c_str(), 0, 0, 0) != SQLITE_OK)
        {
            qDebug() << sqlite3_errmsg(db);
        }
    }
    if(sqlite3_exec(db, "COMMIT;", 0, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
    }

    delete tempFile;

    Rows rows = ParseSQL::exec(db, "select DISTINCT Team from NFLPlayers");

    int key = -1;
    for(Rows::const_iterator iter = rows.begin(); iter != rows.end(); ++iter, --key)
    {
        std::string key_str = QString::number(key).toStdString();

        std::string insert("insert into NFLPlayers values (");
        insert += "\"" + key_str + "\",";
        insert += "\"" + iter->at(0) + "\",";
        insert += "\"Defensive Team\",";
        insert += "\"" + iter->at(0) + "\"";
        insert += ");";

        if(sqlite3_exec(db, insert.c_str(), 0, 0, 0) != SQLITE_OK)
        {
            qDebug() << sqlite3_errmsg(db);
        }
    }
}

void MainWindow::createTeamEditor(bool)
{
    CreateTeamDialog dialog(this, _ownerModel);
    dialog.exec();
}

void MainWindow::ownerClicked(const QModelIndex& ownerIndex)
{
    delete _ownerInfo;
    _ownerInfo = new OwnerInfo(_window->ownerInfo, _db, ownerIndex.data(OwnerModel::KeyRole).toInt());
    _window->ownerInfo->layout()->addWidget(_ownerInfo);
}

