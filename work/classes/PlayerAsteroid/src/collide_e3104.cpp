#include "class.h"

typedef Vector (*GeometryPositionFn)(AEGeometry *geometry);

__attribute__((visibility("hidden"))) extern GeometryPositionFn PlayerAsteroid_geometryPosition;

extern "C" int Player_getHitpoints(Player *player);

bool PlayerAsteroid::collide(float x, float y, float z)
{
    PlayerAsteroid *self = this;
    if (Player_getHitpoints(F<Player *>(self, 0x4)) > 0) {
        GeometryPositionFn getPosition = PlayerAsteroid_geometryPosition;
        float px = getPosition(F<AEGeometry *>(self, 0x8)).x;
        float py = getPosition(F<AEGeometry *>(self, 0x8)).y;
        float pz = getPosition(F<AEGeometry *>(self, 0x8)).z;
        int radiusInt = F<int>(F<Player *>(self, 0x4), 0x40);
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
