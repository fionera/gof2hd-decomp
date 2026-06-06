#include "class.h"

extern "C" void *gAppManager;
extern "C" void **gFModSoundPtr;
extern "C" void MatrixGetPosition(void *out, float *matrix);
extern "C" void *FModSound_updateEvent3DAttributes(void *sound, void *event, void *vec, Vector *pos, Vector *vel, bool b);

struct Mat { float m[12]; };

__attribute__((minsize)) extern "C" void Player_PlayEngineSound(Player *self, Vector *vec)
{
    *(Vector **)((char *)self + 0xf4) = vec;
    if (*((char *)gAppManager + 0xf) != 0) {
        Mat pos;
        MatrixGetPosition(&pos, self->transform);
        void *ev = FModSound_updateEvent3DAttributes(
            gFModSoundPtr[0],
            *(void **)((char *)self + 0xf0),
            *(void **)((char *)self + 0xf4),
            (Vector *)&pos, 0, true);
        *(void **)((char *)self + 0xf0) = ev;
        self->field_108 = 1;
    }
}
