#include "class.h"

struct Station;
struct Galaxy;
struct SolarSystem;
struct AEGeometry;
struct PlayerAsteroid;
struct Waypoint;
struct BoundingSphere;
struct LODManager;

__attribute__((visibility("hidden"))) extern int    *g_ca_stack;    // [DAT_000bf97c]
__attribute__((visibility("hidden"))) extern int   **g_ca_canvas;   // [DAT_000bf984] paint-canvas holder
__attribute__((visibility("hidden"))) extern int   **g_ca_galaxy;   // galaxy holder
__attribute__((visibility("hidden"))) extern int    *g_ca_rngHolder;// [DAT_000bf988] RNG object holder
__attribute__((visibility("hidden"))) extern char  **g_ca_lowDetail;// [DAT_000bfd1c] low-detail flag

extern "C" {
int   Status_inAlienOrbit_ca();
int   Status_getSystem_ca();
int   SolarSystem_getIndex_ca();
int   Status_getStation_ca();
int   Station_getIndex_ca(Station *s);
int   Status_getCurrentCampaignMission_ca();
int   Status_inSupernovaOrbit_ca();
int   Galaxy_getAsteroidProbabilities_ca(Galaxy *g, Station *st);

void  AERandom_setSeed_ca(long long seed);
void  AERandom_reset_ca();
int   AERandom_nextInt_ca(int rng);
// AERandom::nextInt(rng, bound) reached through the loaded function-pointer DAT_000bf98c.
int   AERandom_nextIntBound_ca(int rng, int bound);

void *Level_opnew_ca(unsigned size);
void  operator_deletearr_ca(void *p);

void  ArrayKIPlayer_ctor_ca(void *a);
void  ArraySetLength_KIPlayer_ca(int n, void *a);

void  Waypoint_ctor_ca(Waypoint *w, int x, int y, int z, void *route);
void  BoundingSphere_ctor_ca(BoundingSphere *bs);
void  AEGeometry_ctor_ca(AEGeometry *g, unsigned short mesh, void *canvas, int flag);
void  AEGeometry_setLodMeshes_ca(AEGeometry *g, short *meshes, int *dists, int count);
void  LODManager_addObject_ca(LODManager *m, AEGeometry *g);

void  PlayerAsteroid_ctor_ca(PlayerAsteroid *a, int x_or_id, AEGeometry *geo, int colVariant,
                             int kind, Vector *pos, float scale, int radius);
void  PlayerAsteroid_setAsteroidCenter_ca(PlayerAsteroid *a, float cx, float cy, float cz);

void  Vector_assign_ca(Vector *dst, Vector *src);
// Distance between the existing asteroid `idx` center and `pos` (the original is SIMD reject math).
float Level_ca_asteroidDistance(Level *self, unsigned idx, Vector *pos);
// Builds the lod-mesh distance table appropriate for the current detail level / distance band and
// installs it on the geometry (the original inlines several f64 immediates the decompiler corrupted).
void  Level_ca_installLodMeshes(Level *self, AEGeometry *geo, short baseMesh, int near);
}

// Level::createAsteroids() — fills the asteroid field for the current orbit. Picks a field origin
// from the mission/station context, then reject-samples spawn positions so asteroids do not overlap.
void Level::createAsteroids()
{
    char *self = (char *)this;

    int *rng = (int *)*(int **)&g_ca_rngHolder; // RNG object pointer-to-pointer
    int *rngObj = *(int **)g_ca_rngHolder;
    (void)rng;

    // base collision-variant index: 2 in the "ring" system (idx 0x16), else 0; 0 when in alien orbit.
    int colBase;
    if (Status_inAlienOrbit_ca() == 0) {
        Status_getSystem_ca();
        colBase = (SolarSystem_getIndex_ca() == 0x16) ? 2 : 0;
    } else {
        colBase = 0;
    }

    // asteroid container.
    void *arr = Level_opnew_ca(0xc);
    ArrayKIPlayer_ctor_ca(arr);
    *(void **)(self + 0xfc) = arr;

    Galaxy *gal = (Galaxy *)**g_ca_galaxy;
    Station *st = (Station *)Status_getStation_ca();
    int *prob = (int *)Galaxy_getAsteroidProbabilities_ca(gal, st);

    int *rngHolder = (int *)*(int **)g_ca_rngHolder;
    (void)rngHolder;
    int seed = *rngObj;
    Station *st2 = (Station *)Status_getStation_ca();
    Station_getIndex_ca(st2);
    AERandom_setSeed_ca((long long)seed);

    int countRoll = AERandom_nextIntBound_ca(*rngObj, 0x28);
    ArraySetLength_KIPlayer_ca(countRoll + 0x28, *(void **)(self + 0xfc));

    int rx = AERandom_nextIntBound_ca(*rngObj, 0x4e20); // DAT_000bf990 == 0x4e20
    int ry = AERandom_nextIntBound_ca(*rngObj, 0x4e20);
    int rz = AERandom_nextIntBound_ca(*rngObj, 0x4e20);

    int alien = Status_inAlienOrbit_ca();
    int camp  = Status_getCurrentCampaignMission_ca();

    // field-origin coordinates (x=ox, y=oy, z=oz).
    int ox, oy, oz;
    if (alien != 0) {
        oy = 0;
        oz = 30000;
        ox = -50000; // DAT_000bf994 default
        if (camp == 0x9a)
            ox = -70000;
    } else {
        bool placed = false;
        if (camp == 0x72) {
            Station *s = (Station *)Status_getStation_ca();
            if (Station_getIndex_ca(s) == 0x53) {
                oz = 30000;
                ox = 30000;   // DAT_000bf998
                oy = 0;
                placed = true; // jumps to LAB_000bf874 (oy = 0)
            }
        }
        if (!placed && camp == 0x59 && Status_inSupernovaOrbit_ca() != 0) {
            ox = 0;           // DAT_000bf99c
            oz = 0;           // DAT_000bf9a0
            oy = 0;
            placed = true;
        }
        if (!placed && camp == 0x5b) {
            Station *s = (Station *)Status_getStation_ca();
            if (Station_getIndex_ca(s) == 0x6e) {
                oy = 0;
                oz = 50000;
                ox = 60000;
                placed = true;
            }
        }
        if (!placed && camp == 0x91) {
            Station *s = (Station *)Status_getStation_ca();
            if (Station_getIndex_ca(s) == 0x70) {
                oz = 50000;
                ox = 50000;   // DAT_000bf9a4
                oy = 0;
                placed = true;
            }
        }
        if (!placed) {
            ox = rx - 50000;
            oz = rz + 20000;
            oy = ry - 50000;
            if (Status_getCurrentCampaignMission_ca() == 0 && *(int *)(self + 0xc0) == 3) {
                oz = 0;
                ox = 0;
                oy = 0;
            }
        }
    }

    AERandom_reset_ca();

    // field center vector (this+0xc8): note disasm stores (oy, ox, oz) into 0xc8/0xcc/0xd0.
    Vector center;
    center.x = (float)oy;
    center.y = (float)ox;
    center.z = (float)oz;
    Vector_assign_ca((Vector *)(self + 0xc8), &center);

    Waypoint *wp = (Waypoint *)Level_opnew_ca(0x138);
    Waypoint_ctor_ca(wp, oz, oy, ox, 0);
    *(Waypoint **)(self + 0xd8) = wp;

    BoundingSphere *bs = (BoundingSphere *)Level_opnew_ca(0x48);
    BoundingSphere_ctor_ca(bs);
    *(BoundingSphere **)(self + 0xc4) = bs;

    int density = AERandom_nextInt_ca(*rngObj) + 2; // # of "core" (dense) asteroids
    int alien2 = Status_inAlienOrbit_ca();

    void *canvas = (void *)**g_ca_canvas;
    int kind = 0x9a;
    int probCursor = 0;

    for (unsigned i = 0; i < **(unsigned **)(self + 0xfc); i = i + 1) {
        // choose asteroid kind from the probability table (skip in alien orbit -> fixed 0xa4).
        if (alien2 == 0) {
            bool ok = false;
            int cursor = probCursor;
            while (!ok) {
                int roll = AERandom_nextInt_ca(*rngObj);
                int next = 0;
                if (roll < *(int *)((int)prob + cursor * 4 + 4)) {
                    kind = *(int *)((int)prob + cursor * 4);
                    next = cursor + 2;
                    if (cursor > 8)
                        next = 0;
                    ok = (kind == 0xd9) || (kind < 0xa4);
                }
                cursor = next;
            }
            probCursor = cursor;
        } else {
            kind = 0xa4;
        }

        // spread radius: tighter (60000) for the dense core, wider otherwise.
        unsigned spread = (int)i < density ? 60000u : (unsigned)0xea60; // DAT_000bfd10
        float half = (float)(spread >> 1);

        // collision variant: 1 in alien orbit, 3 for the special "0xd9" kind, else colBase.
        int colVariant = colBase;
        if (Status_inAlienOrbit_ca() != 0)
            colVariant = 1;
        if (kind == 0xd9)
            colVariant = 3;

        // mesh id chosen from a small table indexed by colVariant.
        static const int meshTable[4] = {0x37a0, 0x37a4, 0x37a8, 0x37ac};
        int mesh = meshTable[colVariant & 3];

        // reject-sample a position far enough from already-placed asteroids.
        Vector pos;
        for (;;) {
            float cx = *(float *)(self + 0xc8);
            pos.x = (cx - half) + (float)AERandom_nextIntBound_ca(*rngObj, spread);
            float cy = *(float *)(self + 0xcc);
            pos.y = (cy - half) + (float)AERandom_nextIntBound_ca(*rngObj, spread);
            float cz = *(float *)(self + 0xd0);
            pos.z = (cz - half) + (float)AERandom_nextIntBound_ca(*rngObj, spread);
            if (i == 0 || (int)i >= density)
                break;
            bool farEnough = false;
            for (unsigned j = 0; j < i; j = j + 1) {
                float d = Level_ca_asteroidDistance(this, j, &pos);
                if (8000 < (int)d) { farEnough = true; break; }
            }
            if (farEnough)
                break;
        }

        AEGeometry *geo = (AEGeometry *)Level_opnew_ca(0xc0);
        AEGeometry_ctor_ca(geo, (unsigned short)mesh, canvas, 0);

        // install LOD meshes + register with the LOD manager (detail-dependent distance table).
        bool near = (int)i < density;
        Level_ca_installLodMeshes(this, geo, (short)mesh, near ? 1 : 0);
        LODManager_addObject_ca(*(LODManager **)self, geo);

        // per-asteroid random radius and scale.
        int base = (*g_ca_lowDetail && **g_ca_lowDetail != 0) ? 0x46 : (near ? 0x46 : 0x46);
        (void)base;
        int radius = AERandom_nextInt_ca(*rngObj) + (near ? 0x78 : 0x1e);
        float scale = (float)radius * 0.001f; // DAT_000bfcf4 scale immediate

        // larger asteroids may roll an extra collision shape (corrupted f64 compares in original).
        double dscale = (double)scale;
        if (dscale >= 1.0 && dscale >= 1.0 && dscale >= 1.0 &&
            AERandom_nextInt_ca(*rngObj) != 0) {
            AERandom_nextInt_ca(*rngObj);
        }

        PlayerAsteroid *a = (PlayerAsteroid *)Level_opnew_ca(0x170);
        PlayerAsteroid_ctor_ca(a, kind, geo, colVariant, kind, &pos, scale, (int)scale);
        *(PlayerAsteroid **)(*(int *)(*(int *)(self + 0xfc) + 4) + i * 4) = a;

        // virtual init(level) on the freshly built asteroid (vtable slot +0x14).
        int *obj = *(int **)(*(int *)(*(int *)(self + 0xfc) + 4) + i * 4);
        (**(void (***)(int *, Level *))(*obj + 0x14))(obj, this);

        PlayerAsteroid_setAsteroidCenter_ca(
            *(PlayerAsteroid **)(*(int *)(*(int *)(self + 0xfc) + 4) + i * 4),
            *(float *)(self + 0xc8), *(float *)(self + 0xcc), *(float *)(self + 0xd0));
    }

    if (prob != 0)
        operator_deletearr_ca(prob);
}
