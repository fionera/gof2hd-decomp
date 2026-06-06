#include "class.h"

extern "C" void *gFModSound;
extern "C" unsigned char FModSound_resume(void *self, void *event);

__attribute__((minsize)) extern "C" void Player_ResumeEngineSound(Player *self, bool force)
{
    void *event = *(void **)((char *)self + 0xf0);
    if (event != 0 && (self->field_f8 != 0 || force)) {
        self->field_f8 = FModSound_resume(gFModSound, event) ^ 1;
    }
}
