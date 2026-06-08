#include "class.h"

extern "C" void Hud_secondaryWeaponChanged(Hud *self);

extern "C" void Hud_setCurrentSecondaryWeapon(Hud *self, Item *item)
{
    self->f_258 = item;
    return Hud_secondaryWeaponChanged(self);
}
