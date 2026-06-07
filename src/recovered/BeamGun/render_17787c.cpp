#include "class.h"

extern "C" void AEGeometry_render(AEGeometry *self);
extern "C" void BeamGun_render_tail(AEGeometry *self);

struct BeamGun {
    void render();
};

void BeamGun::render()
{
    Gun *gun = F<Gun *>(this, 0x8);
    if (F<uint8_t>(gun, 0x4c) == 0)
        return;

    AEGeometry_render(F<AEGeometry *>(this, 0x18));

    if (F<uint8_t>(this, 0x21) != 0) {
        AEGeometry *secondary = F<AEGeometry *>(this, 0x1c);
        if (secondary != 0)
            return BeamGun_render_tail(secondary);
    }
}
