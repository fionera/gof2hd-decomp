#include "class.h"

// Hud::checkIfQuickMenuIsEmpty() — decides whether the radial quick-menu has any usable
// entry (a secondary weapon, jump drive, wingmen, or cloak) and stores the "empty" flag at
// +0x283, then tail-calls the menu-refresh hook.
extern "C" void *Status_getShip();
extern "C" void *Ship_getEquipment(void *ship, int slot);
extern "C" int  Ship_hasJumpDrive(void *ship);
extern "C" int  Status_getWingmen();
extern "C" unsigned char Ship_hasCloak(void *ship);
extern "C" unsigned char Ship_hasCloakNeg(void *ship); // the final inverted-cloak accessor
extern "C" void Hud_refreshQuickMenu(Hud *self);       // DAT_001ac644 tail thunk

extern "C" Hud *Hud_checkIfQuickMenuIsEmpty(Hud *self)
{
    void *ship = Status_getShip();
    unsigned int *equip = (unsigned int *)Ship_getEquipment(ship, 1);
    self->f_25c = equip;

    unsigned char empty;
    bool hasSecondary = false;
    if (equip != 0) {
        for (unsigned int i = 0; i < equip[0]; i++) {
            if (*(int *)(equip[1] + i * 4) != 0) { hasSecondary = true; break; }
        }
    }
    if (hasSecondary) {
        empty = 0;
    } else {
        Status_getShip();
        if (Ship_hasJumpDrive(Status_getShip()) == 0 && Status_getWingmen() == 0) {
            Status_getShip();
            empty = (unsigned char)(Ship_hasCloakNeg(Status_getShip()) ^ 1);
        } else {
            empty = 0;
        }
    }
    UC(self, 0x283) = empty;
    Hud_refreshQuickMenu(self);
    return self;
}
