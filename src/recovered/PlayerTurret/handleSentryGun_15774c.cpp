#include "class.h"

extern "C" void PlayerTurret_pickEnemy(PlayerTurret *self);
extern "C" void PlayerTurret_handleRotation(PlayerTurret *self, int delta, AEGeometry *main, AEGeometry *turret);

struct PlayerTurret {
    void handleSentryGun(int delta);
};

void PlayerTurret::handleSentryGun(int delta)
{
    I(this, 0x130) = I(this, 0x130) + delta;
    PlayerTurret_pickEnemy(this);
    void *enemy = P(this, 0x14c);
    if (enemy != 0 && UC(enemy, 0x5e) == 0) {
        AEGeometry *geometry = TP<AEGeometry>(this, 0x8);
        PlayerTurret_handleRotation(this, delta, geometry, geometry);
    }
}
