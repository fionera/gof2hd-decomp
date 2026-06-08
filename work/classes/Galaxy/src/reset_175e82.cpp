#include "class.h"

void Galaxy::reset()
{
    uint8_t *stations = (uint8_t *)P(this, 0x0);
    for (int i = 0; i != 0x87; i = i + 1) {
        stations[i] = 0;
    }
}
