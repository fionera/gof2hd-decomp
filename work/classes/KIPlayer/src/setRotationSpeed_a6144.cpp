#include "class.h"

extern "C" void KIPlayer_setRotationSpeed(KIPlayer *self, float speed)
{
    *(float *)((char *)self + 0xa8) = speed;
}
