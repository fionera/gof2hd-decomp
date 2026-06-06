#include "class.h"

struct Status {
    Ship *getShip();
};

struct Ship {
    int getCurrentLoad();
    int getMaxLoad();
};

extern Status *status;

__attribute__((minsize)) bool Item::checkCargoSpace()
{
    int currentLoad = status->getShip()->getCurrentLoad();
    int requiredLoad = amount + currentLoad;
    int maxLoad = status->getShip()->getMaxLoad();
    return requiredLoad <= maxLoad;
}
