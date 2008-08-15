
#include "sqlite/sqlite3.h"
#include "view/createteamdialog.h"
#include "view/mainwindow.h"
#include "view/ownermodel.h"
#include "view/parsesql.h"
#include "view/playermodel.h"
#include "view/shouter.h"
#include "view/tradeplayerdialog.h"
#include "view/transaction.h"
#include "view/transtypes.h"
#include "view/ui_defaultownerInfo.h"
#include "view/ui_mainform.h"
#include <QDebug>
#include <QFileDialog>
#include <QTemporaryFile>
#include <assert.h>
#include <fstream>
#include <set>
#include <sstream>

MainWindow::MainWindow()
    : _window(new Ui::MainWindow())
    , _ownerInfo(0)
    , _defaultOwnerInfo(0)
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

    v = connect(_window->actionPlayer_Trade, SIGNAL(triggered(bool)), this, SLOT(launchTradePlayerDialog(bool)));
    assert(v);

    v = connect(_window->actionExport_CSV, SIGNAL(triggered(bool)), this, SLOT(exportCSV(bool)));
    assert(v);

    v = connect(_window->actionGenerate_Test_Data, SIGNAL(triggered(bool)), this, SLOT(generateTestData(bool)));
    assert(v);

    v = connect(_window->playerInput, SIGNAL(textChanged(const QString&)), this, SLOT(playerInputLineEditChange(const QString&)));
    assert(v);

    v = connect(_window->ownerView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(ownerClicked(const QModelIndex&)));
    assert(v);

    v = connect(Shouter::instance(), SIGNAL(transaction()), this, SLOT(trans()));
    assert(v);


    setValidDb(false);
}

MainWindow::~MainWindow()
{
    sqlite3_close(_db);
}

void MainWindow::setValidDb(bool valid)
{
    if(valid)
    {
        _window->actionSave_As->setEnabled(true);
        _window->actionOwner->setEnabled(true);
        _window->actionNFL_Player->setEnabled(true);
        _window->actionPlayer_Trade->setEnabled(true);
        _window->actionExport_CSV->setEnabled(true);
        _window->actionGenerate_Test_Data->setEnabled(true);
    }
    else
    {
        _window->actionSave_As->setEnabled(false);
        _window->actionOwner->setEnabled(false);
        _window->actionNFL_Player->setEnabled(false);
        _window->actionPlayer_Trade->setEnabled(false);
        _window->actionExport_CSV->setEnabled(false);
        _window->actionGenerate_Test_Data->setEnabled(false);
        showDefaultOwnerInfo();
    }
}


void MainWindow::showDefaultOwnerInfo()
{
        delete _ownerInfo;
        _ownerInfo = 0;

        delete _defaultOwnerInfo;
        _defaultOwnerInfo = new QWidget(_window->ownerInfo);
        Ui::defaultOwnerInfo def;
        def.setupUi(_defaultOwnerInfo);
        _window->ownerInfo->layout()->addWidget(_defaultOwnerInfo);
        _defaultOwnerInfo->setEnabled(false);
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

        setValidDb(false);
        setValidDb(true);
    }
}

void outputOwner(sqlite3* db, std::ostream& os, int ownerKey)
{
    std::string sql = "select NFLPlayers.KEY, NFLPlayers.Name, NFLPlayers.Pos, NFLPlayers.Team, OwnerPlayers.TransType, OwnerPlayers.Price FROM NFLPlayers, OwnerPlayers WHERE OwnerPlayers.PlayerKey=NFLPlayers.Key AND OwnerPlayers.OwnerKey=";
    sql += QString::number(ownerKey).toStdString() + ";";
    Rows rows = ParseSQL::exec(db, sql);
    Rows::const_reverse_iterator end = rows.rend();
    std::set<int> ignoreBuy;
    for(Rows::const_reverse_iterator iter = rows.rbegin(); iter != end; ++iter)
    {
        const int playerKey = atoi(iter->at(0).c_str());
        const std::string& playerName = iter->at(1);
        const std::string& playerPos = iter->at(2);
        const std::string& playerTeam = iter->at(3);
        const TransTypes type = static_cast<TransTypes>(atoi(iter->at(4).c_str()));
        const int price = atoi(iter->at(5).c_str());

        if(type == Sell)
        {
            ignoreBuy.insert(playerKey);
        }
        else
        {
            // if this buy should be ignored because the player was sold
            if(ignoreBuy.find(playerKey) != ignoreBuy.end())
            {
                ignoreBuy.erase(playerKey);
            }
            else
            {
                os << "\"" << playerName << "\"" << ","
                   << "\"" << playerPos << "\"" << ","
                   << "\"" << playerTeam << "\"" << ","
                   << price
                   << std::endl;
            }
        }
    }
}

void MainWindow::exportCSV(bool)
{
    QFileDialog dialog(this, "Choose File To Export");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFilter("*.csv");
    dialog.setDefaultSuffix("csv");

    if(dialog.exec())
    {
        QString fileName = dialog.selectedFiles().at(0);

        bool weAreCool = true;
        QFile file(fileName);
        if(file.exists())
        {
            weAreCool = file.remove();
        }

        if(weAreCool)
        {
            std::ofstream outFile(fileName.toStdString().c_str());

            std::string sql = "select KEY, Name, Team_Name FROM Owners;";
            Rows rows = ParseSQL::exec(_db, sql);
            for(Rows::const_iterator iter = rows.begin(); iter != rows.end(); ++iter)
            {
                const int key = atoi(iter->at(0).c_str());
                const std::string& name = iter->at(1);
                const std::string& team = iter->at(2);
                outFile << "\"" << name << "\"" << ","
                        << "\"" << team << "\""
                        << std::endl;

                outputOwner(_db, outFile, key);
                outFile << std::endl;
            }
        }
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

        setValidDb(true);
    }
}

void MainWindow::generateTestData(bool)
{
    _ownerModel->addOwner("Drew", "Dreams");
    _ownerModel->addOwner("Jasmine", "Jugglers");
    _ownerModel->addOwner("Jennifer", "Jelly Rolls");
    _ownerModel->addOwner("Jimmie", "Jalopys");
    _ownerModel->addOwner("Joslynn", "Jives");
    _ownerModel->addOwner("Michelle", "Moth Balls");
    _ownerModel->addOwner("Rob", "Rockets");
    _ownerModel->addOwner("Zac", "Zig");

    Rows rows = ParseSQL::exec(_db, "select Key from NFLPlayers;");

    const unsigned int teams = 8;
    const unsigned int playersPerTeam = 14;
    const unsigned int totalPlayers = teams * playersPerTeam;

    const unsigned int nflSize = rows.size();
    const unsigned int spacing = nflSize / totalPlayers;

    int counter = 0;
    int ownerIndex = 0;
    for(Rows::const_iterator iter = rows.begin(); iter != rows.end(); ++iter, ++counter)
    {

        if((counter % spacing) == 0)
        {
            ownerIndex = ownerIndex % teams;
            const int ownerKey = ownerIndex + 1;

            int nflKey = atoi(iter->at(0).c_str());
            int price = rand() % 12 + 1;
            Transaction::buy(_db, ownerKey, nflKey, price);

            ++ownerIndex;
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
    _window->playerView->resizeColumnToContents(0);
    _window->playerView->resizeColumnToContents(1);
    _window->playerView->resizeColumnToContents(2);
}

void MainWindow::initOwnerModel()
{
    delete _ownerModel;
    _ownerModel = new OwnerModel(_db);

    delete _ownerProxyModel;
    _ownerProxyModel = new QSortFilterProxyModel();
    _ownerProxyModel->setSourceModel(_ownerModel);

    _window->ownerView->setModel(_ownerProxyModel);
    _window->ownerView->resizeColumnToContents(0);
    _window->ownerView->resizeColumnToContents(1);
    _window->ownerView->resizeColumnToContents(2);
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

    const std::string createOwnerPlayer= "create table OwnerPlayers (Key INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, OwnerKey INTEGER NOT NULL, Playerkey INTEGER NOT NULL, TransType INTEGER NON NULL, Price INTEGER NOT NULL);";
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
    delete _defaultOwnerInfo;
    _defaultOwnerInfo = 0;

    delete _ownerInfo;
    _ownerInfo = new OwnerInfo(_window->ownerInfo, _db, ownerIndex.data(OwnerModel::KeyRole).toInt());
    _window->ownerInfo->layout()->addWidget(_ownerInfo);
}

void MainWindow::launchTradePlayerDialog(bool)
{
    TradePlayerDialog dialog(this, _db);
    dialog.exec();
}

void MainWindow::trans()
{
    QModelIndexList indexes = _window->ownerView->selectionModel()->selectedIndexes();

    if(indexes.isEmpty())
    {
        showDefaultOwnerInfo();
    }
    else
    {
        ownerClicked(indexes.at(0));
    }
}
