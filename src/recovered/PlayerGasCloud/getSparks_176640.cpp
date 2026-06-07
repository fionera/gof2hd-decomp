#include "class.h"

struct PlayerGasCloud {
    void *getSparks();
};

void *PlayerGasCloud::getSparks()
{
    return F<void *>(this, 0x138);
}
