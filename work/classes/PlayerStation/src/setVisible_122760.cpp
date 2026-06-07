#include "class.h"

void PlayerStation::setVisible(bool visible)
{
    F<uint8_t>(this, 0xf5) = visible;
}
