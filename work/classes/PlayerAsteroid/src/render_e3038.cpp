#include "class.h"

extern "C" void AEGeometry_render(AEGeometry *geometry);
extern "C" void AEGeometry_renderBase(AEGeometry *geometry);
extern "C" void Explosion_render(Explosion *explosion);

void PlayerAsteroid::render()
{
    if (this->f_f5 != 0) {
        if (this->f_78 != 0) {
            AEGeometry_render(this->f_78);
        }
        int state = this->f_88;
        if (state != 0) {
            if (state == 3) {
                return Explosion_render(this->f_12c);
            }
            return;
        }
        return AEGeometry_renderBase(this->f_8);
    }
}
