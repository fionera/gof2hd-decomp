#include "class.h"

struct Route {
    Route(int *pts, unsigned int n);
};

namespace AbyssEngine {
    namespace AERandom {
        int nextInt(int rng);
    }
}

inline void *operator new(__SIZE_TYPE__, void *p) noexcept { return p; }
extern "C" void *_Znwj(unsigned int size);

__attribute__((visibility("hidden"))) extern int *g_routeRng;
__attribute__((visibility("hidden"))) extern int (*g_routeRandom)(int rng, int bound);

Route *Level::createRoute(int count) {
    unsigned int n = count * 3;
    int *pts = new int[n];
    int *p = pts + 1;
    int *rng = g_routeRng;
    int (*rnd)(int, int) = g_routeRandom;
    for (int i = -1; i + 1 < (int)n; i = i + 3) {
        int sign = (rnd(*rng, 2) == 0) ? 1 : -1;
        p[-1] = (rnd(*rng, 30000) + 50000) * sign;
        p[0] = rnd(*rng, 20000) - 10000;
        if (i == -1) {
            pts[2] = AbyssEngine::AERandom::nextInt(*rng) + 50000;
        } else {
            int prev = p[-2];
            p[1] = AbyssEngine::AERandom::nextInt(*rng) + prev + 50000;
        }
        p = p + 3;
    }
    Route *r = (Route *)_Znwj(0x18);
    new (r) Route(pts, n);
    return r;
}
