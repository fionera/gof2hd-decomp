#include "class.h"

struct Waypoint;
struct AEGeometry;
struct Player;
struct PlayerFixedObject;

__attribute__((visibility("hidden"))) extern int *g_cso2_stack;   // [DAT_000cea34]
__attribute__((visibility("hidden"))) extern int **g_cso2_rng;    // [DAT_000cea38]

extern "C" {
int  AERandom_nextInt_cso2(int rng);
int  cso2_rand20000(int rng); // (*pcVar23)(rng, 20000) randomized jitter
// Constructs the requested static-object type (junk, turret, sentry gun, jumpgate gear, capital
// landmark, etc.) fully positioned at (x,y,z); the original is a giant per-type cascade of
// SIMD-built geometry that Ghidra could not lift. Returns the new KIPlayer-derived object.
int  Level_cso2_construct(Level *self, int type, int x, int y, int z);
}

// Level::createStaticObject(Waypoint* wp, int type, bool jitter) — spawns one scenery / structure
// object at the waypoint (optionally with a small random position offset).
extern "C" int Level_createStaticObject(Level *thisptr, Waypoint *wp, int type, int jitter)
{
    char *self = (char *)thisptr;

    int x = 0, y = 0, z = 0;
    if (wp != 0) {
        x = *(int *)((char *)wp + 0x124);
        y = *(int *)((char *)wp + 0x128);
        z = *(int *)((char *)wp + 0x12c);
    }
    if (jitter) {
        int rng = *(int *)g_cso2_rng;
        x = x + cso2_rand20000(rng) - 10000;
        y = y + cso2_rand20000(rng) - 10000;
        z = z + cso2_rand20000(rng) - 10000;
    }

    // type 0x4215 picks one of three debris meshes at random; everything else maps 1:1.
    if (type == 0x4215) {
        int r = AERandom_nextInt_cso2(*(int *)g_cso2_rng);
        type = (r == 0) ? 0x4215 : (r == 1) ? 0x4216 : 0x4217;
    }

    (void)self;
    return Level_cso2_construct(thisptr, type, x, y, z);
}
