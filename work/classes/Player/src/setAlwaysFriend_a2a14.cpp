#include "class.h"

__attribute__((minsize)) extern "C" void Player_setAlwaysFriend(Player *self, bool value)
{
    self->alwaysFriend = value;
    self->enemyFlags = 0x100;
    self->turnedEnemy = 0;
}
