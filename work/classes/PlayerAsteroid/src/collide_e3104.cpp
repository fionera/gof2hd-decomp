#include "class.h"

typedef Vector (*GeometryPositionFn)(AEGeometry *geometry);

__attribute__((visibility("hidden"))) extern GeometryPositionFn PlayerAsteroid_geometryPosition;

extern "C" int Player_getHitpoints(Player *player);

bool PlayerAsteroid::collide(float x, float y, float z)
{
    PlayerAsteroid *self = this;
    if (Player_getHitpoints(self->f_4) > 0) {
        GeometryPositionFn getPosition = PlayerAsteroid_geometryPosition;
        float px = getPosition(self->f_8).x;
        float py = getPosition(self->f_8).y;
        float pz = getPosition(self->f_8).z;
        int radiusInt = F<int>(self->f_4, 0x40);
        float radius = (float)radiusInt;
        float dx = x - px;
        if (dx < radius) {
            float negativeRadius = (float)-radiusInt;
            if (dx > negativeRadius) {
                float dy = y - py;
                if (dy < radius && dy > negativeRadius) {
                    float dz = z - pz;
                    if (dz < radius) {
                        if (dz > negativeRadius) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
