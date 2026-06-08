#include "class.h"

void MineGun::setPlayer(PlayerEgo *player)
{
    P(this, 0xb0) = player;
}
