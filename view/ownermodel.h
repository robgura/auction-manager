
#pragma once

#include "view/owner.h"
#include <vector>
#include <QAbstractItemModel>

struct sqlite3;

    //sqlite3_exec(db, "insert into Owners values (null, \"Drew\", \"Beta\"); ", 0, 0, 0);
    //sqlite3_exec(db, "insert into Owners values (null, \"Michelle\", \"Delta\"); ", 0, 0, 0);
    //sqlite3_exec(db, "insert into Owners values (null, \"Rob\", \"Alpha\"); ", 0, 0, 0);
    //sqlite3_exec(db, "insert into Owners values (null, \"Zac\", \"Gamma\"); ", 0, 0, 0);
class OwnerModel : public QAbstractItemModel
{
    public:
        OwnerModel(sqlite3* db);

        virtual Qt::ItemFlags flags(const QModelIndex & index) const;
        virtual QModelIndex index(int, int, const QModelIndex&) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex&) const;
        virtual int columnCount(const QModelIndex&) const;
        virtual QVariant data(const QModelIndex&, int) const;

        void addOwner(const std::string& owner, const std::string& teamName);

    private:
        class LoadOwnerHandler;

        sqlite3* _db;

        typedef std::vector<Owner> Owners;
        Owners _ownerCache;
};

