#include "class.h"

struct Mission;
struct Player;
struct KIPlayer;
struct Globals;

__attribute__((visibility("hidden"))) extern int    *g_cm_stack;     // [DAT_000c036c]
__attribute__((visibility("hidden"))) extern int   **g_cm_rng;       // [DAT_000c0374]
__attribute__((visibility("hidden"))) extern Globals **g_cm_globals; // [DAT_000c0378]

extern "C" {
int  Status_getMission_cm();
int  Status_inAlienOrbit_cm();
int  Status_getLevel_cm();
int  Status_getCurrentCampaignMission_cm();
int  AERandom_nextInt_cm(int rng);
int  cm_randPos(int rng, int slot); // randomized position component
void *Level_opnew_cm(unsigned size);
void ArrayKIPlayer_ctor_cm(void *a);
void ArraySetLength_KIPlayer_cm(unsigned n, void *a);
int  Globals_getRandomEnemyFighter_cm(Globals *g, int race);
int  Level_createShip_cm(Level *self, int race, int b, int shipDesc, int wp, int hostile, int grp);
void Player_setAlwaysEnemy_cm(Player *p);
// The full per-mission-type scene construction (escorts, freighters, derelicts, generators, radio
// triggers, ...) — thousands of lines of SIMD-heavy mission scripting Ghidra could not lift.
void Level_cm_buildMissionScene(Level *self, Mission *mission);
}

// Level::createMission() — builds the in-flight actors/objectives for the player's active
// freelance mission (and the random alien-orbit ambush layered on top of it).
void Level::createMission()
{
    char *self = (char *)this;
    Mission *mission = (Mission *)Status_getMission_cm();
    if (mission == 0)
        return;

    // alien-orbit ambush wave: 2..N alien fighters at randomized far positions.
    if (Status_inAlienOrbit_cm() != 0) {
        int lvl = Status_getLevel_cm();
        int rng = *(int *)g_cm_rng;
        int roll = AERandom_nextInt_cm(rng);
        float base = (float)lvl * 0.5f - 1.0f;
        unsigned count = 2;
        if (2.0f <= base + (float)roll) {
            int r2 = AERandom_nextInt_cm(rng);
            count = (unsigned)(base + (float)r2);
        }
        int campA = Status_getCurrentCampaignMission_cm();
        int campB = Status_getCurrentCampaignMission_cm();
        if (campB == 0x21) count = 2;
        if (campA == 0x44) count = 2;

        void *arr = Level_opnew_cm(0xc);
        ArrayKIPlayer_ctor_cm(arr);
        *(void **)(self + 0xf8) = arr;
        ArraySetLength_KIPlayer_cm(count, arr);

        Globals *globals = *g_cm_globals;
        for (unsigned i = 0; i < count; i = i + 1) {
            int fighter = Globals_getRandomEnemyFighter_cm(globals, 9);
            int ship = Level_createShip_cm((Level *)self, 9, 0, fighter, 0, 1, 0);
            *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) = ship;
            int *kp = *(int **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4);
            float x = (float)(cm_randPos(rng, 0) - 60000);
            float y = (float)(cm_randPos(rng, 1) - 40000);
            float z = (float)(cm_randPos(rng, 2) - 60000);
            (*(void (**)(int *, float, float, float))(*kp + 0x48))(kp, x, y, z);
            Player_setAlwaysEnemy_cm(*(Player **)((char *)kp + 4));
        }
    }

    // everything else (mission-type-specific objects, objectives, escorts) lives in the helper.
    Level_cm_buildMissionScene((Level *)self, mission);
}
