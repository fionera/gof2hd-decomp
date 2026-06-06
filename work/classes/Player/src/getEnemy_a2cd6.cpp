#include "class.h"

extern "C" Player *Player_getEnemy(Player *self, int index)
{
    return self->enemies->data[index];
}
