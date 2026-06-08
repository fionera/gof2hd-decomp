#include "class.h"

// MenuTouchWindow::startSupernova() - sets up the "Supernova" pre-made savegame and jumps
// into the game. Same shape as startValkyrie but advances 0x54 missions and a richer loadout.

extern "C" void _mtw_Status_resetGame();
extern "C" void _mtw_Status_nextCampaignMission(bool a);
extern "C" void _mtw_Status_setMission(void *status);
extern "C" void *_mtw_Ship_makeShip(int shipDef);
extern "C" void _mtw_Status_setShip(void *status);
extern "C" void _mtw_Ship_setRace(void *ship, int race);
extern "C" void *_mtw_Status_getShip(void *status);       // pcVar14
extern "C" void *_mtw_Item_make(int itemDef);             // pcVar12
extern "C" void *_mtw_Item_makeQty(int itemDef, int qty); // pcVar13 (makeItem with count)
extern "C" void _mtw_Ship_setItem(void *ship, void *item, int slot); // pcVar11
extern "C" void _mtw_Ship_addCargo(void *ship, void *item);
extern "C" void _mtw_Status_setCredits(void *status);
extern "C" void *_mtw_Galaxy_getStation(void *galaxy, int idx);
extern "C" void _mtw_Status_setStation(void *status);
extern "C" void _mtw_Status_setSystemVisibility(void *status, int sys, bool vis);
extern "C" void _mtw_Achievements_setMedal(void *ach, int id, int n);
extern "C" void _mtw_RecordHandler_saveOptions(void *rh);
extern "C" void _mtw_Status_setKills(void *status);
extern "C" void _mtw_FModSound_stop(void *snd);

// PC-relative holders.
extern void *const gSnStatus    __attribute__((visibility("hidden")));
extern void *const gSnShipTable __attribute__((visibility("hidden")));
extern void *const gSnGalaxy    __attribute__((visibility("hidden")));
extern void *const gSnOptA      __attribute__((visibility("hidden")));
extern void *const gSnAch       __attribute__((visibility("hidden")));
extern void *const gSnOptB      __attribute__((visibility("hidden")));
extern void *const gSnOptHolder __attribute__((visibility("hidden")));
extern void *const gSnFmod      __attribute__((visibility("hidden")));
extern void *const gSnTransition __attribute__((visibility("hidden")));

typedef void (*TransitionFn)(void *app, int mode);

struct MenuTouchWindow { void startSupernova(); };
void MenuTouchWindow::startSupernova()
{
    void **statusHolder = (void **)gSnStatus;
    _mtw_Status_resetGame();
    for (int i = 0x54; i != 0; i--)
        _mtw_Status_nextCampaignMission((bool)(unsigned char)(long)*statusHolder);

    _mtw_Status_setMission(*statusHolder);
    void *status = *statusHolder;
    int *defs = (int *)(*(int *)(*(void **)gSnShipTable) + 4);
    _mtw_Ship_makeShip(*(int *)(defs[0] + 0x78));
    _mtw_Status_setShip(status);

    void *ship = *statusHolder;
    int race = (int)(long)_mtw_Status_getShip(ship);
    _mtw_Ship_setRace(ship, race);

    int *row = (int *)(*(int *)(*(void **)gSnShipTable) + 4);
    void *it; void *sh;
    it = _mtw_Item_make(*(int *)(row[0] + 0x2c0)); sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(*(int *)(row[0] + 0x50));  sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_makeQty(*(int *)(row[0] + 0x90), 0x14);  sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_makeQty(*(int *)(row[0] + 0xb0), 0x14);  sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_make(*(int *)(row[0] + 0x144)); sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(*(int *)(row[0] + 0xcc));  sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_make(*(int *)(row[0] + 0x110)); sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 2);
    it = _mtw_Item_make(*(int *)(row[0] + 0x158)); sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 3);
    it = _mtw_Item_make(*(int *)(row[0] + 0x154)); sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 4);
    it = _mtw_Item_make(*(int *)(row[0] + 0xe0));  sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 5);
    it = _mtw_Item_makeQty(*(int *)(row[0] + 0x1e8), 8); sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_addCargo(sh, it);

    _mtw_Status_setCredits(*statusHolder);
    void *station = *statusHolder;
    _mtw_Galaxy_getStation(*(void **)gSnGalaxy, 0x46);
    _mtw_Status_setStation(station);
    _mtw_Status_setSystemVisibility(*statusHolder, 6, true);
    _mtw_Status_setSystemVisibility(*statusHolder, 0x19, true);
    _mtw_Status_setCredits(*statusHolder);

    char *opt = (char *)*(void **)gSnOptA;
    char *optB = (char *)*(void **)gSnOptB;
    *(int *)(*(char **)statusHolder + 0x84) = 0x1a0a;
    *(int *)(opt + 0x20) = 0x1010101;
    opt[0x17] = 1;
    opt[0x36] = 0; *(int *)(opt + 0x32) = 0;
    *(unsigned short *)(opt + 8) = 0x101;
    opt[0xa] = 1; opt[0x15] = 1; opt[0x13] = 1;
    *(unsigned short *)(opt + 0xd) = 0x101;
    opt[0xf] = 1; opt[0x26] = 1; opt[0x31] = 1; opt[0x24] = 1;
    *(unsigned short *)(opt + 0x1c) = 0x101;
    opt[0x1e] = 1;
    *(unsigned short *)(opt + 0x38) = 0x101;
    void *ach = *(void **)gSnAch;
    optB[0x34] = 1;

    _mtw_Achievements_setMedal(ach, 0x17, 3);
    _mtw_Achievements_setMedal(*(void **)gSnAch, 0x1e, 1);
    _mtw_Status_setKills(*statusHolder);
    _mtw_RecordHandler_saveOptions(*(void **)gSnOptHolder);
    _mtw_FModSound_stop(*(void **)gSnFmod);

    void **app = (void **)gSnTransition;
    *(uint32_t *)(optB + 0x2c) = u32(this, 0x1a4);
    TransitionFn fn = *(TransitionFn *)((char *)app);
    fn(*app, 5);
}
