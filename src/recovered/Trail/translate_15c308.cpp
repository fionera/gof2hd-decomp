#include "class.h"

using AbyssEngine::AEMath::Vector;

void Trail::translate(const Vector &delta)
{
    float dx = delta.x;
    float dy = delta.y;
    float dz = delta.z;

    for (int i = 0; i < i32(this, 0x20); i += 3) {
        int *point = (int *)((char *)ptr(this, 0x18) + i * 4);
        int x = (int)(dx + (float)point[0]);
        int y = (int)(dy + (float)point[1]);
        int z = (int)(dz + (float)point[2]);
        point[0] = x;
        point[1] = y;
        point[2] = z;
    }
}
