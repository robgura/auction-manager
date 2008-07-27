
#include "view/ownerinfo.h"
#include "view/parsesql.h"
#include <QTreeWidgetItem>
#include <QString>

OwnerInfo::OwnerInfo(QWidget* parent, sqlite3* db, int ownerKey)
    : QWidget(parent)
    , _db(db)
{
    _form.setupUi(this);

    setupTransactions(ownerKey);
}

void OwnerInfo::setupTransactions(int ownerKey)
{
    std::string sql = "select NFLPlayers.Name, NFLPlayers.Pos, OwnerPlayers.TransType, OwnerPlayers.Price FROM NFLPlayers, OwnerPlayers WHERE OwnerPlayers.PlayerKey=NFLPlayers.Key AND OwnerPlayers.OwnerKey=";
    std::string key_str = QString::number(ownerKey).toStdString();
    sql += key_str + ";";

    Rows rows = ParseSQL::exec(_db, sql);

    for(Rows::const_iterator iter = rows.begin(); iter != rows.end(); ++iter)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, iter->at(0).c_str());
        item->setText(1, iter->at(1).c_str());
        item->setText(2, iter->at(3).c_str());

        _form.treeWidget->addTopLevelItem(item);
    }

}
