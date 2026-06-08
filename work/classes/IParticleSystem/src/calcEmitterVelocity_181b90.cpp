#include "class.h"

extern "C" char *MatrixGetPosition(char *out, Matrix const *matrix);
extern "C" void *Vector_sub(void *out, char *a, void *b);
extern "C" void *Vector_mul(void *out, char *a, float scale);
extern "C" void *Vector_assign(void *self, char *other);

void IParticleSystem::calcEmitterVelocity(int delta)
{
    char position[12];
    char scaled[12];
    char diff[12];
    MatrixGetPosition(position, this->f_18);
    Vector_sub(diff, position, (char *)this + 0x28);
    Vector_mul(scaled, diff, 1000.0f / (float)delta);
    Vector_assign((char *)this + 0x1c, scaled);
    U8(this, 0x5) = 0;
    Vector_assign((char *)this + 0x28, position);
}
