#include "class.h"

struct KIPlayer;
extern "C" void KIPlayer_setWingmanCommand(PlayerFighter *self, int cmd, KIPlayer *target);
extern "C" int Level_getPlayerRoute(int level);
extern "C" int Route_getExactClone(int route);
extern "C" int Route_getCurrent(int route);

extern "C" void PlayerFighter_setWingmanCommand(PlayerFighter *self, int cmd, KIPlayer *target)
{
    int saved = F<int32_t>(self, 0xe4);
    KIPlayer_setWingmanCommand(self, cmd, target);
    if ((unsigned)(cmd - 2) < 2) {
        F<int32_t>(self, 0x1b8) = 0x1389;
        if (F<float>(self, 0x1e8) != 5.5f) {
            F<int32_t>(self, 0x1c0) = 0x1389;
        }
        if (cmd == 2) {
            if (Level_getPlayerRoute(F<int32_t>(self, 0x54)) != 0) {
                int r = Level_getPlayerRoute(F<int32_t>(self, 0x54));
                F<int32_t>(self, 0x14c) = Route_getExactClone(r);
                F<int32_t>(self, 0x1e4) = Route_getCurrent(F<int32_t>(self, 0x14c));
                goto done;
            }
        } else if (target != 0) {
            goto done;
        }
        F<int32_t>(self, 0xe4) = saved;
    } else if (cmd == 0) {
        F<int32_t>(self, 0xe4) = saved;
        F<uint32_t>(self, 0x140) = (F<int32_t>(self, 0x140) == 0);
    } else if (cmd == 1) {
        F<uint8_t>(self, 0x13d) = 0;
        F<uint8_t>(self, 0x1e0) = 0;
    }
done:
    F<uint32_t>(self, 0x1e8) = F<uint32_t>(self, 0x1ac);
    F<uint32_t>(self, 0x1f0) = F<uint32_t>(self, 0x1a4);
}
