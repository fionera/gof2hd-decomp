#include "class.h"

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
