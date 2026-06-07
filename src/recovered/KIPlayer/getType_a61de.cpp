#include "class.h"

extern "C" int KIPlayer_getType(KIPlayer *self)
{
    return *(int *)((char *)self + 0xac);
}
