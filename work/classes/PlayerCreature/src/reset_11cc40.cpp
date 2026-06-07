#include "class.h"

extern "C" void KIPlayer_reset(KIPlayer *self);
extern "C" void KIPlayer_setActive(KIPlayer *self, bool active);
extern "C" int Player_getHitpoints(Player *self);
extern "C" void Matrix_assign(Matrix *dst, const float *src);

struct PlayerCreature {
    void reset();
};

void PlayerCreature::reset()
{
    KIPlayer_reset((KIPlayer *)this);
    F<int>(this, 0x88) = 0;
    KIPlayer_setActive((KIPlayer *)this, true);
    F<int>(this, 0x138) = F<int>(this, 0x134);
    F<int>(this, 0x13c) = Player_getHitpoints(F<Player *>(this, 0x4));

    char matrixBytes[0x3c];
    float *matrix = (float *)matrixBytes;
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;
    matrix[12] = 1.0f;
    matrix[13] = 1.0f;
    matrix[14] = 1.0f;
    Matrix_assign((Matrix *)((char *)this + 0x144), matrix);
}
