
#pragma once

#include "view/ui_create_team.h"
#include "view/ownermodel.h"
#include <QDialog>

class CreateTeamDialog : public QDialog
{
    Q_OBJECT

    public:
        CreateTeamDialog(QWidget* parent, OwnerModel* ownerModel);

    private:
        Ui::CreateTeams _form;

    private:
        void load();

    private slots:
        void addButtonClicked(bool);
        void removeButtonClicked(bool);

    private:
        OwnerModel* _ownerModel;
};

