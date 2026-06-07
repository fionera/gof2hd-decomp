#include "class.h"

struct PlayerWormHole {
    void render();
};

extern "C" void AEGeometry_render(void *geometry);

void PlayerWormHole::render()
{
    if (F<uint8_t>(this, 0xf5) == 0)
        return;
    return AEGeometry_render(F<void *>(this, 0x8));
}
