#include "class.h"

AbyssEngine::AEMath::Vector Radar::elipsoidIntersect(
    int y, int x, AbyssEngine::AEMath::Vector value)
{
    int centerY = *(volatile int *)((char *)this + 0x108);
    float dy = (float)(centerY - x);
    float dy2 = dy * dy;
    int centerX = *(volatile int *)((char *)this + 0x104);
    float dx = (float)(centerX - y);
    float dx2 = dx * dx;
    float weightY = *(volatile float *)((char *)this + 0x110);
    float weightX = *(volatile float *)((char *)this + 0x10c);
    float distance = weightY * dy2 + weightX * dx2;

    if (distance >= 0.0f) {
        float scale = (distance - gGlobals->sqrt(distance)) / distance;
        if (scale >= 0.0f && scale <= 1.0f) {
            value.y = (float)(int)((float)x + scale * dy);
            value.x = (float)(int)((float)y + scale * dx);
        }
    }

    return value;
}
