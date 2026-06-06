#include "class.h"

__attribute__((minsize)) extern "C" bool Player_isAsteroid(Player *self)
{
    KIPlayer *ki = self->kiPlayer;
    bool result = false;
    if (ki != 0) {
        result = *((char *)ki + 0x3c) != 0;
    }
    return result;
}
