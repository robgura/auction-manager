
#include "view/shouter.h"

Shouter* Shouter::_shouter(0);

Shouter::Shouter()
{
}

Shouter* Shouter::instance()
{
    if(_shouter == 0)
    {
        _shouter = new Shouter();
    }
    return _shouter;
}

void Shouter::trans()
{
    emit transaction();
}
