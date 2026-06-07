#include "class.h"

extern "C" void KIPlayer_setSpacePoints(KIPlayer *self, Array<SpacePoint *> *pts)
{
    *(Array<SpacePoint *> **)((char *)self + 0xcc) = pts;
}
