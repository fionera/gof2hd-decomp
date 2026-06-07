#include "class.h"

struct Trail;
extern "C" void *Trail_dtor(Trail *p);
extern "C" void operator_delete(void *p);

extern "C" void PlayerFighter_removeTrail(PlayerFighter *self)
{
    Trail *t = F<Trail *>(self, 0x154);
    if (t != 0) {
        operator_delete(Trail_dtor(t));
    }
    F<Trail *>(self, 0x154) = 0;
}
