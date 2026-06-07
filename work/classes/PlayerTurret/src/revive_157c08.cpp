#include "class.h"

extern "C" void Player_reset(Player *self);
extern "C" void KIPlayer_setActive(PlayerTurret *self, bool active);
extern "C" void Explosion_reset(Explosion *self);
extern "C" void AEGeometry_setVisible(void *self, bool visible);

struct PlayerTurret {
    void revive();
};

void PlayerTurret::revive()
{
    int zero = 0;
    int one = 1;
    Player_reset(TP<Player>(this, 0x4));
    *(volatile int *)B(this, 0x78) = zero;
    *(volatile int *)B(this, 0x88) = one;
    *(volatile int *)B(this, 0xd8) = zero;
    KIPlayer_setActive(this, true);
    Explosion_reset(TP<Explosion>(this, 0x13c));
    void *geometry = P(this, 0xc);
    *(volatile int *)B(this, 0x128) = zero;
    *(volatile uint8_t *)B(this, 0xf5) = one;
    if (geometry == 0) {
        geometry = P(this, 0x8);
    }
    AEGeometry_setVisible(geometry, true);
}
