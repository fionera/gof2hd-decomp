#include "class.h"

void Galaxy::setVisited(bool *src, int count)
{
    int i;
    for (i = 0; i < count; i = i + 1) {
        uint8_t *stations = (uint8_t *)this->stations;
        stations[i] = ((uint8_t *)src)[i];
    }
    while (true) {
        if (count >= 0x87)
            return;
        uint8_t *stations = (uint8_t *)this->stations;
        stations[count] = 0;
        count = count + 1;
    }
}
