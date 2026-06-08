#include "class.h"

void Galaxy::visitStation(int index)
{
    uint8_t *stations = (uint8_t *)P(this, 0x0);
    stations[index] = 1;
}
