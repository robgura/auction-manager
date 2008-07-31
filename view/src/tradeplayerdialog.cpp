
#include "view/parsesql.h"
#include "view/tradeplayerdialog.h"
#include "view/transaction.h"
#include "view/transtypes.h"
#include <QDebug>
#include <QRadioButton>
#include <QTreeWidgetItem>
#include <assert.h>

TradePlayerDialog::TradePlayerDialog(QWidget* parent, sqlite3* db)
    : QDialog(parent)
    , _db(db)
{
    _form.setupUi(this);

    loadTeams();

    loadOwnersPlayers();

    connectButtons();
}

void TradePlayerDialog::connectButtons()
{
    for(RBMap::const_iterator iter = _sellers.begin(); iter != _sellers.end(); ++iter)
    {
        bool value = connect(iter->second, SIGNAL(clicked(bool)), this, SLOT(clicked(bool)));
        assert(value);
    }
}

void TradePlayerDialog::loadOwnersPlayers()
{
    _form.treeWidget->clear();

    int seller = findSelected(_sellers);

    std::string sql = "select NFLPlayers.Key, NFLPlayers.Name, NFLPlayers.Pos, OwnerPlayers.TransType FROM NFLPlayers, OwnerPlayers WHERE OwnerPlayers.PlayerKey=NFLPlayers.Key AND OwnerPlayers.OwnerKey=";
    sql += QString::number(seller).toStdString() + ";";

    Rows rows = ParseSQL::exec(_db, sql);

    std::map<int, QTreeWidgetItem*> items;

    for(Rows::const_iterator iter = rows.begin(); iter != rows.end(); ++iter)
    {
        int playerKey(atoi(iter->at(0).c_str()));
        const std::string playerName(iter->at(1));
        const std::string pos(iter->at(2));
        TransTypes type((TransTypes) atoi(iter->at(3).c_str()));

        if(type == Buy)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem;
            item->setText(0, playerName.c_str());
            item->setText(1, pos.c_str());
            item->setData(0, Qt::UserRole, QVariant(playerKey));
            items.insert(std::map<int, QTreeWidgetItem*>::value_type(playerKey, item));
        }
        else if(type == Sell)
        {
            std::map<int, QTreeWidgetItem*>::iterator iter = items.find(playerKey);
            if(iter != items.end())
            {
                items.erase(iter);
            }
        }
    }

    for(std::map<int, QTreeWidgetItem*>::const_iterator iter = items.begin(); iter != items.end(); ++iter)
    {
        _form.treeWidget->addTopLevelItem(iter->second);
    }

    _form.treeWidget->resizeColumnToContents(0);
    _form.treeWidget->resizeColumnToContents(1);
}

void TradePlayerDialog::loadTeams()
{
    Rows rows = ParseSQL::exec(_db, "select Key, Name FROM Owners");

    for(Rows::const_iterator iter = rows.begin(); iter != rows.end(); ++iter)
    {
        bool first = iter == rows.begin();

        int key = atoi(iter->at(0).c_str());

        QRadioButton* rb = new QRadioButton(iter->at(1).c_str(), _form.sellers);
        _form.sellers->layout()->addWidget(rb);
        _sellers.insert(std::make_pair(key, rb));

        if(first)
        {
            rb->setChecked(true);
        }

        rb = new QRadioButton(iter->at(1).c_str(), _form.buyers);
        _form.buyers->layout()->addWidget(rb);
        _buyers.insert(std::make_pair(key, rb));

        if(first)
        {
            rb->setChecked(true);
        }
    }
}

int TradePlayerDialog::findSelected(const TradePlayerDialog::RBMap& rbMap)
{
    for(RBMap::const_iterator iter = rbMap.begin(); iter != rbMap.end(); ++iter)
    {
        if(iter->second->isChecked())
        {
            return iter->first;
        }
    }
    return -1;
}

void TradePlayerDialog::accept()
{
    if(! _form.treeWidget->selectedItems().isEmpty())
    {
        int player = _form.treeWidget->selectedItems().at(0)->data(0, Qt::UserRole).toInt();

        int seller = findSelected(_sellers);

        int buyer = findSelected(_buyers);

        Transaction::sell(_db, seller, player, _form.spinBox->value());

        Transaction::buy(_db, buyer, player, _form.spinBox->value());

        QDialog::accept();
    }
}

void TradePlayerDialog::clicked(bool)
{
    loadOwnersPlayers();
}
