#include "gof2/Galaxy.h"


extern "C" void *Galaxy_ctor_op_new_array(unsigned int size);
extern "C" void *Galaxy_ctor_op_new(unsigned int size);
extern "C" void Galaxy_ctor_FileRead_ctor(void *self);
extern "C" void *Galaxy_ctor_FileRead_loadSystemsBinary(void *self);
extern "C" void *Galaxy_ctor_FileRead_dtor(void *self);
extern "C" void Galaxy_ctor_op_delete(void *p);
extern "C" float Galaxy_Globals_sqrt(void *g, float v);
extern "C" int Galaxy_distancePercent5(int a, int b, int c, int d, int e);
extern "C" int Galaxy_SS_getIndex(void *ss);
extern "C" int Galaxy_SS_getX(void *ss);
extern "C" int Galaxy_SS_getY(void *ss);
extern "C" int Galaxy_SS_getZ(void *ss);
extern "C" int Galaxy_idiv(int a, int b);
extern "C" float Galaxy_dist_sqrt(void *g, float v);
extern "C" void Galaxy_Vector_subassign(void *dst, const void *src);
extern "C" int Galaxy_pp_inAlienOrbit(void *status);
extern "C" void *Galaxy_pp_new(unsigned int size);
extern "C" void Galaxy_pp_delete(void *p);
extern "C" int Galaxy_pp_Station_getSystem(void *station);
extern "C" int Galaxy_pp_SolarSystem_getX(void *sys);
extern "C" int Galaxy_pp_SolarSystem_getY(void *sys);
extern "C" int Galaxy_pp_Item_getMinPriceSystem(void *item);
extern "C" void Galaxy_ArrayReleaseClasses_SolarSystem(void *arr);
extern "C" void *Galaxy_Array_SolarSystem_dtor(void *arr);
extern "C" int Galaxy_ap_inAlienOrbit(void *status);
extern "C" int Galaxy_ap_inSupernovaOrbit(void *status);
extern "C" int Galaxy_ap_getCurrentCampaignMission(void *status);
extern "C" void *Galaxy_ap_new(unsigned int size);
extern "C" void Galaxy_ap_delete(void *p);
extern "C" int Galaxy_ap_Station_getSystem(void *station);
extern "C" int Galaxy_ap_SolarSystem_getX(void *sys);
extern "C" int Galaxy_ap_SolarSystem_getY(void *sys);
extern "C" int Galaxy_ap_Item_getMinPriceSystem(void *item);
extern "C" void *Galaxy_gs_op_new(unsigned int size);
extern "C" void Galaxy_FileRead_ctor(void *self);
extern "C" int Galaxy_FileRead_loadStation(void *self, int index);
extern "C" void *Galaxy_FileRead_dtor(void *self);
extern "C" void Galaxy_gs_op_delete(void *p);

// ---- Galaxy_175e08.cpp ----
Galaxy::Galaxy()
{
    uint8_t *stations = (uint8_t *)Galaxy_ctor_op_new_array(0x87);
    P(this, 0x0) = stations;
    for (int i = 0; i != 0x87; i = i + 1) {
        stations[i] = 0;
    }
    void *fr = Galaxy_ctor_op_new(1);
    Galaxy_ctor_FileRead_ctor(fr);
    P(this, 0x4) = Galaxy_ctor_FileRead_loadSystemsBinary(fr);
    Galaxy_ctor_op_delete(Galaxy_ctor_FileRead_dtor(fr));
}

// ---- reset_175e82.cpp ----
void Galaxy::reset()
{
    uint8_t *stations = (uint8_t *)P(this, 0x0);
    for (int i = 0; i != 0x87; i = i + 1) {
        stations[i] = 0;
    }
}

// ---- distancePercent_175f44.cpp ----
__attribute__((visibility("hidden"))) extern void **g_Galaxy_globals;


int Galaxy::distancePercent(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    float sum = (float)(dy * dy + dx * dx);
    return (int)Galaxy_Globals_sqrt(*g_Galaxy_globals, sum);
}

// ---- visitStation_175f26.cpp ----
void Galaxy::visitStation(int index)
{
    uint8_t *stations = (uint8_t *)P(this, 0x0);
    stations[index] = 1;
}

// ---- invDistancePercent_175f2e.cpp ----
int Galaxy::invDistancePercent(int x1, int y1, int x2, int y2)
{
    return 100 - Galaxy_distancePercent5(y2, x1, y1, x2, y2);
}

// ---- setVisited_175ef4.cpp ----
void Galaxy::setVisited(bool *src, int count)
{
    int i;
    for (i = 0; i < count; i = i + 1) {
        uint8_t *stations = (uint8_t *)P(this, 0x0);
        stations[i] = ((uint8_t *)src)[i];
    }
    while (true) {
        if (count >= 0x87)
            return;
        uint8_t *stations = (uint8_t *)P(this, 0x0);
        stations[count] = 0;
        count = count + 1;
    }
}

// ---- getSystem_175e98.cpp ----
int Galaxy::getSystem(int index)
{
    if (index < 0)
        return 0;
    void *systems = P(this, 0x4);
    void *data = P(systems, 0x4);
    return ((int *)data)[index];
}

// ---- distance_175f80.cpp ----
// Galaxy::distance(SolarSystem* a, SolarSystem* b) — Euclidean distance between two systems'
// positions (Z scaled by 1/10), scaled by a global unit factor. Returns 0 when a==b (same index).

__attribute__((visibility("hidden"))) extern int **g_Galaxy_dist_canary;
__attribute__((visibility("hidden"))) extern void **g_Galaxy_dist_globals;
__attribute__((visibility("hidden"))) extern float g_Galaxy_dist_scale;

float Galaxy_distance(void *self, void *a, void *b)
{
    int *canary = *g_Galaxy_dist_canary;
    int saved = *canary;

    float result = 0.0f;
    if (Galaxy_SS_getIndex(a) != Galaxy_SS_getIndex(b)) {
        float va[3];
        float vb[3];
        va[0] = (float)Galaxy_SS_getX(a);
        va[1] = (float)Galaxy_SS_getY(a);
        va[2] = (float)Galaxy_idiv(Galaxy_SS_getZ(a), 10);
        vb[0] = (float)Galaxy_SS_getX(b);
        vb[1] = (float)Galaxy_SS_getY(b);
        vb[2] = (float)Galaxy_idiv(Galaxy_SS_getZ(b), 10);

        Galaxy_Vector_subassign(va, vb);
        float sq = va[1] * va[1] + va[0] * va[0] + va[2] * va[2];
        result = Galaxy_dist_sqrt(*g_Galaxy_dist_globals, sq) * g_Galaxy_dist_scale;
    }


    return result;
}

// ---- distance (member) 175f80 ----
// Galaxy::distance(SolarSystem* a, SolarSystem* b): Euclidean distance between the two systems'
// galactic-map positions, with the Z coordinate compressed by 1/10 (the star map is much flatter
// in depth than in plane), scaled by the global unit factor. Same-system pairs are distance 0.
float Galaxy::distance(SolarSystem *a, SolarSystem *b)
{
    if (Galaxy_SS_getIndex(a) == Galaxy_SS_getIndex(b))
        return 0.0f;

    float pa[3];
    float pb[3];
    pa[0] = (float)Galaxy_SS_getX(a);
    pa[1] = (float)Galaxy_SS_getY(a);
    pa[2] = (float)Galaxy_idiv(Galaxy_SS_getZ(a), 10);
    pb[0] = (float)Galaxy_SS_getX(b);
    pb[1] = (float)Galaxy_SS_getY(b);
    pb[2] = (float)Galaxy_idiv(Galaxy_SS_getZ(b), 10);

    Galaxy_Vector_subassign(pa, pb);
    float sq = pa[0] * pa[0] + pa[1] * pa[1] + pa[2] * pa[2];
    return Galaxy_dist_sqrt(*g_Galaxy_dist_globals, sq) * g_Galaxy_dist_scale;
}

// ---- getPlasmaProbabilities_176248.cpp ----
__attribute__((visibility("hidden"))) extern void **g_Galaxy_pp_status;
__attribute__((visibility("hidden"))) extern void **g_Galaxy_pp_items;


void *Galaxy::getPlasmaProbabilities(void *station)
{
    int alien = Galaxy_pp_inAlienOrbit(*g_Galaxy_pp_status);
    void *systems = alien == 0 ? P(this, 0x4) : 0;
    void *itemTable = *g_Galaxy_pp_items;

    int *probs = (int *)Galaxy_pp_new(0x10);
    int *ids = (int *)Galaxy_pp_new(0x10);

    int slot = 0;
    for (int id = 0xc9; id != 0xcd; id = id + 1) {
        ids[slot] = id;
        int prob;
        int next;
        if (alien == 0) {
            int sys = Galaxy_pp_Station_getSystem(station);
            int sysX = Galaxy_pp_SolarSystem_getX(((void **)P(systems, 0x4))[sys]);
            sys = Galaxy_pp_Station_getSystem(station);
            int sysY = Galaxy_pp_SolarSystem_getY(((void **)P(systems, 0x4))[sys]);
            int it = Galaxy_pp_Item_getMinPriceSystem(((void **)P(itemTable, 0x4))[id]);
            int itX = Galaxy_pp_SolarSystem_getX(((void **)P(systems, 0x4))[it]);
            it = Galaxy_pp_Item_getMinPriceSystem(((void **)P(itemTable, 0x4))[id]);
            int itY = Galaxy_pp_SolarSystem_getY(((void **)P(systems, 0x4))[it]);
            prob = invDistancePercent(sysX, sysY, itX, itY);
            next = slot + 1;
            if (prob < 0x32)
                prob = 0;
        } else {
            prob = 0;
            next = slot;
        }
        probs[slot] = prob;
        slot = next;
    }

    bool sorted = true;
    int i = 1;
    do {
        for (; i != 4; i = i + 1) {
            int prev = i - 1;
            int a = probs[i];
            int b = probs[prev];
            if (b < a) {
                probs[prev] = a;
                int idPrev = ids[prev];
                int idCur = ids[i];
                probs[i] = b;
                ids[prev] = idCur;
                ids[i] = idPrev;
                sorted = false;
            }
        }
        bool again = !sorted;
        sorted = true;
        i = 1;
        if (!again)
            break;
    } while (true);

    int sub = 0;
    for (int k = 0; k != 4; k = k + 1) {
        int v = probs[k];
        if (0 < v)
            probs[k] = v + sub;
        sub = sub - 2;
    }

    int *out = (int *)Galaxy_pp_new(0x20);
    for (unsigned int j = 0; j <= 7; j = j + 2) {
        out[j] = *(int *)((char *)ids + j * 2);
        out[j + 1] = *(int *)((char *)probs + j * 2);
    }

    Galaxy_pp_delete(probs);
    Galaxy_pp_delete(ids);
    return out;
}

// ---- _Galaxy_175e54.cpp ----
// Destructor: free the visited-flag array, release/destroy the SolarSystem* Array.
Galaxy::~Galaxy()
{
    ::operator delete[](P(this, 0x0));
    void *systems = P(this, 0x4);
    P(this, 0x0) = 0;
    Galaxy_ArrayReleaseClasses_SolarSystem(systems);
    if (P(this, 0x4) != 0) {
        ::operator delete(Galaxy_Array_SolarSystem_dtor(P(this, 0x4)));
    }
    P(this, 0x4) = 0;
}

// ---- getSystems_175e94.cpp ----
// Returns the owned Array<SolarSystem*> (stored at +0x4).
void *Galaxy::getSystems()
{
    return P(this, 0x4);
}

// ---- getVisited_175f22.cpp ----
// Returns the per-station visited-flag array (0x87 bytes, stored at +0x0).
void *Galaxy::getVisited()
{
    return P(this, 0x0);
}

// ---- getAsteroidProbabilities_176098.cpp ----
__attribute__((visibility("hidden"))) extern void **g_Galaxy_ap_status;
__attribute__((visibility("hidden"))) extern void **g_Galaxy_ap_items;


void *Galaxy::getAsteroidProbabilities(void *station)
{
    int alien = Galaxy_ap_inAlienOrbit(*g_Galaxy_ap_status);
    int supernova = Galaxy_ap_inSupernovaOrbit(*g_Galaxy_ap_status);
    void *systems = alien == 0 ? P(this, 0x4) : 0;
    void *itemTable = *g_Galaxy_ap_items;

    int *probs = (int *)Galaxy_ap_new(0x2c);
    int *ids = (int *)Galaxy_ap_new(0x2c);

    int slot = 0;
    for (int id = 0x9a; id != 0xa4; id = id + 1) {
        ids[slot] = id;
        int prob;
        int next;
        if (alien == 0) {
            int sys = Galaxy_ap_Station_getSystem(station);
            int sysX = Galaxy_ap_SolarSystem_getX(((void **)P(systems, 0x4))[sys]);
            sys = Galaxy_ap_Station_getSystem(station);
            int sysY = Galaxy_ap_SolarSystem_getY(((void **)P(systems, 0x4))[sys]);
            int it = Galaxy_ap_Item_getMinPriceSystem(((void **)P(itemTable, 0x4))[id]);
            int itX = Galaxy_ap_SolarSystem_getX(((void **)P(systems, 0x4))[it]);
            it = Galaxy_ap_Item_getMinPriceSystem(((void **)P(itemTable, 0x4))[id]);
            int itY = Galaxy_ap_SolarSystem_getY(((void **)P(systems, 0x4))[it]);
            prob = invDistancePercent(sysX, sysY, itX, itY);
            next = slot + 1;
            if (prob < 0x32)
                prob = 0;
        } else {
            prob = 0;
            next = slot;
        }
        probs[slot] = prob;
        slot = next;
    }

    ids[10] = 0xa4;
    probs[10] = alien != 0 ? 100 : 0;

    bool sorted = true;
    int i = 1;
    do {
        for (; i != 0xb; i = i + 1) {
            int prev = i - 1;
            int a = probs[i];
            int b = probs[prev];
            if (b < a) {
                probs[prev] = a;
                int idPrev = ids[prev];
                int idCur = ids[i];
                probs[i] = b;
                ids[prev] = idCur;
                ids[i] = idPrev;
                sorted = false;
            }
        }
        bool again = !sorted;
        sorted = true;
        i = 1;
        if (!again)
            break;
    } while (true);

    int sub = 0;
    for (int k = 0; k != 0xb; k = k + 1) {
        int v = probs[k];
        if (0 < v)
            probs[k] = v + sub;
        sub = sub - 2;
    }

    int *out = (int *)Galaxy_ap_new(0x58);
    for (unsigned int j = 0; j < 0x16; j = j + 2) {
        out[j] = *(int *)((char *)ids + j * 2);
        out[j + 1] = *(int *)((char *)probs + j * 2);
        if (supernova != 0 && Galaxy_ap_getCurrentCampaignMission(*g_Galaxy_ap_status) > 0x59) {
            out[j] = 0xd9;
        }
    }

    Galaxy_ap_delete(probs);
    Galaxy_ap_delete(ids);
    return out;
}

// ---- getStation_175ea8.cpp ----
__attribute__((visibility("hidden"))) extern void **g_Galaxy_gs_globals;


int Galaxy::getStation(int index)
{
    int result;
    if (index < 0) {
        result = I(*g_Galaxy_gs_globals, 0x78);
    } else {
        void *fr = Galaxy_gs_op_new(1);
        Galaxy_FileRead_ctor(fr);
        result = Galaxy_FileRead_loadStation(fr, index);
        Galaxy_gs_op_delete(Galaxy_FileRead_dtor(fr));
    }
    return result;
}
