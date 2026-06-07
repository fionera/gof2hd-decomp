#include "class.h"

unsigned int CameraGetCurrent(AbyssEngine::PaintCanvas *self)
{
    unsigned int cur = *(unsigned int *)((char *)self + 0x170);
    if (cur >= *(unsigned int *)((char *)self + 0x164)) {
        cur = 0xffffffff;
    }
    return cur;
}
