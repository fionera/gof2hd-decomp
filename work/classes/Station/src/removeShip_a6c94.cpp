#include "class.h"

struct Ship;
// ArrayRemove<Ship*>(Ship* ship, Array* ships) — tail-called veneer.
extern "C" void Station_arrayRemoveShip(Ship *ship, void *ships);

// Station::removeShip(Ship*) — this in r0, ship in r1.
extern "C" void Station_removeShip(Station *self, Ship *ship)
{
    void *ships = self->f_2c;
    if (ships == 0)
        return;
    Station_arrayRemoveShip(ship, ships);
}
