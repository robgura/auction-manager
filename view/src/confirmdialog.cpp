
#include "view/confirmdialog.h"
#include "sqlite/sqlite3.h"
#include <QDebug>


ConfirmPurchase::ConfirmPurchase(QWidget* parent, sqlite3* db, int ownerKey, int playerKey)
    : QDialog(parent)
    , _db(db)
    , _ownerKey(ownerKey)
    , _playerKey(playerKey)
    , _price(0)
{
    _form.setupUi(this);

    loadNflData();
    loadOwnerData();
}

class NFL
{
    public:
        static std::string name;
        static std::string pos;
        static std::string team;

        static int handle(void*, int columns, char** columnData, char** columnNames)
        {
            name = columnData[0];
            pos = columnData[1];
            team = columnData[2];
            return 0;
        }
};

std::string NFL::name;
std::string NFL::team;
std::string NFL::pos;

void ConfirmPurchase::loadNflData()
{
    std::string key = QString::number(_playerKey).toStdString();
    const std::string sql = "select Name, Pos, Team from NFLPlayers where Key = " + key + ";";
    if(sqlite3_exec(_db, sql.c_str(), NFL::handle, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(_db);
    }

    _form.playerName->setText(NFL::name.c_str());
    _form.playerTeam->setText(NFL::team.c_str());
    _form.pos->setText(NFL::pos.c_str());

}

class OwnerStatic
{
    public:
        static std::string name;
        static std::string team;

        static int handle(void*, int columns, char** columnData, char** columnNames)
        {
            name = columnData[0];
            team = columnData[1];
            return 0;
        }
};

std::string OwnerStatic::name;
std::string OwnerStatic::team;

void ConfirmPurchase::loadOwnerData()
{
    std::string key = QString::number(_ownerKey).toStdString();
    const std::string sql = "select Name, Team_Name from Owners where Key = " + key + ";";
    if(sqlite3_exec(_db, sql.c_str(), OwnerStatic::handle, 0, 0) != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(_db);
    }

    _form.ownerName->setText(OwnerStatic::name.c_str());
    _form.ownerTeam->setText(OwnerStatic::team.c_str());
}

void ConfirmPurchase::accept()
{
    _price = _form.spinBox->value();
    QDialog::accept();
}
