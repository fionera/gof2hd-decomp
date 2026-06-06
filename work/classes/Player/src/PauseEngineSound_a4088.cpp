#include "class.h"

extern "C" void *gFModSound;
extern "C" unsigned char FModSound_pause(void *self);

__attribute__((minsize)) extern "C" void Player_PauseEngineSound(Player *self)
{
    int event = *(int *)((char *)self + 0xf0);
    if (event != 0) {
        self->field_f8 = FModSound_pause(gFModSound);
    }
}
