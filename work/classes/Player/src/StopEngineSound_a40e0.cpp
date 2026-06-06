#include "class.h"

extern "C" void *gFModSound;
extern "C" void FModSound_stop(void *self, void *event);

__attribute__((minsize)) extern "C" void Player_StopEngineSound(Player *self)
{
    int event = *(int *)((char *)self + 0xf0);
    if (event != 0) {
        FModSound_stop(gFModSound, (void *)event);
        self->field_108 = 0;
        self->engineEvent = 0;
    }
}
