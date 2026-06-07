#include "class.h"

uint8_t Radar::hasScanner()
{
    return F<uint8_t>(this, 0x1ab);
}
