
#include "view/ownerinfo.h"
#include "view/parsesql.h"
#include "view/transaction.h"
#include "view/transtypes.h"
#include <QTreeWidgetItem>
#include <QString>

OwnerInfo::OwnerInfo(QWidget* parent, sqlite3* db, int ownerKey)
    : QWidget(parent)
    , _db(db)
{
    _form.setupUi(this);

    setupMoney(ownerKey);
    setupTransactions(ownerKey);

    setupOwnerName(ownerKey);
}

void OwnerInfo::setupOwnerName(int ownerKey)
{
    std::string sql = "select Name FROM Owners WHERE Key=";
    std::string key_str = QString::number(ownerKey).toStdString();
    sql += key_str + ";";

    Rows rows = ParseSQL::exec(_db, sql);

    if(rows.empty())
    {
        _form.ownerName->setText("Can't Find Owner Name");
    }
    else
    {
        _form.ownerName->setText(rows.at(0).at(0).c_str());
    }
}

void OwnerInfo::setupMoney(int ownerKey)
{
    _form.totalCash->setText("$" + QString::number(Transaction::moneyLeft(_db, ownerKey)));
    _form.maxBid->setText("$" + QString::number(Transaction::availableMoneyLeft(_db, ownerKey)));
}

void OwnerInfo::setupTransactions(int ownerKey)
{
    std::string sql = "select NFLPlayers.Name, NFLPlayers.Pos, OwnerPlayers.TransType, OwnerPlayers.Price FROM NFLPlayers, OwnerPlayers WHERE OwnerPlayers.PlayerKey=NFLPlayers.Key AND OwnerPlayers.OwnerKey=";
    std::string key_str = QString::number(ownerKey).toStdString();
    sql += key_str + ";";

    Rows rows = ParseSQL::exec(_db, sql);

    for(Rows::const_iterator iter = rows.begin(); iter != rows.end(); ++iter)
    {
        const std::string playerName(iter->at(0));
        const std::string pos(iter->at(1));
        const std::string price(iter->at(3));
        TransTypes type((TransTypes) atoi(iter->at(2).c_str()));

        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, playerName.c_str());
        item->setText(1, pos.c_str());

        if(type == Buy)
        {
            item->setText(2, price.c_str());
        }
        else if(type == Sell)
        {
            item->setText(2, ("(" + price + ")").c_str());
            item->setData(2, Qt::ForegroundRole, QVariant(QColor(Qt::red)));
            item->setData(1, Qt::ForegroundRole, QVariant(QColor(Qt::gray)));
            item->setData(0, Qt::ForegroundRole, QVariant(QColor(Qt::gray)));
        }

        _form.treeWidget->addTopLevelItem(item);
    }

    _form.treeWidget->resizeColumnToContents(0);
    _form.treeWidget->resizeColumnToContents(1);
    _form.treeWidget->resizeColumnToContents(2);

}
