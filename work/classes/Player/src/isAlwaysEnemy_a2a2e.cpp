#include "class.h"

extern "C" unsigned char Player_isAlwaysEnemy(Player *self)
{
    return self->alwaysEnemy;
}
