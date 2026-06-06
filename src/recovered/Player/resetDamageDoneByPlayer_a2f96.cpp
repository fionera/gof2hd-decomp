#include "class.h"

extern "C" void Player_resetDamageDoneByPlayer(Player *self)
{
    *(int *)((char *)self + 0xdc) = 0;
    self->damageDoneByPlayer = 0;
    self->turnedEnemy = 0;
}
