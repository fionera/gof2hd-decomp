#include "class.h"

int BoundingAAB::outerCollide(float x, float y, float z)
{
    BoundingAAB *self = this;
    float centerX = F<float>(self, 0x8) + F<float>(self, 0x14);
    float extentX = F<float>(self, 0x20);
    if (!(centerX - extentX < x)) {
        return 0;
    }
    if (!(x < centerX + extentX)) {
        return 0;
    }

    float centerY = F<float>(self, 0xc) + F<float>(self, 0x18);
    float extentY = F<float>(self, 0x24);
    if (!(centerY - extentY < y)) {
        return 0;
    }
    if (!(y < centerY + extentY)) {
        return 0;
    }

    float centerZ = F<float>(self, 0x10) + F<float>(self, 0x1c);
    float extentZ = F<float>(self, 0x28);
    if (!(centerZ - extentZ < z)) {
        return 0;
    }
    if (z < centerZ + extentZ) {
        return 1;
    }
    return 0;
}
