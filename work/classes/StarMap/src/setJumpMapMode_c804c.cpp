#include "class.h"

void StarMap::setJumpMapMode(bool enabled, bool value)
{
    volatile uint8_t *bytes = (volatile uint8_t *)this;
    bytes[0xab] = (uint8_t)value;
    bytes[0xaa] = (uint8_t)enabled;
}
