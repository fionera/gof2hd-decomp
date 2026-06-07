#include "class.h"

struct PlayerTurret {
    void setTurretRange(int range);
};

void PlayerTurret::setTurretRange(int range)
{
    I(this, 0x164) = range;
}
