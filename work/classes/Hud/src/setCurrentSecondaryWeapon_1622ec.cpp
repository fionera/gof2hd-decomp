#include "class.h"

extern "C" void Hud_secondaryWeaponChanged(Hud *self);

extern "C" void Hud_setCurrentSecondaryWeapon(Hud *self, Item *item)
{
    P(self, 0x258) = item;
    return Hud_secondaryWeaponChanged(self);
}
