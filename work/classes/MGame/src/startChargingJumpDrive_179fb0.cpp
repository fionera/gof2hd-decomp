#include "class.h"

struct Status;
struct Ship;
struct ChoiceWindow;
struct PlayerEgo;
struct Hud;
__attribute__((visibility("hidden"))) extern Status **g_status;   // DAT_18a128
__attribute__((visibility("hidden"))) extern int *g_jumpFlag;     // DAT_18a134 (*piVar6)
__attribute__((visibility("hidden"))) extern int **g_alienAmt;    // DAT_18a138
__attribute__((visibility("hidden"))) extern int **g_jumpCost;    // DAT_18a130
__attribute__((visibility("hidden"))) extern int **g_alienCost;   // DAT_18a140

extern "C" Ship *Status_getShip(Status *s);
extern "C" int Ship_hasCargo(Ship *ship, int id, int n);
extern "C" int Status_hardCoreMode();
extern "C" int Status_inAlienOrbit();
extern "C" void *MGame_opnew(unsigned sz);
extern "C" void ChoiceWindow_ctor(ChoiceWindow *w);
extern "C" void *GameText_getText(int table, int id);
extern "C" void ChoiceWindow_set(ChoiceWindow *w, void *txt);
extern "C" void MGame_pauseSounds(MGame *self);
extern "C" void *Ship_getCargo(Ship *ship, int id);
extern "C" int Item_getAmount(void *item);
extern "C" void PlayerEgo_startJumpDrive(PlayerEgo *p);
extern "C" void Hud_hudEvent(Hud *h, int ev, PlayerEgo *p, int arg);
extern "C" void Ship_removeCargo(Ship *ship, int id, int n);

// MGame::startChargingJumpDrive(): if the jump drive is armed, validate fuel/cargo, then
// either pop a "no fuel" choice dialogue or actually trigger the hyperspace jump.
extern "C" void MGame_startChargingJumpDrive(MGame *self) {
    if (F<uint8_t>(self, 0xdd) == 0) return;
    Status **sp = g_status;
    int needed = 1;
    if (Ship_hasCargo(Status_getShip(*sp), 0x7a, 1) == 0) {
        ChoiceWindow *w = F<ChoiceWindow *>(self, 0x94);
        if (w == 0) {
            w = (ChoiceWindow *)MGame_opnew(0x5c);
            ChoiceWindow_ctor(w);
            F<ChoiceWindow *>(self, 0x94) = w;
        }
        void *txt = GameText_getText(**(int **)g_status, 0x243);
        ChoiceWindow_set(w, txt);
        F<uint8_t>(self, 0x5d) = 1;
        F<uint8_t>(self, 0xce) = 1;
        MGame_pauseSounds(self);
        **g_jumpCost = 0;
        return;
    }
    int hc = Status_hardCoreMode();
    int *jf = g_jumpFlag;
    if (hc != 0) needed = 2;
    int cost;
    if (*jf == F<int>((MGame *)*sp, 0x78)) {
        cost = needed << 1;
    } else {
        cost = **g_alienAmt;
        if (Status_inAlienOrbit() != 0) cost = needed;
    }
    Ship_getCargo(Status_getShip(*sp), 0x7a);
    if (Item_getAmount(0) < cost) {
        ChoiceWindow *w = F<ChoiceWindow *>(self, 0x94);
        if (w == 0) {
            w = (ChoiceWindow *)MGame_opnew(0x5c);
            ChoiceWindow_ctor(w);
            F<ChoiceWindow *>(self, 0x94) = w;
        }
        int hc2 = Status_hardCoreMode();
        void *txt = GameText_getText(**(int **)g_status, hc2 != 0 ? 0x243 : 0x244);
        ChoiceWindow_set(w, txt);
        F<uint8_t>(self, 0x5d) = 1;
        F<uint8_t>(self, 0xce) = 1;
        MGame_pauseSounds(self);
        *jf = 0;
        return;
    }
    PlayerEgo_startJumpDrive(F<PlayerEgo *>(self, 0x58));
    if (*jf != F<int>((MGame *)*sp, 0x78)) {
        if (Status_inAlienOrbit() == 0) needed = **g_alienCost;
    }
    Hud_hudEvent(F<Hud *>(self, 0x74), 0x1e, F<PlayerEgo *>(self, 0x58), needed);
    Ship_removeCargo(Status_getShip(*sp), 0x7a, needed);
}
