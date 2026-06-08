#include "class.h"

// Galaxy::distance(SolarSystem* a, SolarSystem* b) — Euclidean distance between two systems'
// positions (Z scaled by 1/10), scaled by a global unit factor. Returns 0 when a==b (same index).
extern "C" int Galaxy_SS_getIndex(void *ss);
extern "C" int Galaxy_SS_getX(void *ss);
extern "C" int Galaxy_SS_getY(void *ss);
extern "C" int Galaxy_SS_getZ(void *ss);
extern "C" int Galaxy_idiv(int a, int b);
extern "C" float Galaxy_dist_sqrt(void *g, float v);
extern "C" void Galaxy_Vector_subassign(void *dst, const void *src);
extern "C" void Galaxy_dist_stack_chk_fail();

__attribute__((visibility("hidden"))) extern int **g_Galaxy_dist_canary;
__attribute__((visibility("hidden"))) extern void **g_Galaxy_dist_globals;
__attribute__((visibility("hidden"))) extern float g_Galaxy_dist_scale;

extern "C" float Galaxy_distance(void *self, void *a, void *b)
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

    if (*canary != saved) {
        Galaxy_dist_stack_chk_fail();
    }
    return result;
}
