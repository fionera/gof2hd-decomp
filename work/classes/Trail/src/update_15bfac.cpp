#include "class.h"

using AbyssEngine::AEMath::Matrix;

extern "C" void Trail_MatrixSetTranslation(void *out, Matrix *matrix, float x, float y, float z);

void Trail::update(float ax, float ay, float az, float bx, float by, float bz)
{
    int *points = (int *)ptr(this, 0x18);
    float width = (float)i32(this, 0x0c);

    points[0] = (int)(ax - width);
    points[1] = (int)ay;
    points[2] = (int)az;

    width = (float)i32(this, 0x0c);
    points[3] = (int)(width + ax);
    points[4] = (int)ay;
    points[5] = (int)az;

    width = (float)i32(this, 0x0c);
    points[6] = (int)(bx - width);
    points[7] = (int)by;
    points[8] = (int)bz;

    width = (float)i32(this, 0x0c);
    points[9] = (int)(width + bx);
    points[10] = (int)by;
    points[11] = (int)bz;

    int *copy = points + i32(this, 0x20);
    for (int i = i32(this, 0x20) - 1; 10 < i; i -= 6) {
        copy[-2] = copy[-8];
        copy[-1] = copy[-7];
        *(uint64_t *)(copy - 6) = *(uint64_t *)(copy - 12);
        *(uint64_t *)(copy - 4) = *(uint64_t *)(copy - 10);
        copy -= 6;
    }

    int byteOffset = 0;
    int pointIndex = 0;
    while (pointIndex < i32(this, 0x20)) {
        int *source = (int *)((char *)points + byteOffset);
        int *relative = (int *)((char *)ptr(this, 0x1c) + byteOffset);
        pointIndex += 3;
        relative[0] = (int)((float)source[0] - ax);
        relative[1] = (int)((float)source[1] - ay);
        relative[2] = (int)((float)source[2] - az);
        byteOffset += 0x0c;
    }

    byteOffset = 0;
    int vertex = 0;
    AbyssEngine::PaintCanvas **canvas = gTrailCanvasUpdate;
    while (vertex < (i32(this, 0x24) + 1) * 2) {
        int *point = (int *)((char *)ptr(this, 0x1c) + byteOffset);
        float x = (float)point[0];
        float z = (float)point[2];
        float y = (float)point[1];
        (*canvas)->MeshSetPoint(u32(this, 0x10), (uint16_t)vertex, x, y, z);
        byteOffset += 0x0c;
        vertex++;
    }

    char matrixStorage[sizeof(Matrix)];
    Matrix *local = (Matrix *)(*canvas)->TransformGetLocal(u32(this, 0x14));
    Trail_MatrixSetTranslation(matrixStorage, local, ax, ay, az);
}
