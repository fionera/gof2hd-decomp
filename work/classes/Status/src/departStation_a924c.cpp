#include "class.h"

struct Generator { Generator(); };
struct SolarSystem;
struct Galaxy;

__attribute__((visibility("hidden"))) extern Status **g_dsStatus;     // DAT_000b962c/9630
__attribute__((visibility("hidden"))) extern int g_dsTypeMask;        // DAT_000b9628
__attribute__((visibility("hidden"))) extern int *g_dsRandom;         // DAT_000b9634
__attribute__((visibility("hidden"))) extern Galaxy **g_dsGalaxy;     // DAT_000b9638

extern "C" {
int Status_inAlienOrbit_ds();                                        // 0x723d0
int Station_getIndex(Station *s);                                    // 0x71824
void *Station_getItems_ds();                                         // 0x71830
void Station_setItems(Station *s, void *items, bool owns);           // 0x736e4
int Station_getShips_ds();                                           // 0x73708
void Station_setShips(void *s, bool v);                              // 0x736fc
void Station_setAgents(Station *s, void *agents);                    // 0x73720
int isOnStack_ds(Status *self, Station *s);                          // 0x73660
void addStationToStack_ds(Status *self, Station *s);                 // 0x736c0
void Generator_ctor(Generator *g);                                   // 0x736cc
void *Generator_getItemBuyList(Generator *g, Station *s);            // 0x736d8
int Generator_getShipBuyList(Station *g);                            // 0x736f0
void *Generator_createAgents(Generator *g, Station *s);              // 0x73714
int Mission_getType_ds();                                            // 0x72874
int Mission_isCampaignMission_ds(Mission *m);                        // 0x7372c
int Mission_getProductionGoodIndex_ds(Mission *m);                   // 0x728a4
int Mission_isEmpty_ds();                                            // 0x72868
int Mission_getTargetStation_ds();                                   // 0x73738
int Ship_hasCargo_ds(int ship, int good);                            // 0x71a70
int AERandom_nextInt_ds(int rng, int bound);                         // 0x71848
void *Galaxy_getSystem_ds(Galaxy *g, int sysIdx);                    // 0x73684
int SolarSystem_getStations_ds(void *sys);                           // 0x71a88
void *Ship_getCargo_ds();                                            // 0x73750
void Item_setStationAmount_ds(void *item, int v);                    // 0x7375c
void Status_departTail(int ship);                                    // tail 0x1abf08
}

// Byte-offset helpers for fields that live in padding regions of the recovered Status struct.
static inline int &SF(Status *s, unsigned off) { return *(int *)((char *)s + off); }

// Status::departStation(Station* dest)   [self in r0, dest in r1]
//   Leaving the current station to travel to `dest`: refreshes inventories, stacks the prior
//   station, regenerates the destination's wares/ships/agents, then re-targets the active
//   campaign/freelance mission, moves wanted criminals, and clears per-cargo station amounts.
extern "C" void Status_departStation(Status *self, Station *dest)
{

    // refresh the just-departed station inventory if it is the campaign hub in state 3.
    if (Status_inAlienOrbit_ds() == 0) {
        bool hub = Station_getIndex(self->station) == 0x6c;
        if (hub && SF(self, 0x114) == 3) {
            Station *st = (Station *)(void *)(long)SF(self, 0x14c);
            Station_setItems(st, Station_getItems_ds(), true);
            Station_setShips((void *)(long)SF(self, 0x14c), (bool)Station_getShips_ds());
        }
    }

    bool wantedMove = false;
    if (Status_inAlienOrbit_ds() == 0 && self->playerStation != dest) {
        wantedMove = Station_getIndex(dest) != Station_getIndex(self->station);
    }

    if (self->playerStation != dest) {
        Station *prev = (Station *)(void *)(long)isOnStack_ds(self, dest);
        addStationToStack_ds(self, dest);
        bool hub = Station_getIndex(dest) == 0x6c;
        if (hub && SF(self, 0x114) == 3) {
            Station_setItems(dest, Station_getItems_ds(), true);
            Station_setShips((void *)dest, (bool)Station_getShips_ds());
            if (prev != 0 && prev != dest) {
                Station_setItems(prev, Station_getItems_ds(), true);
                Station_setShips((void *)prev, (bool)Station_getShips_ds());
            }
            if (prev == 0) {
                Generator g;
                Generator_ctor(&g);
                Station_setAgents(dest, Generator_createAgents(&g, dest));
            }
        } else if (prev == 0) {
            Generator g;
            Generator_ctor(&g);
            Station_setItems(dest, Generator_getItemBuyList(&g, dest), false);
            Station_setShips((void *)dest, (bool)Generator_getShipBuyList((Station *)&g));
            Station_setAgents(dest, Generator_createAgents(&g, dest));
        }
        SF(self, 0x70) = (int)self->playingTime;
        SF(self, 0x74) = (int)(self->playingTime >> 32);
    }

    // pick the active mission target for the new station.
    self->mission = (Mission *)*(void **)((char *)(*g_dsStatus));   // reset to default
    Status *st = *g_dsStatus;
    Array<Mission *> *missions = self->missions;
    for (unsigned i = 0; i < *(unsigned *)missions; i = i + 1) {
        Mission *mi = ((Mission **)(*(char **)((char *)missions + 4)))[i];
        if (mi == 0)
            continue;
        int type = Mission_getType_ds();
        int camp = Mission_isCampaignMission_ds(mi);

        if (type == 0xad && camp != 0) {
            int good = Mission_getProductionGoodIndex_ds(mi);
            if (Ship_hasCargo_ds((int)(long)st->ship, good) != 0) {
                self->mission = mi;
                break;
            }
        }
        if (st->currentCampaignMission < 0x2d && type == 0xa1 && camp != 0 &&
            Station_getIndex(dest) == SF(self, 0x80) && Status_inAlienOrbit_ds() == 0) {
            self->mission = mi;
            break;
        }
        if (Mission_isEmpty_ds() == 0) {
            int tgt = Mission_getTargetStation_ds();
            if (tgt == Station_getIndex(dest) &&
                ((unsigned)(type - 0x96) > 0x17 || ((1 << ((type - 0x96) & 0xff)) & g_dsTypeMask) == 0) &&
                type != 8 && type != 0xe) {
                if (camp != 0 || (type != 0xb && type != 0xd)) { self->mission = mi; break; }
            }
        }
        if (Mission_isEmpty_ds() == 0 && camp != 0) {
            if (type == 0xa0 && Mission_getTargetStation_ds() != Station_getIndex(dest)) {
                self->mission = mi;
                break;
            }
        }
    }

    // recompute the next freelance destination station.
    if (self->currentCampaignMission < 0x2d) {
        if (self->currentCampaignMission >= 0x20) {
            bool same = Station_getIndex(dest) == Mission_getTargetStation_ds() ||
                        Station_getIndex(dest) == SF(self, 0x80);
            if (!same) {
                if (SF(self, 0x8c) + 1 >= 10) {
                    SF(self, 0x8c) = 0;
                    int sys;
                    do {
                        do {
                            sys = AERandom_nextInt_ds(*g_dsRandom, 0x16);
                            SF(self, 0x7c) = sys;
                        } while (((char *)self->systemVisibilities->data)[sys] == 0);
                    } while (sys == 10 || sys == 0xf);
                    void *ss = Galaxy_getSystem_ds(*g_dsGalaxy, SF(self, 0x7c));
                    int stations = SolarSystem_getStations_ds(ss);
                    int n = AERandom_nextInt_ds(*g_dsRandom, SolarSystem_getStations_ds(ss));
                    SF(self, 0x80) = *(int *)(*(int *)(stations + 4) + n * 4);
                } else {
                    SF(self, 0x8c) = SF(self, 0x8c) + 1;
                }
            }
        }
    } else {
        SF(self, 0x7c) = -10;
        SF(self, 0x80) = -10;
    }

    if (wantedMove)
        self->moveWanted();

    // clear the per-cargo station-amount counters.
    unsigned *cargo = (unsigned *)Ship_getCargo_ds();
    if (cargo != 0) {
        for (unsigned i = 0; i < *cargo; i = i + 1) {
            void *it = *(void **)(cargo[1] + i * 4);
            if (it != 0)
                Item_setStationAmount_ds(it, 0);
        }
    }
    *(unsigned char *)((char *)self + 0x108) = 0;
    Status_departTail((int)(long)self->ship);
}
