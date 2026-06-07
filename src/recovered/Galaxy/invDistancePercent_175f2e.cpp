#include "class.h"

extern "C" int Galaxy_distancePercent5(int a, int b, int c, int d, int e);

int Galaxy::invDistancePercent(int x1, int y1, int x2, int y2)
{
    return 100 - Galaxy_distancePercent5(y2, x1, y1, x2, y2);
}
