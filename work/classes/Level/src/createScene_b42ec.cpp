#include "class.h"

struct Mission;
struct Station;
struct Ship;
struct Agent;
struct Player;
struct KIPlayer;
struct PlayerStatic;
struct AEGeometry;
struct Globals;

__attribute__((visibility("hidden"))) extern int    *g_csc_stack;    // [DAT_000c46a0]
__attribute__((visibility("hidden"))) extern int   **g_csc_missionDef;// [DAT_000c46ac]
__attribute__((visibility("hidden"))) extern void  **g_csc_canvas;   // [DAT_000c46b0] etc.
__attribute__((visibility("hidden"))) extern int    *g_csc_rng;      // [DAT_000c46bc]/[DAT_000c4ba0]
__attribute__((visibility("hidden"))) extern Globals **g_csc_globals;// [DAT_000c46d0]
__attribute__((visibility("hidden"))) extern int   **g_csc_shipHost; // [DAT_000c46a4]

extern "C" {
void  ArrayReleaseClasses_KIPlayer_csc(void *a);
void *ArrayKIPlayer_dtor_csc(void *a);
void  operator_delete_csc(void *p);
void *Level_opnew_csc(unsigned size);
void *Level_opnew_arr_csc(unsigned size);
void  ArrayKIPlayer_ctor_csc(void *a);
void  ArraySetLength_KIPlayer_csc(unsigned n, void *a);
void  ArrayAdd_KIPlayer_csc(KIPlayer *k, void *a);
void  Level_createPlayer_csc(Level *self);
void  Status_setMission_csc(Mission *m);
void  Level_createMission_csc(Level *self);
int   Status_getCurrentCampaignMission_csc();
int   Status_getSystem_csc();
int   SolarSystem_getRace_csc();
int   Status_getStation_csc();
int   Station_getIndex_csc(Station *s);
int   Station_getAgents_csc(Station *s);
int   Station_getShips_csc();
int   Status_getShip_csc();
int   Ship_getIndex_csc();
int   Ship_getRace_csc(Ship *s);
int   Agent_getRace_csc(Agent *a);
int   Agent_getImageParts_csc(Agent *a);
int   Agent_isMale_csc(Agent *a);
int   AERandom_nextInt_csc(int rng);
int   Globals_getRandomEnemyFighter_csc(Globals *g, int race);
int   Level_createShip_csc(Level *self, int a, int b, int shipDesc, int wp, int flagA, int flagB);
void  AEGeometry_ctor_csc(AEGeometry *g, unsigned mesh, void *canvas, int flag);
void *AEGeometry_dtor_csc(AEGeometry *g);
void  AEGeometry_addChild_csc(AEGeometry *parent, AEGeometry *child);
void  AEGeometry_setRotation_csc(AEGeometry *g, Vector *r);
void  PlayerStatic_ctor_csc(PlayerStatic *p, int a, AEGeometry *geo);
void  PlayerFighter_removeTrail_csc(int pf);
void  PlayerFighter_setExhaustVisible_csc(int pf);
void  KIPlayer_setToSleep_csc(KIPlayer *k);
void  Player_setAlwaysFriend_csc(Player *p, int flag);
// Position/rotate a freshly created static actor — corrupted SIMD in the original.
void  Level_csc_placeActor(Level *self, int actor, int idx, int profile);
}

// Level::createScene() — builds the non-flight presentation scenes (cut-scene mode 2, station
// crew gallery mode 4, and ship-showroom mode 0x17).
void Level::createScene()
{
    char *self = (char *)this;

    if (*(void **)(self + 0xf8) != 0) {
        ArrayReleaseClasses_KIPlayer_csc(*(void **)(self + 0xf8));
        if (*(void **)(self + 0xf8) != 0)
            operator_delete_csc(ArrayKIPlayer_dtor_csc(*(void **)(self + 0xf8)));
    }
    int mode = *(int *)(self + 0xc0);
    *(int *)(self + 0xf8) = 0;

    if (mode == 2) {
        Level_createPlayer_csc(this);
        Status_setMission_csc((Mission *)**g_csc_missionDef);
        Level_createMission_csc(this);
        if (Status_getCurrentCampaignMission_csc() == 0x2b) {
            void *canvas = *g_csc_canvas;
            AEGeometry *g = (AEGeometry *)Level_opnew_csc(0xc0);
            AEGeometry_ctor_csc(g, 0x37d0, canvas, 0);
            PlayerStatic *p = (PlayerStatic *)Level_opnew_csc(0x130);
            PlayerStatic_ctor_csc(p, -1, g);
            ArrayAdd_KIPlayer_csc((KIPlayer *)p, *(void **)(self + 0xf8));
            g = (AEGeometry *)Level_opnew_csc(0xc0);
            AEGeometry_ctor_csc(g, 0x37d1, canvas, 0);
            p = (PlayerStatic *)Level_opnew_csc(0x130);
            PlayerStatic_ctor_csc(p, -1, g);
            ArrayAdd_KIPlayer_csc((KIPlayer *)p, *(void **)(self + 0xf8));
        }
        return;
    }

    if (mode == 4) {
        Status_getSystem_csc();
        int race = SolarSystem_getRace_csc();
        unsigned crew = (race == 1) ? 2 : 3;
        Station *st = (Station *)Status_getStation_csc();
        int *agents = (int *)Station_getAgents_csc(st);
        char taken[7];
        void *canvas = *g_csc_canvas;

        if (agents == 0) {
            void *arr = Level_opnew_csc(0xc);
            ArrayKIPlayer_ctor_csc(arr);
            *(void **)(self + 0xf8) = arr;
            ArraySetLength_KIPlayer_csc(3, arr);
        } else {
            int nAgents = *agents;
            void *arr = Level_opnew_csc(0xc);
            ArrayKIPlayer_ctor_csc(arr);
            *(void **)(self + 0xf8) = arr;
            ArraySetLength_KIPlayer_csc(nAgents * 3 + crew, arr);
            for (int j = 0; j != 7; j = j + 1) taken[j] = 0;

            for (int a = 0; a < nAgents; a = a + 1) {
                Agent *ag = *(Agent **)(agents[1] + a * 4);
                int part = Agent_getRace_csc(ag);
                if (part == 3) {
                    if (Agent_getImageParts_csc(ag) == 0) part = 3;
                    else {
                        int *ip = (int *)Agent_getImageParts_csc(ag);
                        part = (*ip != 2) ? 3 : *ip;
                    }
                }
                int seat;
                do { seat = AERandom_nextInt_csc(*g_csc_rng); } while (taken[seat] != 0);
                taken[seat] = 1;

                AEGeometry *g = (AEGeometry *)Level_opnew_csc(0xc0);
                AEGeometry_ctor_csc(g, (unsigned)part, canvas, 0);
                PlayerStatic *p = (PlayerStatic *)Level_opnew_csc(0x130);
                PlayerStatic_ctor_csc(p, -1, g);
                *(PlayerStatic **)(*(int *)(*(int *)(self + 0xf8) + 4) + a * 4) = p;
                Level_csc_placeActor(this, (int)p, seat, 0);

                g = (AEGeometry *)Level_opnew_csc(0xc0);
                AEGeometry_ctor_csc(g, (unsigned)mode, canvas, 0);
                p = (PlayerStatic *)Level_opnew_csc(0x130);
                PlayerStatic_ctor_csc(p, -1, g);
                *(PlayerStatic **)(*(int *)(*(int *)(self + 0xf8) + 4) + (nAgents + a) * 4) = p;

                g = (AEGeometry *)Level_opnew_csc(0xc0);
                AEGeometry_ctor_csc(g, 0x380c, canvas, 0);
                p = (PlayerStatic *)Level_opnew_csc(0x130);
                PlayerStatic_ctor_csc(p, -1, g);
                *(PlayerStatic **)(*(int *)(*(int *)(self + 0xf8) + 4) + (nAgents * 2 + a) * 4) = p;
            }
        }
        for (unsigned u = 0; u < crew; u = u + 1) {
            AEGeometry *g = (AEGeometry *)Level_opnew_csc(0xc0);
            AEGeometry_ctor_csc(g, (unsigned)mode, canvas, 0);
            PlayerStatic *p = (PlayerStatic *)Level_opnew_csc(0x130);
            PlayerStatic_ctor_csc(p, -1, g);
            *(PlayerStatic **)((*(int **)(self + 0xf8))[1] +
                               (**(int **)(self + 0xf8) + (u - crew)) * 4) = p;
        }
        return;
    }

    if (mode == 0x17) {
        int *host = *g_csc_shipHost;
        Station *st = (Station *)Status_getStation_csc();
        unsigned race;
        if (Station_getIndex_csc(st) == 0x65) race = 8;
        else {
            st = (Station *)Status_getStation_csc();
            if (Station_getIndex_csc(st) == 100) race = 7;
            else { Status_getSystem_csc(); race = SolarSystem_getRace_csc(); }
        }

        void *arr = Level_opnew_csc(0xc);
        ArrayKIPlayer_ctor_csc(arr);
        *(void **)(self + 0xf8) = arr;
        ArraySetLength_KIPlayer_csc(1, arr);

        Status_getShip_csc();
        int shipIdx = Ship_getIndex_csc();
        Ship *ship = (Ship *)Status_getShip_csc();
        int shipRace = Ship_getRace_csc(ship);
        int actor = Level_createShip_csc(this, shipRace, 0, shipIdx, 0,
                                         *(int *)(self + 0xc0) != 0x17, 0);
        **(int **)(*(int *)(self + 0xf8) + 4) = actor;
        Level_csc_placeActor(this, actor, shipIdx, 1);
        PlayerFighter_removeTrail_csc(actor);
        PlayerFighter_setExhaustVisible_csc(actor);
        KIPlayer_setToSleep_csc((KIPlayer *)actor);
        Player_setAlwaysFriend_csc(*(Player **)(actor + 4), 1);

        void *canvas = *g_csc_canvas;
        for (unsigned u = 0; u < 4; u = u + 1) {
            // decoration meshes around the showroom ship.
            AEGeometry *g = (AEGeometry *)Level_opnew_csc(0xc0);
            AEGeometry_ctor_csc(g, (unsigned)(0x3800 + u), canvas, 0);
            Vector rot = {0, 0, 0};
            AEGeometry_setRotation_csc(g, &rot);
            PlayerStatic *p = (PlayerStatic *)Level_opnew_csc(0x130);
            PlayerStatic_ctor_csc(p, -1, g);
            if ((int)race < 4 && race != 1) {
                AEGeometry *child = (AEGeometry *)Level_opnew_csc(0xc0);
                AEGeometry_ctor_csc(child, (unsigned)u, canvas, 0);
                AEGeometry_addChild_csc(g, child);
                operator_delete_csc(AEGeometry_dtor_csc(child));
            }
            ArrayAdd_KIPlayer_csc((KIPlayer *)p, *(void **)(self + 0xf8));
        }

        // background traffic fighters.
        Station *st2 = (Station *)Status_getStation_csc();
        bool fromStationShips = (Station_getIndex_csc(st2) == 0x6c) &&
                                (*(int *)(*host + 0x114) == 3);
        int rng = *g_csc_rng;
        int spawnCount = AERandom_nextInt_csc(rng);
        if (fromStationShips) {
            unsigned *ships = (unsigned *)Station_getShips_csc();
            spawnCount = (ships == 0) ? 0 : (int)*ships;
        }
        char seats[64];
        for (int j = 0; j < 64; j = j + 1) seats[j] = 0;

        for (int s = 0; s < spawnCount; s = s + 1) {
            int r = AERandom_nextInt_csc(rng);
            unsigned spawnRace = race;
            if (r < 0x1e) {
                spawnRace = AERandom_nextInt_csc(rng);
                if (AERandom_nextInt_csc(rng) < 0x1e) spawnRace = 8;
            }
            int fighter = Globals_getRandomEnemyFighter_csc(*g_csc_globals, spawnRace);
            Station *st3 = (Station *)Status_getStation_csc();
            if (Station_getIndex_csc(st3) == 100) {
                int pick = AERandom_nextInt_csc(rng);
                fighter = (pick == 1) ? 0x26 : (pick == 0) ? 0x25 : 0x28;
            }
            if (fromStationShips) {
                Station_getShips_csc();
                fighter = Ship_getIndex_csc();
            }
            KIPlayer *k = (KIPlayer *)Level_createShip_csc(this, 0, 0, fighter, 0, 0, 0);
            int seat = AERandom_nextInt_csc(rng);
            int guard = -100;
            while (guard != 0 && seats[seat & 0x3f] != 0) {
                seat = AERandom_nextInt_csc(rng);
                guard = guard + 1;
            }
            seats[seat & 0x3f] = 1;
            Level_csc_placeActor(this, (int)k, seat, 2);
            Player_setAlwaysFriend_csc(*(Player **)((char *)k + 4), 1);
            KIPlayer_setToSleep_csc(k);
            PlayerFighter_setExhaustVisible_csc((int)k);
            ArrayAdd_KIPlayer_csc(k, *(void **)(self + 0xf8));
        }
    }
}
