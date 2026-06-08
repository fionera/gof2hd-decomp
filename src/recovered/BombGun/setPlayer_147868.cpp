#include "class.h"

struct BombGun {
    void setPlayer(PlayerEgo *player);
};

void BombGun::setPlayer(PlayerEgo *player)
{
    F<PlayerEgo *>(this, 0xec) = player;
}
