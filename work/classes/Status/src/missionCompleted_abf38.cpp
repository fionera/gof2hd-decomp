#include "class.h"

// Singleton Status holders referenced via PC-relative slots.
__attribute__((visibility("hidden"))) extern Status **g_mcStatusA;   // DAT_000bc4c4 etc.
__attribute__((visibility("hidden"))) extern Status **g_mcStatusB;

extern "C" {
int Mission_hasWon(Mission *m);
int Mission_isCampaignMission(Mission *m);
int Mission_getAgent(Mission *m);
int Mission_getClientImage(Mission *m);
int Mission_getTargetStation();
int Mission_getType();
int Mission_getStatusValue();
int Mission_getProductionGoodIndex(Mission *m);
int Mission_getProductionGoodAmount();
void Mission_setWon(Mission *m, bool v);
int Station_getIndex(Station *s);
void *Ship_getEquipment(Ship *ship);
void *Ship_getCargo();
int Ship_getCurrentLoad();
int Ship_hasCargo(int ship, int item);
int Ship_hasEquipment(int ship, int item);
int Item_getIndex(void *item);
int Item_getType(void *item);
int Item_getSort(void *item);
int Item_isInList(int a, int b, void *list);
int Item_isInList2(int a, void *list);
void *BluePrint_getIngredientList(void *bp);
void BluePrint_getQuantityList(void *bp);
int Agent_getStation(Agent *a);
int Status_inAlienOrbit3();
}

// Status::missionCompleted(bool atStation, bool docked, long long extra)
//   Scans the active mission list; for the first unfinished mission whose completion criteria
//   are met it returns that Mission (some types auto-mark themselves won). Mission-type-specific
//   checks mirror the engine's giant dispatch.
extern "C" Mission *Status_missionCompleted(Status *self, bool atStation, bool docked,
                                            long long extra)
{
    unsigned which = (unsigned)docked;
    unsigned both = which & (unsigned)extra;

    Array<Mission *> *missions = self->missions;
    for (unsigned i = 0; i < *(unsigned *)missions; i = i + 1) {
        Mission *m = ((Mission **)(*(char **)((char *)missions + 4)))[i];
        if (Mission_hasWon(m) != 0)
            return (Mission *)0;

        // skip irrelevant missions (no agent/target/campaign tie-in).
        if (m == 0 ||
            (Mission_isCampaignMission(m) == 0 && Mission_getAgent(m) == 0 &&
             Mission_getClientImage(m) == 0 && Mission_getTargetStation() == 0 &&
             Mission_getTargetStation() == 0))
            continue;

        int type = Mission_getType();
        switch (type) {
        case 0x96:
            if (Mission_getStatusValue() <= self->missionCount) { Mission_setWon(m, true); return m; }
            break;
        case 0x97:
            if (Mission_getStatusValue() <= self->kills) { Mission_setWon(m, true); return m; }
            break;
        case 0x99:
            if (Mission_getStatusValue() <= self->stationsVisited) { Mission_setWon(m, true); return m; }
            break;
        case 0x9b:
            if (Mission_getStatusValue() <= self->goodsProduced) { Mission_setWon(m, true); return m; }
            break;
        case 0x98: {
            unsigned *eq = (unsigned *)Ship_getEquipment(self->ship);
            for (unsigned j = 0; j < *eq; j = j + 1) {
                void *it = *(void **)(eq[1] + j * 4);
                if (it != 0 && Item_getIndex(it) == Mission_getStatusValue())
                    return m;
            }
            break;
        }
        case 0x9a: {
            if (Mission_getProductionGoodAmount() <= Ship_getCurrentLoad()) { Mission_setWon(m, true); return m; }
            break;
        }
        case 0xa7:
        case 0xae:
            if (Mission_getProductionGoodAmount() <= Mission_getStatusValue()) { Mission_setWon(m, true); return m; }
            break;
        case 0x9c:
            if (!docked) {
                int here = Station_getIndex(self->station);
                int target = Mission_getTargetStation();
                if (extra >= 0x2711 && here == target) return m;
            }
            break;
        case 0xa0:
            if (docked) return m;
            {
                int here = Station_getIndex(self->station);
                int target = Mission_getTargetStation();
                if (extra < 0x2711 && here != target) return m;
            }
            break;
        case 0x9d: {
            unsigned *eq = (unsigned *)Ship_getEquipment(self->ship);
            for (unsigned j = 0; j < *eq; j = j + 1) {
                void *it = *(void **)(eq[1] + j * 4);
                if (it != 0 && Item_getType(it) == Mission_getStatusValue())
                    return m;
            }
            break;
        }
        case 0x9e: {
            unsigned *eq = (unsigned *)Ship_getEquipment(self->ship);
            bool hasGood = false, hasSpecial = false;
            for (unsigned j = 0; j < *eq; j = j + 1) {
                void *it = *(void **)(eq[1] + j * 4);
                if (it != 0) {
                    if (Item_getType(it) == 0)
                        hasGood = true;
                    else if (Item_getSort(it) == 10)
                        hasSpecial = true;
                }
            }
            if (hasGood && hasSpecial) return m;
            break;
        }
        case 0xa2:
            if (docked) {
                if (Station_getIndex(self->station) == Mission_getTargetStation()) {
                    void *bp = *(void **)(*(char **)((char *)self + 0x18) + Mission_getStatusValue() * 4);
                    unsigned *ing = (unsigned *)BluePrint_getIngredientList(bp);
                    BluePrint_getQuantityList(bp);
                    bool all = true;
                    for (unsigned j = 0; j < *ing; j = j + 1) {
                        if (Ship_hasCargo((int)(long)self->ship, *(int *)(ing[1] + j * 4)) == 0) { all = false; break; }
                    }
                    if (all) return m;
                }
            }
            break;
        case 0xa3: {
            unsigned *arr = (unsigned *)self->field_90;
            bool all = true;
            for (unsigned j = 0; j < *arr; j = j + 1) {
                if (*(int *)(arr[1] + j * 4) < 0) { all = false; break; }
            }
            if (all) return m;
            break;
        }
        case 0xa8: {
            Status *s = *g_mcStatusA;
            if (Mission_getStatusValue() <= s->field_178) {
                Mission_setWon(m, true);
                s->field_178 = 0;
                return m;
            }
            break;
        }
        case 0xaa:
            if (Mission_getStatusValue() == 1) { Mission_setWon(m, true); return m; }
            break;
        case 0xab:
            if (docked) {
                if (Station_getIndex(self->station) == Mission_getTargetStation()) return m;
            }
            break;
        case 0xac:
            if (both != 0) {
                if (Station_getIndex(self->station) == Mission_getTargetStation()) {
                    int idx = Mission_getProductionGoodIndex(m);
                    int amt = Mission_getProductionGoodAmount();
                    if (Item_isInList(idx, amt, Ship_getCargo()) != 0) return m;
                }
            }
            break;
        case 0xa6:
            if (docked) {
                if (Station_getIndex(self->station) == Mission_getTargetStation()) {
                    int idx = Mission_getProductionGoodIndex(m);
                    int amt = Mission_getProductionGoodAmount();
                    if (Item_isInList(idx, amt, Ship_getCargo()) != 0) return m;
                    if (Ship_hasEquipment((int)(long)((*g_mcStatusA)->ship),
                                          Mission_getProductionGoodIndex(m)) == 0) return m;
                }
            }
            break;
        case 0xb8:
            if (Mission_getStatusValue() == 0) {
                Status *s = *g_mcStatusB;
                if (s->currentCampaignMission != 0x5c || Status_inAlienOrbit3() != 0 ||
                    Station_getIndex(s->station) != 0x71) {
                    Mission_setWon(m, true);
                    return m;
                }
            }
            break;
        case 0xbd:
            if (docked) {
                unsigned *eq = (unsigned *)Ship_getEquipment(self->ship);
                for (unsigned j = 0; j < *eq; j = j + 1) {
                    void *it = *(void **)(eq[1] + j * 4);
                    if (it != 0 && Item_getSort(it) == Mission_getStatusValue())
                        return m;
                }
            }
            break;
        default:
            // legacy mission ids 8..0xe.
            if (type == 8) {
                if (docked && Station_getIndex(self->station) == Mission_getTargetStation()) {
                    int idx = Mission_getProductionGoodIndex(m);
                    int amt = Mission_getProductionGoodAmount();
                    if (Item_isInList(idx, amt, Ship_getCargo()) != 0) return m;
                }
            } else if (type == 0xd) {
                if (docked && *(unsigned char *)((char *)self + 0xf0) != 0) return m;
            } else if (type == 0xe) {
                if (docked) {
                    Agent *ag = (Agent *)Mission_getAgent(m);
                    if (Station_getIndex(self->station) == Agent_getStation(ag)) {
                        if (Item_isInList2(0x73, Ship_getCargo()) != 0) return m;
                    }
                }
            } else if (type == 0xb || type == 0) {
                if (docked && Station_getIndex(self->station) == Mission_getTargetStation()) return m;
            }
            break;
        }
    }
    return (Mission *)0;
}
