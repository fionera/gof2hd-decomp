#include "class.h"

extern "C" void *Vector_assign(void *self, char *other);
extern "C" char *MatrixGetPosition(char *out, Matrix const *matrix);

void IParticleSystem::resetEmitterVelocity()
{
    char value[12] = {};
    Vector_assign((char *)this + 0x1c, value);
    *(volatile uint8_t *)((char *)this + 0x5) = 1;
    char *matrixValue = value;
    MatrixGetPosition(matrixValue, this->f_18);
    Vector_assign((char *)this + 0x28, matrixValue);
    *(volatile uint8_t *)((char *)this + 0x4) = 0;
}
