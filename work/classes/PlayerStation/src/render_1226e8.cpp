#include "class.h"

extern "C" void AEGeometry_render(void *geometry);

void PlayerStation::render()
{
    if (F<uint8_t>(this, 0xf5) == 0) {
        return;
    }
    return AEGeometry_render(P(this, 0x140));
}
