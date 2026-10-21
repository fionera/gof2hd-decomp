#include "engine/render/Trail.h"
#include "game/core/Globals.h"
#include "engine/render/PaintCanvas.h"

namespace AEMath = AbyssEngine::AEMath;

Trail::Trail(int type, int segments) {
    this->sourceX = 0;
    this->sourceY = 0;
    this->sourceZ = 0;
    this->segments = segments;

    float uvSide = 0.0f;
    if ((type & ~3) != 4) {
        uvSide = 0.0f;
        if ((uint32_t)(type - 9) < 3) {
            uvSide = 1.0f;
        }
    }

    this->width = 0x3c;
    Globals::Canvas->MeshCreate((uint16_t)(segments * 2 + 2),
                       (uint16_t)(segments * 2),
                       0x13,
                       0x4e77,
                       this->meshId);

    // Two triangles per segment between the vertex pairs (i, i+1) and (i+2, i+3).
    for (int i = 0; i < this->segments * 2 - 2; i += 2) {
        Globals::Canvas->MeshSetTriangle(this->meshId, (uint16_t) i, (uint16_t) i, (uint16_t) (i | 1),
                                         (uint16_t) (i + 2));
        Globals::Canvas->MeshSetTriangle(this->meshId, (uint16_t) (i | 1), (uint16_t) (i + 3), (uint16_t) (i + 2),
                                         (uint16_t) (i | 1));
    }

    float uvScale = 0.0f;
    for (int point = 0, i = 0; i < this->segments * 2 + 2; i += 2, point++) {
        float u = (float) -(int) (((float) point / (float) (this->segments + 1)) * uvScale);
        Globals::Canvas->MeshSetUv(this->meshId, (uint16_t) i, uvSide, u);
        Globals::Canvas->MeshSetUv(this->meshId, (uint16_t)(i | 1), uvSide, u);
    }

    int count = (this->segments * 2 + 2) * 3;
    this->pointCount = count;

    this->points = new int[count];
    this->relativePoints = new int[count];
    Globals::Canvas->TransformCreate(this->transformId);
    Globals::Canvas->TransformAddMeshId(this->transformId, this->meshId);
    changeType(type);
}

void Trail::changeType(int type) {
    switch (type) {
        case 1:
        case 7:
        case 11:
            Globals::Canvas->TransformSetColor(this->transformId, 0xff0000ffu);
            break;
        case 2:
        case 9:
            Globals::Canvas->TransformSetColor(this->transformId, 0x00ff00ffu);
            break;
        default:
            Globals::Canvas->TransformSetColor(this->transformId, 0xffffffffu);
            break;
        case 5:
            Globals::Canvas->TransformSetColor(this->transformId, 0x00ff0000u);
            break;
        case 3:
        case 6:
        case 10:
            Globals::Canvas->TransformSetColor(this->transformId, 0xffff00ffu);
            break;
        case 8:
            Globals::Canvas->TransformSetColor(this->transformId, 0xff4000ffu);
            break;
    }
}

Trail::~Trail() {
    delete[] this->points;
    this->points = nullptr;

    delete[] this->relativePoints;
    this->relativePoints = nullptr;
}

void Trail::update(float ax, float ay, float az, float bx, float by, float bz) {
    // The newest segment: a pair of points width apart around a and around b.
    int *points = this->points;
    points[0] = (int) (ax - (float) this->width);
    points[1] = (int) ay;
    points[2] = (int) az;
    points[3] = (int) ((float) this->width + ax);
    points[4] = (int) ay;
    points[5] = (int) az;
    points[6] = (int) (bx - (float) this->width);
    points[7] = (int) by;
    points[8] = (int) bz;
    points[9] = (int) ((float) this->width + bx);
    points[10] = (int) by;
    points[11] = (int) bz;

    // Age the older segments by one slot.
    for (int i = this->pointCount - 1; i > 10; i -= 6) {
        points[i] = points[i - 6];
        points[i - 1] = points[i - 7];
        points[i - 2] = points[i - 8];
        points[i - 3] = points[i - 9];
        points[i - 4] = points[i - 10];
        points[i - 5] = points[i - 11];
    }

    // The mesh is placed at a, its vertices are relative to it.
    for (int i = 0; i < this->pointCount; i += 3) {
        this->relativePoints[i] = (int) ((float) points[i] - ax);
        this->relativePoints[i + 1] = (int) ((float) points[i + 1] - ay);
        this->relativePoints[i + 2] = (int) ((float) points[i + 2] - az);
    }
    for (int vertex = 0; vertex < (this->segments + 1) * 2; ++vertex) {
        PaintCanvas *canvas = Globals::Canvas;
        int *point = &this->relativePoints[vertex * 3];
        canvas->MeshSetPoint(this->meshId, (uint16_t) vertex, (float) point[0], (float) point[1], (float) point[2]);
    }
    AEMath::MatrixSetTranslation(*static_cast<Matrix *>(Globals::Canvas->TransformGetLocal(this->transformId)),
                                 ax, ay, az);
}

void Trail::setWidth(int width) {
    int delta = this->width - width;
    PaintCanvas **canvas = &Globals::Canvas;
    for (int vertex = 0; vertex < (this->segments + 1) * 2; ++vertex) {
        int *point = this->points + vertex * 3;
        int x = point[0] + delta;
        point[0] = x;
        (*canvas)->MeshSetPoint(this->meshId, (uint16_t) vertex, (float) x, (float) point[1], (float) point[2]);
    }

    this->width = width;
}

void Trail::update(const Vector &a, const Vector &b) {
    update(a.x, a.y, a.z, b.x, b.y, b.z);
}

void Trail::update(const Matrix &a, const Matrix &b, const Vector &v) {
    Vector va = AEMath::MatrixTransformVector(a, v);
    Vector vb = AEMath::MatrixTransformVector(b, v);
    update(va.x, va.y, va.z, vb.x, vb.y, vb.z);
}

void Trail::reset(Vector value) {
    int x = (int) value.x;
    int y = (int) value.y;
    int z = (int) value.z;

    for (int i = 0; i < this->pointCount; i += 3) {
        int *point = this->points + i;
        point[0] = x;
        point[1] = y;
        point[2] = z;
    }

    update(value, value);
}

void Trail::translate(const Vector &delta) {
    float dx = delta.x;
    float dy = delta.y;
    float dz = delta.z;

    for (int i = 0; i < this->pointCount; i += 3) {
        int *point = this->points + i;
        point[0] = (int) (dx + (float) point[0]);
        point[1] = (int) (dy + (float) point[1]);
        point[2] = (int) (dz + (float) point[2]);
    }
}

void Trail::render() {
    Globals::Canvas->DrawTransform(this->transformId, nullptr);
}
