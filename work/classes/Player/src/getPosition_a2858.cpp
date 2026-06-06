#include "class.h"

extern "C" void MatrixGetPosition(void *out, float *matrix);

__attribute__((minsize)) extern "C" void Player_getPosition(void *out, Player *self)
{
    MatrixGetPosition(out, self->transform);
}
