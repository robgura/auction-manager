
#pragma once

#include "view/owner.h"
#include <vector>
#include <QAbstractItemModel>

struct sqlite3;

class OwnerModel : public QAbstractItemModel
{
    public:
        enum Role
        {
            KeyRole = Qt::UserRole
        };

    public:
        OwnerModel(sqlite3* db);

        virtual Qt::ItemFlags flags(const QModelIndex & index) const;
        virtual QModelIndex index(int, int, const QModelIndex&) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex&) const;
        virtual int columnCount(const QModelIndex&) const;
        virtual QVariant data(const QModelIndex&, int) const;

        virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
        virtual QStringList mimeTypes() const;

        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

        void addOwner(const std::string& owner, const std::string& teamName);

    private:
        void purchase(const int ownerKey, const int playerKey, const int price);

    private:
        class LoadOwnerHandler;

        sqlite3* _db;

        typedef std::vector<Owner> Owners;
        Owners _ownerCache;
};

