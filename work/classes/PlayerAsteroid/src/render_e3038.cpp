#include "class.h"

extern "C" void AEGeometry_render(AEGeometry *geometry);
extern "C" void AEGeometry_renderBase(AEGeometry *geometry);
extern "C" void Explosion_render(Explosion *explosion);

void PlayerAsteroid::render()
{
    if (F<uint8_t>(this, 0xf5) != 0) {
        if (F<AEGeometry *>(this, 0x78) != 0) {
            AEGeometry_render(F<AEGeometry *>(this, 0x78));
        }
        int state = F<int>(this, 0x88);
        if (state != 0) {
            if (state == 3) {
                return Explosion_render(F<Explosion *>(this, 0x12c));
            }
            return;
        }
        return AEGeometry_renderBase(F<AEGeometry *>(this, 0x8));
    }
}
