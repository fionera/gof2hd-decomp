#include "class.h"

extern "C" Array<Player *> *Player_getEnemies(Player *self)
{
    return self->enemies;
}
