#include "class.h"

float CameraGetCurrentFactor1(AbyssEngine::PaintCanvas *self)
{
    unsigned int cur = *(unsigned int *)((char *)self + 0x170);
    if (cur >= *(unsigned int *)((char *)self + 0x164)) {
        return 1.0f;
    }
    char *cam = (*(char ***)((char *)self + 0x168))[cur];
    return *(float *)(cam + 0x48);
}
