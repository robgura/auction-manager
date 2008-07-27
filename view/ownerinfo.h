
#pragma once

#include <QWidget>
#include "view/ui_ownerInfo.h"

class OwnerInfo : public QWidget
{
    public:
        OwnerInfo(QWidget* parent, int ownerKey);

    private:
        Ui::Form _form;
};
