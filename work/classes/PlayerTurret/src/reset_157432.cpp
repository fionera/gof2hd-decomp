#include "class.h"

extern "C" void KIPlayer_reset(PlayerTurret *self);
extern "C" void KIPlayer_setActive(PlayerTurret *self, bool active);

struct PlayerTurret {
    void reset();
};

void PlayerTurret::reset()
{
    KIPlayer_reset(this);
    I(this, 0x88) = 0;
    KIPlayer_setActive(this, true);
}
