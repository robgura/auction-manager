
#include "view/createteamdialog.h"
#include <assert.h>

CreateTeamDialog::CreateTeamDialog(QWidget* parent, OwnerModel* ownerModel)
    : QDialog(parent)
    , _ownerModel(ownerModel)
{
    _form.setupUi(this);

    _form.ownerView->setModel(_ownerModel);

    bool v = connect(_form.addButton, SIGNAL(clicked(bool)), this, SLOT(addButtonClicked(bool)));
    assert(v);

    v = connect(_form.removeButton, SIGNAL(clicked(bool)), this, SLOT(removeButtonClicked(bool)));
    assert(v);
}


void CreateTeamDialog::addButtonClicked(bool)
{
    const std::string ownerName = _form.ownerName->displayText().toStdString();
    const std::string teamName = _form.teamName->displayText().toStdString();

    _ownerModel->addOwner(ownerName, teamName);
}

void CreateTeamDialog::removeButtonClicked(bool)
{
}

void CreateTeamDialog::load()
{
}
