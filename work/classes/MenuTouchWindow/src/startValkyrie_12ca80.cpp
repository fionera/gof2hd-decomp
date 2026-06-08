#include "class.h"

// MenuTouchWindow::startValkyrie() - sets up the "Valkyrie" pre-made savegame and jumps
// into the game. Walks 0x2d campaign missions forward, builds a ship with fixed loadout,
// flips a set of option flags, awards medals, then transitions to the game module.

extern "C" void _mtw_Status_resetGame();
extern "C" void _mtw_Status_nextCampaignMission(bool a);
extern "C" void *operator_new(uint32_t);
extern "C" void _mtw_Mission_ctor(void *m);
extern "C" void _mtw_Status_setMission(void *status);
extern "C" void *_mtw_Ship_makeShip(int shipDef);
extern "C" void _mtw_Status_setShip(void *status);
extern "C" void _mtw_Ship_setRace(void *ship, int race);
extern "C" void *_mtw_Item_makeItem(int itemDef, int qty);
extern "C" void *_mtw_makeItem2(int itemDef);
extern "C" void *_mtw_Status_getShip(void *status);       // pcVar13: returns ship from status
extern "C" void *_mtw_Item_make(int itemDef);             // pcVar12: makeItem(def)
extern "C" void _mtw_Ship_setItem(void *ship, void *item, int slot); // pcVar9
extern "C" void _mtw_Status_setCredits(void *status);
extern "C" void *_mtw_Galaxy_getStation(void *galaxy, int idx);
extern "C" void _mtw_Status_setStation(void *status);
extern "C" void _mtw_Status_setSystemVisibility(void *status, int sys, bool vis);
extern "C" void _mtw_Achievements_setMedal(void *ach, int id, int n);
extern "C" void _mtw_RecordHandler_saveOptions(void *rh);
extern "C" void _mtw_Status_setKills(void *status);
extern "C" void _mtw_FModSound_stop(void *snd);

// PC-relative holders.
extern void *const gValStatus    __attribute__((visibility("hidden"))); // *holder -> Status object
extern void *const gValShipTable __attribute__((visibility("hidden"))); // *holder -> def table (.+4 -> defs)
extern void *const gValGalaxy    __attribute__((visibility("hidden")));
extern void *const gValOptA      __attribute__((visibility("hidden"))); // option flags object
extern void *const gValAch       __attribute__((visibility("hidden"))); // Achievements holder
extern void *const gValOptB      __attribute__((visibility("hidden"))); // second flags object (+0x34,+0x2c)
extern void *const gValOptHolder __attribute__((visibility("hidden")));
extern void *const gValFmod      __attribute__((visibility("hidden")));
extern void *const gValTransition __attribute__((visibility("hidden")));

typedef void (*TransitionFn)(void *app, int mode);

struct MenuTouchWindow { void startValkyrie(); };
void MenuTouchWindow::startValkyrie()
{
    void **statusHolder = (void **)gValStatus;
    _mtw_Status_resetGame();
    for (int i = 0x2d; i != 0; i--)
        _mtw_Status_nextCampaignMission((bool)(unsigned char)(long)*statusHolder);

    void *status = *statusHolder;
    void *mission = operator_new(0x78);
    _mtw_Mission_ctor(mission);
    _mtw_Status_setMission(status);

    int *defs = (int *)(*(int *)(*(void **)gValShipTable) + 4);
    status = *statusHolder;
    _mtw_Ship_makeShip(*(int *)(defs[0] + 0x14)); // defs is table; [4]->row; +0x14
    _mtw_Status_setShip(status);

    void *ship = *statusHolder;
    int race = (int)(long)_mtw_Status_getShip(ship);
    _mtw_Ship_setRace(ship, race);

    int *row = (int *)(*(int *)(*(void **)gValShipTable) + 4);
    void *it; void *sh;
    it = _mtw_Item_make(*(int *)(row[0] + 8));   sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(*(int *)(row[0] + 0x14));sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_make(*(int *)(row[0] + 0x90));sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(*(int *)(row[0] + 0x144));sh = _mtw_Status_getShip(*statusHolder);_mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(*(int *)(row[0] + 0xcc)); sh = _mtw_Status_getShip(*statusHolder);_mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_make(*(int *)(row[0] + 0x158));sh = _mtw_Status_getShip(*statusHolder);_mtw_Ship_setItem(sh, it, 2);
    it = _mtw_Item_make(*(int *)(row[0] + 0x154));sh = _mtw_Status_getShip(*statusHolder);_mtw_Ship_setItem(sh, it, 3);

    _mtw_Status_setCredits(*statusHolder);
    void *station = *statusHolder;
    _mtw_Galaxy_getStation(*(void **)gValGalaxy, 0x5b);
    _mtw_Status_setStation(station);
    _mtw_Status_setSystemVisibility(*statusHolder, 6, true);
    _mtw_Status_setSystemVisibility(*statusHolder, 0x19, true);
    _mtw_Status_setCredits(*statusHolder);

    char *opt = (char *)*(void **)gValOptA;
    void *ach = *(void **)gValAch;
    char *optB = (char *)*(void **)gValOptB;
    *(int *)(*(char **)statusHolder + 0x84) = 0x1a0a;
    *(unsigned short *)(opt + 8) = 0x101;
    *(unsigned short *)(opt + 0xd) = 0x101;
    *(unsigned short *)(opt + 0x1c) = 0x101;
    *(int *)(opt + 0x20) = 0x1010101;
    opt[0x17] = 1; opt[0xa] = 1; opt[0x15] = 1; opt[0x13] = 1; opt[0xf] = 1;
    opt[0x1e] = 1; opt[0x24] = 1; opt[0x36] = 0; *(int *)(opt + 0x32) = 0; opt[0x38] = 1;
    optB[0x34] = 1;

    _mtw_Achievements_setMedal(ach, 0x17, 3);
    _mtw_Achievements_setMedal(*(void **)gValAch, 0x1e, 1);
    _mtw_RecordHandler_saveOptions(*(void **)gValOptHolder);
    _mtw_Status_setKills(*statusHolder);
    _mtw_FModSound_stop(*(void **)gValFmod);

    void **app = (void **)gValTransition;
    *(uint32_t *)(optB + 0x2c) = u32(this, 0x1a4);
    TransitionFn fn = *(TransitionFn *)((char *)app); // DAT_001ab904 thunk
    fn(*app, 5);
}
