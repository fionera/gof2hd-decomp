#include "class.h"

extern "C" void Player_setActive(int player, int on);
extern "C" void PlayerFighter_setExhaustVisible(PlayerFighter *self, bool vis);
extern "C" void PlayerFighter_awake_tail(int geom, int on);

extern "C" void PlayerFighter_awake(PlayerFighter *self)
{
    F<int32_t>(self, 0x88) = 1;
    Player_setActive(F<int32_t>(self, 0x4), 1);
    PlayerFighter_setExhaustVisible(self, true);
    int geom = F<int32_t>(self, 0xc);
    F<uint8_t>(self, 0xf5) = 1;
    if (geom == 0) {
        geom = F<int32_t>(self, 0x8);
    }
    return PlayerFighter_awake_tail(geom, 1);
}
