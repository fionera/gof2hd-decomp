#include "class.h"

extern "C" void AEGeometry_render(AEGeometry *self);
extern "C" void PlayerCreature_render_tail(PlayerCreature *self);

struct PlayerCreature {
    void render();
};

void PlayerCreature::render()
{
    AEGeometry *geometry = F<AEGeometry *>(this, 0x78);
    if (geometry != 0) {
        AEGeometry_render(geometry);
    }
    if ((uint32_t)(F<int>(this, 0x88) - 3) >= 2) {
        return PlayerCreature_render_tail(this);
    }
}
