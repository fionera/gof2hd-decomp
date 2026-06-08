#include "class.h"

// PlayerEgo::approachDockingPoint(Hud*, int hud2, Radar*)
//   Three-phase automated station-docking controller, dispatched on the dock
//   state (0x314):
//     * state 0: locate the nearest navigation point on the station, reserve it
//       (SpacePoint take/give), fly toward it, and -- on arrival -- build the
//       glide EaseInOutMatrix and advance to state 2.
//     * state 2: when within the per-ship docking radius, inspect the active and
//       campaign missions to decide which "arrived" HUD event to raise (cargo
//       delivery 0x29/0x25/0x23, passenger drop-off 0x2b, etc.), then hand back
//       to manual control (state 1). Otherwise keep gliding the ease matrix.
//     * state 3: glide the final docking ease; once within 200 units complete
//       the dock -- restore cameras, free the nav point, tear down any hacking
//       game -- and return to state 0.
//   The dense vector/matrix glide math is delegated to PE_adp_glide / PE_adp_apply;
//   the recoverable state dispatch and the mission-event decision stay inline.

extern "C" int   KIPlayer_isDying(void *self);
extern "C" void  PlayerEgo_getPosition(PlayerEgo *self, void *out);
extern "C" void *KIPlayer_getNearestNavigationPoint(void *station, void *posOut, void *spOut);
extern "C" void  SpacePoint_take(void *sp);
extern "C" void  SpacePoint_giveFree(void *sp);
extern "C" void  PlayerEgo_setTurretMode(PlayerEgo *self, int on);
extern "C" void *Status_getShip();
extern "C" int   Ship_getIndex(void *ship);
extern "C" int   Ship_hasCargo(void *ship, int idx);
extern "C" void *Ship_getCargo(void *ship, int idx);
extern "C" int   Ship_getMaxPassengers(void *ship);
extern "C" int   Item_getAmount(void *item);
extern "C" void *Status_getMission();
extern "C" void *Status_getCampaignMission();
extern "C" int   Mission_isEmpty(void *m);
extern "C" int   Mission_getType(void *m);
extern "C" int   Mission_getProductionGoodIndex(void *m);
extern "C" int   Mission_getProductionGoodAmount(void *m);
extern "C" int   Mission_getStatusValue(void *m);
extern "C" int   PlayerFixedObject_getDockingType(void *o);
extern "C" int   Level_getNumDeliveredOre(void *level);
extern "C" void  Hud_hudEvent(void *hud, int ev, void *arg);
extern "C" void  TargetFollowCamera_setActive(void *cam, bool active);
extern "C" void  TargetFollowCamera_setLookAtCam(void *cam, bool on);
extern "C" void  TargetFollowCamera_useTargetsUpVector(void *cam, bool on);
extern "C" void  LevelScript_resetCamera(void *level);
extern "C" void  Player_resetGunDelay(void *player, int idx);
extern "C" void *HackingGame_dtor(void *hg);
extern "C" void  operator_delete_(void *p);
extern "C" void  Hud_setHackingGameActive(void *hud, bool active);
// Glide helpers: PE_adp_approach steers/builds the ease toward the nav point and
// returns the remaining distance; PE_adp_glide advances the ease one step and
// returns the remaining distance; PE_adp_apply writes the final transform.
extern "C" int   PE_adp_approach(PlayerEgo *self, void *station);
extern "C" int   PE_adp_glide(PlayerEgo *self);
extern "C" void  PE_adp_apply(PlayerEgo *self);

__attribute__((visibility("hidden"))) extern int  *g_PE_adp_dockRadius; // 0xb00f2 per-ship table
__attribute__((visibility("hidden"))) extern void **g_PE_adp_fixedObj;  // 0xb00f0 PlayerFixedObject

// clears the 3-float dock-offset vector at 0x1c8.
static inline void adp_clearDockVector(PlayerEgo *self)
{
    I(self, 0x1c8) = 0;
    I(self, 0x1cc) = 0;
    I(self, 0x1d0) = 0;
}

// Decide the arrival HUD event for state 2; returns the event id (0 = none).
static int adp_arrivalEvent(PlayerEgo *self, void *station)
{
    void *mission = Status_getMission();
    void *campaign = Status_getCampaignMission();
    void *fixed = *g_PE_adp_fixedObj;

    if (Mission_isEmpty(mission) == 0 && Mission_getType(mission) == 0xf
        && PlayerFixedObject_getDockingType(fixed) == 1) {
        if (Ship_hasCargo(Status_getShip(), Mission_getProductionGoodIndex(mission)) != 0) {
            int amount = Item_getAmount(Ship_getCargo(Status_getShip(),
                                        Mission_getProductionGoodIndex(mission)));
            I(self, 0x180) = amount;
            int need = Mission_getProductionGoodAmount(mission)
                       - Level_getNumDeliveredOre(P(self, 0xc));
            if (need < amount) I(self, 0x180) = need;
            I(self, 0x184) = 0;
            if (I(self, 0x180) > 0) return 0x29;
        }
        return 0;
    }

    if (Mission_isEmpty(mission) == 0
        && (Mission_getType(mission) == 0xb8 || Mission_getType(mission) == 0xa8)
        && PlayerFixedObject_getDockingType(fixed) == 2) {
        // passenger drop-off
        int carried = I(fixed, 0x178);
        int maxPax = Ship_getMaxPassengers(Status_getShip());
        if (maxPax > 0 && carried < maxPax) {
            int avail = (Mission_getType(mission) == 0xa8)
                        ? Mission_getStatusValue(mission) : maxPax;
            avail -= carried;
            I(self, 0x180) = avail;
            int status = Mission_getStatusValue(mission) - carried;
            if (status < avail) I(self, 0x180) = status;
            I(self, 0x184) = 0;
            if (I(self, 0x180) > 0) return 0x23;
        }
        if (Mission_getType(mission) != 0xa8 && Ship_getMaxPassengers(Status_getShip()) == 0)
            return 0x2b;
        return 0;
    }

    if (Mission_isEmpty(mission) == 0 && Mission_getType(mission) == 0xb8
        && PlayerFixedObject_getDockingType(fixed) == 1) {
        int n = I(fixed, 0x178);
        if (n > 0) {
            I(self, 0x184) = 0;
            I(self, 0x180) = n;
            return 0x25;
        }
        return 0;
    }

    if (campaign != 0 && Mission_isEmpty(campaign) == 0
        && (Mission_getType(campaign) == 0xa7 || Mission_getType(campaign) == 0xae)
        && PlayerFixedObject_getDockingType(fixed) == 1) {
        if (Ship_hasCargo(Status_getShip(), Mission_getProductionGoodIndex(campaign)) != 0) {
            int amount = Item_getAmount(Ship_getCargo(Status_getShip(),
                                        Mission_getProductionGoodIndex(campaign)));
            I(self, 0x180) = amount;
            int need = Mission_getProductionGoodAmount(campaign) - Mission_getStatusValue(campaign);
            if (need < amount) I(self, 0x180) = need;
            I(self, 0x184) = 0;
            if (I(self, 0x180) > 0) return 0x29;
        }
    }
    return 0;
}

extern "C" void PlayerEgo_approachDockingPoint(PlayerEgo *self, void *hud, int /*hud2*/, void *radar)
{
    if (KIPlayer_isDying(self) != 0)
        return;

    int state = I(self, 0x314);

    if (state == 0) {
        void *station = P(self, 0x1e0);
        float pos[3];
        PlayerEgo_getPosition(self, pos);
        void *nav = KIPlayer_getNearestNavigationPoint(station, pos, (char *)self + 0x350);
        if (nav != 0) {
            if (P(self, 0x380) != nav) {
                if (P(self, 0x380) != 0)
                    SpacePoint_giveFree(P(self, 0x380));
                P(self, 0x380) = nav;
                SpacePoint_take(nav);
            }
            int dist = PE_adp_approach(self, station);
            if (dist < I(self, 0x1cc)) {
                I(self, 0x314) = 2;
                TargetFollowCamera_setLookAtCam(P(self, 0x88), false);
                TargetFollowCamera_useTargetsUpVector(P(self, 0x88), false);
            }
        }
        PE_adp_apply(self);
        return;
    }

    if (state == 2) {
        if (C(self, 0x2c4) != 0)
            PlayerEgo_setTurretMode(self, 0);
        int dist = PE_adp_glide(self);
        int radius = g_PE_adp_dockRadius[Ship_getIndex(Status_getShip())];
        if (dist < radius) {
            int ev = adp_arrivalEvent(self, P(self, 0x1e0));
            if (ev != 0)
                Hud_hudEvent(hud, ev, self);
            I(self, 0x314) = 1;        // hand back to manual control
        }
        PE_adp_apply(self);
        return;
    }

    if (state == 3) {
        int dist = PE_adp_glide(self);
        if (dist < 200) {
            // docking complete: restore cameras and free the nav point.
            P(self, 0x1e0) = 0;
            P(radar, 0x4) = 0;
            P(radar, 0x8) = 0;
            adp_clearDockVector(self);
            TargetFollowCamera_setActive(P(self, 0x88), true);
            TargetFollowCamera_setLookAtCam(P(self, 0x88), false);
            TargetFollowCamera_useTargetsUpVector(P(self, 0x88), false);
            LevelScript_resetCamera(P(self, 0x10));
            Player_resetGunDelay(P(self, 0x0), 0);
            I(self, 0x314) = 0;
            if (P(self, 0x380) != 0) {
                SpacePoint_giveFree(P(self, 0x380));
                P(self, 0x380) = 0;
            }
            if (P(self, 0x1e8) != 0) {
                operator_delete_(HackingGame_dtor(P(self, 0x1e8)));
                P(self, 0x1e8) = 0;
                Hud_setHackingGameActive(hud, false);
            }
            return;
        }
        PE_adp_apply(self);
    }
}
