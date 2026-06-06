#include "class.h"

extern "C" int Player_GetEngineEvent(Player *self)
{
    return self->engineEvent;
}
