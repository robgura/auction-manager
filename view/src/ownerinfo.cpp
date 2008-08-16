#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

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
    , _ownerKey(ownerKey)
{
    _form.setupUi(this);

    setupMoney();
    setupTransactions();

    setupOwnerName();

    bool v = connect(_form.deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButton(bool)));
    assert(v);

}

void OwnerInfo::setupOwnerName()
{
    std::string sql = "select Name FROM Owners WHERE Key=";
    std::string key_str = QString::number(_ownerKey).toStdString();
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

void OwnerInfo::setupMoney()
{
    _form.totalCash->setText("$" + QString::number(Transaction::moneyLeft(_db, _ownerKey)));
    _form.maxBid->setText("$" + QString::number(Transaction::availableMoneyLeft(_db, _ownerKey)));
}

Transactions getTransactionData(sqlite3* db, int _ownerKey)
{
    std::string sql = "select NFLPlayers.Key, NFLPlayers.Name, NFLPlayers.Pos, OwnerPlayers.ROWID, OwnerPlayers.TransType, OwnerPlayers.Price FROM NFLPlayers, OwnerPlayers WHERE OwnerPlayers.PlayerKey=NFLPlayers.Key AND OwnerPlayers.OwnerKey=";
    std::string key_str = QString::number(_ownerKey).toStdString();
    sql += key_str + ";";

    Rows rows = ParseSQL::exec(db, sql);

    Transactions trans;
    for(Rows::const_iterator iter = rows.begin(); iter != rows.end(); ++iter)
    {
        Transactions::value_type t;

        t.playerKey = atoi(iter->at(0).c_str());
        t.playerName = iter->at(1);
        t.pos = iter->at(2);
        t.rowId = atoi(iter->at(3).c_str());
        t.type = (TransTypes) atoi(iter->at(4).c_str());
        t.price = iter->at(5);

        trans.push_back(t);
    }

    return trans;
}

/**
 * sets up widget with all owner transactions
 *
 * @version
 * - R Gura         08/14/2008
 *   - created
 */
void OwnerInfo::setupTransactions()
{
    Transactions trans = getTransactionData(_db, _ownerKey);

    std::vector<QTreeWidgetItem*> items;
    for(Transactions::const_iterator iter = trans.begin(); iter != trans.end(); ++iter)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, iter->playerName.c_str());
        item->setText(1, iter->pos.c_str());
        item->setData(0, Data, QVariant::fromValue(*iter));

        if(iter->type == Buy)
        {
            item->setText(2, iter->price.c_str());
        }
        else if(iter->type == Sell)
        {
            item->setText(2, ("(" + iter->price + ")").c_str());
            item->setData(2, Qt::ForegroundRole, QVariant(QColor(Qt::red)));
            item->setData(1, Qt::ForegroundRole, QVariant(QColor(Qt::gray)));
            item->setData(0, Qt::ForegroundRole, QVariant(QColor(Qt::gray)));

            std::vector<QTreeWidgetItem*>::const_reverse_iterator itemEndIter = items.rend();
            for(std::vector<QTreeWidgetItem*>::reverse_iterator itemIter = items.rbegin(); itemIter != itemEndIter; ++itemIter)
            {
                if((*itemIter)->data(0, Data).value<TransactionData>().playerKey == iter->playerKey)
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
        QTreeWidgetItem* item = selection.at(0);
        Transaction::deleteTransaction(_db, item->data(0, Data).value<TransactionData>().rowId);
    }
}
