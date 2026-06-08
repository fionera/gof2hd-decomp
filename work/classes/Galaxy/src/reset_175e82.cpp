#include "class.h"

void Galaxy::reset()
{
    uint8_t *stations = (uint8_t *)this->stations;
    for (int i = 0; i != 0x87; i = i + 1) {
        stations[i] = 0;
    }
}
