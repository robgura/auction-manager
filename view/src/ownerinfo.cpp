
#include "view/ownerinfo.h"
#include "view/parsesql.h"
#include "view/transaction.h"
#include "view/transtypes.h"
#include <QString>
#include <QTreeWidgetItem>
#include <assert.h>

OwnerInfo::OwnerInfo(QWidget* parent, sqlite3* db, int ownerKey)
    : QWidget(parent)
    , _db(db)
{
    _form.setupUi(this);

    setupMoney(ownerKey);
    setupTransactions(ownerKey);

    setupOwnerName(ownerKey);

    bool v = connect(_form.deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButton(bool)));
    assert(v);

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

/**
 * sets up widget with all owner transactions
 *
 * @version
 * - R Gura         08/14/2008
 *   - created
 */
void OwnerInfo::setupTransactions(int ownerKey)
{
    std::string sql = "select NFLPlayers.Key, NFLPlayers.Name, NFLPlayers.Pos, OwnerPlayers.Key, OwnerPlayers.TransType, OwnerPlayers.Price FROM NFLPlayers, OwnerPlayers WHERE OwnerPlayers.PlayerKey=NFLPlayers.Key AND OwnerPlayers.OwnerKey=";
    std::string key_str = QString::number(ownerKey).toStdString();
    sql += key_str + ";";

    Rows rows = ParseSQL::exec(_db, sql);

    std::vector<QTreeWidgetItem*> items;
    for(Rows::const_iterator iter = rows.begin(); iter != rows.end(); ++iter)
    {
        const int playerKey = atoi(iter->at(0).c_str());
        const std::string playerName(iter->at(1));
        const std::string pos(iter->at(2));
        int transactionKey = atoi(iter->at(3).c_str());
        TransTypes type((TransTypes) atoi(iter->at(4).c_str()));
        const std::string price(iter->at(5));

        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, playerName.c_str());
        item->setText(1, pos.c_str());
        item->setData(0, PlayerKey, QVariant(playerKey));

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

            std::vector<QTreeWidgetItem*>::const_reverse_iterator itemEndIter = items.rend();
            for(std::vector<QTreeWidgetItem*>::reverse_iterator itemIter = items.rbegin(); itemIter != itemEndIter; ++itemIter)
            {
                if((*itemIter)->data(0, PlayerKey).toInt() == playerKey)
                {
                    (*itemIter)->setData(1, Qt::ForegroundRole, QVariant(QColor(Qt::gray)));
                    (*itemIter)->setData(0, Qt::ForegroundRole, QVariant(QColor(Qt::gray)));
                }
            }
        }

        items.push_back(item);
        _form.treeWidget->addTopLevelItem(item);
    }

    _form.treeWidget->resizeColumnToContents(0);
    _form.treeWidget->resizeColumnToContents(1);
    _form.treeWidget->resizeColumnToContents(2);

}

void OwnerInfo::deleteButton(bool)
{
    QList<QTreeWidgetItem*> selection = _form.treeWidget->selectedItems();
    if(selection.size() == 1)
    {
    }

}
