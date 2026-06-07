#include "class.h"

using AbyssEngine::AEMath::Vector;

void Trail::reset(Vector value)
{
    int z = (int)value.z;
    int y = (int)value.y;
    int x = (int)value.x;

    for (int i = 0; i < i32(this, 0x20); i += 3) {
        int *point = (int *)((char *)ptr(this, 0x18) + i * 4);
        point[0] = x;
        point[1] = y;
        point[2] = z;
    }

    update(value, value);
}
