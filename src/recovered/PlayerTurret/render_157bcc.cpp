#include "class.h"

extern "C" void AEGeometry_render(void *self);
extern "C" void Explosion_render(Explosion *self);
extern "C" void PlayerTurret_renderBase(PlayerTurret *self);

struct PlayerTurret {
    void render();
};

void PlayerTurret::render()
{
    void *visible = P(this, 0x78);
    if (visible != 0) {
        AEGeometry_render(visible);
    }
    int state = I(this, 0x88);
    if (state == 3) {
        Explosion_render(TP<Explosion>(this, 0x13c));
        state = I(this, 0x88);
    }
    if ((uint32_t)(state - 3) >= 2) {
        PlayerTurret_renderBase(this);
    }
}
