#include "class.h"

struct PlayerGasCloud {
    uint8_t hasExploded();
};

uint8_t PlayerGasCloud::hasExploded()
{
    return F<uint8_t>(this, 0x154);
}
