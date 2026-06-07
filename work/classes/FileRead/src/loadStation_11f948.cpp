#include "class.h"

__attribute__((minsize))
int32_t FileRead::loadStation(int32_t id)
{
    int16_t *selected = new int16_t[1];
    selected[0] = (int16_t)id;
    Array<Station *> *stations = ((FileRead *)selected)->loadStationsBinary(selected, 1);
    int32_t result = *(int32_t *)stations->data;
    delete[] selected;
    return result;
}
