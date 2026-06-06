#define wchar_t gof2_wchar_t
#include "class.h"
#undef wchar_t

void Item::changeStationAmount(int delta) {
    stationAmount += delta;
}
