#include "class.h"

extern "C" Array<SpacePoint *> *KIPlayer_getSpacePoints(KIPlayer *self)
{
    return *(Array<SpacePoint *> **)((char *)self + 0xcc);
}
