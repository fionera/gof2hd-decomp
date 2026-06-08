#include "class.h"

struct MissionObj;
__attribute__((visibility("hidden"))) extern Status **g_ncmStatus;
__attribute__((visibility("hidden"))) extern int g_ncmAchTable[3];   // DAT_000b9d14 (3 mission ids)

extern "C" {
void *operator_new(unsigned int sz);
void Mission_ctor3(void *m, int type, int param, int station);       // Mission::Mission(int,int,int)
void Mission_ctor_default(void *m);                                  // Mission::Mission()
void Status_setCampaignMission_ncm(Status *self, void *m);           // setCampaignMission
void Status_addMissionTail(Status *self, void *m);                   // indirect tail 0x1abf38
void Ship_setCargo_ncm(Ship *ship, void *arr);                       // Ship::setCargo
void Ship_removeAllCargo_ncm(Ship *ship);                            // Ship::removeAllCargo
void *Ship_getEquipment_ncm(Ship *ship);                            // Ship::getEquipment
void *Ship_getCargo_ncm();                                          // Ship::getCargo
void Item_setUnsaleable_ncm(void *item, int v);
int  Item_getSinglePrice_ncm(void *item);
void Item_setPrice_ncm(void *item, int price);
void *Ship_getFirstEquipmentOfSort_ncm(int ship);
void Ship_removeEquipment_ncm(Ship *ship, void *item);
void Ship_addEquipment_ncm(Ship *ship, void *item);
void *Item_makeItem_ncm(int infoPtr);
}

// Per-campaign-step mission descriptor (the bulk of the switch is "build Mission(type,param,
// station) and register it" -- either as the campaign mission or via the add-mission tail).
struct Step { short type; int param; short station; unsigned char campaign; };

// Table indexed by campaign step (recovered from the engine's switch). Steps with extra side
// effects are handled specially below; the rest follow this descriptor.
static const Step kSteps[] = {
    /*0x00*/ {0, 0, 0, 0},
    /*0x01*/ {0x9a, 0, 0x4e, 1}, /*0x02*/ {0xb, 0, 0x4e, 0}, /*0x03*/ {0x9a, 0, 0x4e, 1},
    /*0x04*/ {0xb, 0, 0x4e, 0},  /*0x05*/ {0x9e, 0, 0x4e, 0}, /*0x06*/ {0x4, 0, 0x4e, 0},
    /*0x07*/ {0xb, 0, 0x4e, 0},  /*0x08*/ {0xb, 0, 0x4e, 0},  /*0x09*/ {0xb, 0, 0x4f, 0},
    /*0x0a*/ {0xb, 0, 0x4c, 0},  /*0x0b*/ {0xb, 0, 0x4f, 0},  /*0x0c*/ {0x96, 0, 0, 1},
    /*0x0d*/ {0x4, 0, 0x4f, 0},  /*0x0e*/ {0xb, 0, 0x62, 0},  /*0x0f*/ {0x4, 0, 0x62, 0},
    /*0x10*/ {0xb, 0, 0x62, 0},  /*0x11*/ {0x9c, 0, 0x38, 0}, /*0x12*/ {0x9c, 0, 0x37, 0},
    /*0x13*/ {0xbd, 0, 0x37, 1}, /*0x14*/ {0x4, 0, 0x37, 0},  /*0x15*/ {0xb, 0, 0x37, 0},
    /*0x16*/ {0xb, 20000, 10, 0},/*0x17*/ {0x4, 0, 0x30, 0},  /*0x18*/ {0x4, 0, 0x30, 0},
    /*0x19*/ {0x4, 0, 0x30, 0},  /*0x1a*/ {0xb, 0, 10, 0},    /*0x1b*/ {0x4, 0, 0x5b, 0},
    /*0x1c*/ {0x4, 0, -1, 0},    /*0x1d*/ {0x9c, 0, 0x5b, 0}, /*0x1e*/ {0xb, 30000, 0x62, 0},
    /*0x1f*/ {0xb, 0, 10, 0},    /*0x20*/ {0x8, 0, 10, 1},    /*0x21*/ {0xb, 0, 0x1d, 0},
    /*0x22*/ {0xb, 0, 0x1d, 0},  /*0x23*/ {0xc, 0, 0x1b, 0},  /*0x24*/ {0xa0, 0, 0x1b, 0},
    /*0x25*/ {0x4, 0, 0x16, 0},  /*0x26*/ {0xb, 0, 0x1e, 0},  /*0x27*/ {0xa1, 0, -1, 0},
    /*0x28*/ {0x4, 0, -1, 0},    /*0x29*/ {0xa0, 0, -1, 0},   /*0x2a*/ {0xb, 0, 10, 0},
    /*0x2b*/ {0xb, 0, 10, 0},    /*0x2c*/ {0, 0, 0, 1},       /*0x2d*/ {0xb, 0, 0x4a, 0},
    /*0x2e*/ {0xb, 0, 0x3a, 0},  /*0x2f*/ {0xb, 0, 0x3a, 0},  /*0x30*/ {0x9c, 0, 0x3a, 0},
    /*0x31*/ {0x9c, 0, 0x3e, 0}, /*0x32*/ {0x9c, 0, 0x19, 0}, /*0x33*/ {0xa0, 0, 0x19, 0},
    /*0x34*/ {0, 0, 0, 0},       /*0x35*/ {0xb, 0, 0x4a, 0},  /*0x36*/ {0xb, 0, 0x65, 1},
    /*0x37*/ {0x4, 0, 0x66, 0},  /*0x38*/ {0xb, 0, 0x65, 0},  /*0x39*/ {0xa6, 0, 0x65, 1},
    /*0x3a*/ {0xa3, 0, 0x65, 1}, /*0x3b*/ {0xb, 50000, 0x65, 1}, /*0x3c*/ {0xa4, 0, 0x65, 0},
    /*0x3d*/ {0xb, 0, 100, 0},   /*0x3e*/ {0x4, 0, 0x67, 0},  /*0x3f*/ {0x4, 0, 0x68, 0},
    /*0x40*/ {0xb, 0, 100, 0},   /*0x41*/ {0xb, 0, 0x65, 0},  /*0x42*/ {0x4, 0, 0x68, 1},
    /*0x43*/ {0x8, 0, 0x42, 1},  /*0x44*/ {0x6, 0, 0x42, 0},  /*0x45*/ {0x6, 0, 0x41, 0},
    /*0x46*/ {0xb, 0, 0x42, 0},  /*0x47*/ {0xa4, 0, 0x65, 1}, /*0x48*/ {0xa, 0, 0x51, 0},
    /*0x49*/ {0xb, 0, 100, 0},   /*0x4a*/ {0xb, 0, 100, 0},   /*0x4b*/ {0xb, 0, 100, 0},
    /*0x4c*/ {0xb, 0, 0x65, 0},  /*0x4d*/ {0x4, 0, 0x65, 0},  /*0x4e*/ {0xa5, 0, -1, 0},
    /*0x4f*/ {0x1, 0, 100, 0},   /*0x50*/ {0x4, 0, -1, 0},    /*0x51*/ {0xb, 0, 100, 0},
    /*0x52*/ {0xb, 0, 100, 0},   /*0x53*/ {0, 0, 0, 0},       /*0x54*/ {0xa4, 0, 0, 0},
    /*0x55*/ {0xb, 0, 100, 0},   /*0x56*/ {0x4, 0, 10, 0},    /*0x57*/ {0xb, 0, 10, 0},
    /*0x58*/ {0x4, 0, 0x6d, 0},
};

// Status::nextCampaignMission(bool param_1)
//   Advances the campaign by one step: bumps the campaign-mission counter, snapshots the
//   playing-time, runs the achievement check, then spawns and registers the next scripted
//   Mission for that step (with a few steps performing extra cargo/equipment bookkeeping).
extern "C" void Status_nextCampaignMission(Status *self, bool param_1)
{
    (void)param_1;
    int *state = (int *)((char *)self + 0x1e8);
    int prevTimeLo = (int)self->playingTime;
    int prevTimeHi = (int)(self->playingTime >> 32);

    int step = *state;
    int next = step + 1;

    // achievement check: flag if the new step matches any of the three tracked mission ids.
    for (int k = 0; k < 3; k = k + 1) {
        if (next == g_ncmAchTable[k])
            *(unsigned char *)((char *)self + 0x17c) = 1;
    }

    // advance counter + snapshot time (cases that finalise via campaign use 0x1e8, others 0x1e8
    // too -- both write the same counter in the engine).
    *state = next;
    *(int *)((char *)self + 0x100) = prevTimeLo;
    *(int *)((char *)self + 0x104) = prevTimeHi;

    if (step == 0)
        return;   // step 0 has no mission.

    // --- steps with extra side effects ------------------------------------
    if (step == 3) {
        Ship_setCargo_ncm(self->ship, 0);
    } else if (step == 5) {
        Ship_removeAllCargo_ncm(self->ship);
    } else if (step == 7) {
        unsigned *eq = (unsigned *)Ship_getEquipment_ncm(self->ship);
        if (eq != 0)
            for (unsigned i = 0; i < *eq; i = i + 1) {
                void *it = *(void **)(eq[1] + i * 4);
                if (it != 0) { Item_setUnsaleable_ncm(it, 0); Item_setPrice_ncm(it, Item_getSinglePrice_ncm(it)); }
            }
        unsigned *cg = (unsigned *)Ship_getCargo_ncm();
        if (cg != 0)
            for (unsigned i = 0; i < *cg; i = i + 1) {
                void *it = *(void **)(cg[1] + i * 4);
                if (it != 0) { Item_setUnsaleable_ncm(it, 0); Item_setPrice_ncm(it, Item_getSinglePrice_ncm(it)); }
            }
    } else if (step == 9) {
        void *old = Ship_getFirstEquipmentOfSort_ncm((int)(long)self->ship);
        Ship_removeEquipment_ncm(self->ship, old);
        Ship_addEquipment_ncm(self->ship, Item_makeItem_ncm(0x158));
    }

    // --- spawn + register the step's mission -------------------------------
    void *m = operator_new(0x78);
    if ((unsigned)step < sizeof(kSteps) / sizeof(kSteps[0])) {
        const Step &s = kSteps[step];
        if (s.type == 0 && s.station == 0 && s.param == 0) {
            Mission_ctor_default(m);
        } else {
            Mission_ctor3(m, s.type, s.param, s.station);
        }
        if (s.campaign)
            Status_setCampaignMission_ncm(self, m);
        else
            Status_addMissionTail(self, m);
    } else {
        // beyond the scripted range: default freelance hop.
        Mission_ctor3(m, 0xb, 0, 100);
        Status_addMissionTail(self, m);
    }
}
