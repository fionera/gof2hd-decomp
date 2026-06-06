#include "class.h"

__attribute__((minsize)) extern "C" void Player_setAlwaysEnemy(Player *self, bool value)
{
    self->alwaysEnemy = value;
    self->enemyFlags = 1;
    self->turnedEnemy = 1;
}
