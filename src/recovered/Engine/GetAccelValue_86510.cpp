#include "class.h"

extern "C" double *Engine_GetAccelValue(Engine *self)
{
    double x = *(double *)((char *)self + 0x4b0);
    double y;
    if (*(int *)(**(char ***)((char *)self + 0x30) + 0x30) == 1) {
        x = -x;
        y = -*(double *)((char *)self + 0x4b8);
    } else {
        y = *(double *)((char *)self + 0x4b8);
    }
    *(volatile double *)((char *)self + 0x4c8) = x;
    *(volatile double *)((char *)self + 0x4d0) = y;
    *(double *)((char *)self + 0x4d8) = *(volatile double *)((char *)self + 0x4c0);
    return (double *)((char *)self + 0x4c8);
}
