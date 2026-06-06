#include "class.h"

extern "C" unsigned char Player_isAlwaysFriend(Player *self)
{
    return self->alwaysFriend;
}
