#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_Galaxy_globals;

extern "C" float Galaxy_Globals_sqrt(void *g, float v);

int Galaxy::distancePercent(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    float sum = (float)(dy * dy + dx * dx);
    return (int)Galaxy_Globals_sqrt(*g_Galaxy_globals, sum);
}
