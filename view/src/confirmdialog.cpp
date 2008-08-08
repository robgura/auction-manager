
#include "sqlite/sqlite3.h"
#include "view/confirmdialog.h"
#include "view/parsesql.h"
#include "view/transaction.h"
#include <QPushButton>
#include <assert.h>

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

    _maxBid = Transaction::availableMoneyLeft(_db, ownerKey);
    verifyPrice(_form.spinBox->value());

    verifyPlayerNotOwned();

    bool v = connect(_form.spinBox, SIGNAL(valueChanged(int)), this, SLOT(verifyPrice(int)));
    assert(v);
}

void ConfirmPurchase::verifyPrice(int i)
{
    if(i > _maxBid)
    {
        _form.status->setText("Rejected");
        QString pre = "Owners bid cannot exceed ";
        _form.reason->setText(pre + QString::number(_maxBid));
        _form.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        _form.status->setText("Accepted");
        _form.reason->clear();
        _form.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void ConfirmPurchase::verifyPlayerNotOwned()
{
    std::string key = QString::number(_playerKey).toStdString();
    const std::string sql = "select Owners.Name, OwnerPlayers.TransType FROM OwnerPlayers, Owners where PlayerKey = " + key + " AND Owners.Key=OwnerPlayers.OwnerKey;";
    Rows rows = ParseSQL::exec(_db, sql);

    // the player is already owned, iterator backward through transactions to find the last buy of this player
    // that is who currently owns him
    Rows::const_reverse_iterator end = rows.rend();
    for(Rows::const_reverse_iterator iter = rows.rbegin(); iter != end; ++iter)
    {
        if(iter->at(1) == "0")
        {
            _form.status->setText("Rejected");
            QString pre = "Player is already owned by: ";
            _form.reason->setText(pre + iter->at(0).c_str());
            _form.spinBox->setEnabled(false);
            _form.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            return;
        }
    }

}

void ConfirmPurchase::loadNflData()
{
    std::string key = QString::number(_playerKey).toStdString();
    const std::string sql = "select Name, Pos, Team from NFLPlayers where Key = " + key + ";";

    Rows rows = ParseSQL::exec(_db, sql);

    _form.playerName->setText(rows.at(0).at(0).c_str());
    _form.playerTeam->setText(rows.at(0).at(1).c_str());
    _form.pos->setText(rows.at(0).at(2).c_str());

}

void ConfirmPurchase::loadOwnerData()
{
    std::string key = QString::number(_ownerKey).toStdString();
    const std::string sql = "select Name, Team_Name from Owners where Key = " + key + ";";

    Rows rows = ParseSQL::exec(_db, sql);

    _form.ownerName->setText(rows.at(0).at(0).c_str());
    _form.ownerName->setText(rows.at(0).at(1).c_str());
}

void ConfirmPurchase::accept()
{
    _price = _form.spinBox->value();
    QDialog::accept();
}
