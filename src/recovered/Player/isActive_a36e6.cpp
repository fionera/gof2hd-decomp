#include "class.h"

extern "C" unsigned char Player_isActive(Player *self)
{
    return self->active;
}
