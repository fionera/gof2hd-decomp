#include "class.h"

void Trail::setWidth(int width)
{
    int oldWidth = i32(this, 0x0c);
    int byteOffset = 0;
    int delta = oldWidth - width;
    AbyssEngine::PaintCanvas **canvas = gTrailCanvasSetWidth;
    int vertex = 0;
    while (vertex < (i32(this, 0x24) + 1) * 2) {
        int *point = (int *)((char *)ptr(this, 0x18) + byteOffset);
        int x = point[0] + delta;
        float fx = (float)x;
        float fz = (float)point[2];
        float fy = (float)point[1];
        point[0] = x;
        (*canvas)->MeshSetPoint(u32(this, 0x10), (uint16_t)vertex, fx, fy, fz);
        vertex++;
        byteOffset += 0x0c;
    }

    i32(this, 0x0c) = width;
}
