#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "view/ownerinfo.h"

OwnerInfo::OwnerInfo(QWidget* parent, int ownerKey)
    : QWidget(parent)
{
cout << "Here\n";
    _form.setupUi(this);
}
