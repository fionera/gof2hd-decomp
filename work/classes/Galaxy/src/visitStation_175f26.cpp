#include "class.h"

void Galaxy::visitStation(int index)
{
    uint8_t *stations = (uint8_t *)this->stations;
    stations[index] = 1;
}
