#include "gof2/engine/render/Trail.h"


extern "C" void Trail_MatrixSetTranslation(void *out, Matrix *matrix, float x, float y, float z);
extern "C" void Trail_MatrixTransformVector(Vector *out, const Matrix *matrix, const Vector *vector);

// ---- update_15c1fc.cpp ----
using AbyssEngine::AEMath::Vector;

void Trail::update(const Vector &a, const Vector &b)
{
    return update(a.x, a.y, a.z, b.x, b.y, b.z);
}

// ---- render_15c36c.cpp ----
void Trail::render()
{
    return Trail_renderTransform(*gTrailCanvasRender, u32(this, 0x14), 0);
}

// ---- translate_15c308.cpp ----
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

// ---- _Trail_15bf8c.cpp ----
Trail::~Trail()
{
    void *first = ptr(this, 0x18);
    if (first != 0) {
        operator delete(first);
    }
    ptr(this, 0x18) = 0;

    void *second = ptr(this, 0x1c);
    if (second != 0) {
        operator delete(second);
    }
    ptr(this, 0x1c) = 0;
}

// ---- update_15bfac.cpp ----
using AbyssEngine::AEMath::Matrix;


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

// ---- Trail_15bd58.cpp ----
Trail::Trail(int type, int segments)
{
    u32(this, 0x00) = 0;
    u32(this, 0x04) = 0;
    u32(this, 0x08) = 0;
    i32(this, 0x24) = segments;

    float uvSide = 0.0f;
    if ((type & ~3) != 4) {
        uvSide = 0.0f;
        if ((uint32_t)(type - 9) < 3) {
            uvSide = 1.0f;
        }
    }

    AbyssEngine::PaintCanvas *canvas = *gTrailCanvasCtor;
    i32(this, 0x0c) = 0x3c;
    canvas->MeshCreate((uint16_t)(segments * 2 + 2),
                       (uint16_t)(segments * 2),
                       0x13,
                       0x4e77,
                       u32(this, 0x10));

    for (int i = 2; i - 2 < segments * 2 - 2; i += 2) {
        uint16_t tri = (uint16_t)(i - 2);
        uint16_t odd = (uint16_t)(tri | 1);
        uint16_t current = (uint16_t)i;
        canvas->MeshSetTriangle(u32(this, 0x10), tri, tri, odd, current);
        canvas->MeshSetTriangle(u32(this, 0x10), odd, (uint16_t)(tri + 3), current, odd);
    }

    float uvScale = 0.0f;
    int point = 0;
    for (int i = 0; i < segments * 2 + 2; i += 2) {
        float u = (float)-(int)(((float)point / (float)(segments + 1)) * uvScale);
        canvas->MeshSetUv(u32(this, 0x10), (uint16_t)i, uvSide, u);
        canvas->MeshSetUv(u32(this, 0x10), (uint16_t)(i | 1), uvSide, u);
        point++;
        segments = i32(this, 0x24);
    }

    uint32_t count = (uint32_t)((segments * 2 + 2) * 3);
    u32(this, 0x20) = count;
    uint64_t bytes64 = (uint64_t)count * 4u;
    uint32_t bytes = (uint32_t)bytes64;
    if ((uint32_t)(bytes64 >> 32) != 0) {
        bytes = 0xffffffffu;
    }

    ptr(this, 0x18) = operator new(bytes);
    ptr(this, 0x1c) = operator new(bytes);
    canvas->TransformCreate(&u32(this, 0x14));
    canvas->TransformAddMeshId(u32(this, 0x14), u32(this, 0x10));
    changeType(type);
}

// ---- update_15c228.cpp ----
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;


void Trail::update(const Matrix &a, const Matrix &b, const Vector &v)
{
    char storage[sizeof(Vector) * 2];
    Trail_MatrixTransformVector((Vector *)(void *)(storage + sizeof(Vector)), &a, &v);
    Trail_MatrixTransformVector((Vector *)(void *)storage, &b, &v);
    update(((Vector *)(void *)(storage + sizeof(Vector)))->x,
           ((Vector *)(void *)(storage + sizeof(Vector)))->y,
           ((Vector *)(void *)(storage + sizeof(Vector)))->z,
           ((Vector *)(void *)storage)->x,
           ((Vector *)(void *)storage)->y,
           ((Vector *)(void *)storage)->z);
}

// ---- setWidth_15c184.cpp ----
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

// ---- changeType_15bef4.cpp ----
void Trail::changeType(int type)
{
    switch (type) {
    case 1:
    case 7:
    case 11:
        return Trail_transformSetColor(*gTrailCanvasType1, u32(this, 0x14), 0xff0000ffu);
    case 2:
    case 9:
        return Trail_transformSetColor(*gTrailCanvasType2, u32(this, 0x14), 0x00ff00ffu);
    case 3:
    case 6:
    case 10:
        return Trail_transformSetColor(*gTrailCanvasType3, u32(this, 0x14), 0xffff00ffu);
    case 5:
        return Trail_transformSetColor(*gTrailCanvasType5, u32(this, 0x14), 0x00ff0000u);
    case 8:
        return Trail_transformSetColor(*gTrailCanvasType8, u32(this, 0x14), 0xff4000ffu);
    default:
        return Trail_transformSetColor(*gTrailCanvasTypeDefault, u32(this, 0x14), 0xffffffffu);
    }
}

// ---- reset_15c290.cpp ----
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
