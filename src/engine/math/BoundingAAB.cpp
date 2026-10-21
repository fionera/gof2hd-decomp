#include "engine/math/BoundingAAB.h"
#include "engine/math/BoundingVolume.h"
#include "engine/math/AEMath.h"
#include <cmath>

int BoundingAAB::outerCollide(float x, float y, float z) {
    float centerX = this->position.x + this->offset.x;
    float extentX = this->halfExtents.x;
    if (!(centerX - extentX < x)) {
        return 0;
    }
    if (!(x < centerX + extentX)) {
        return 0;
    }

    float centerY = this->position.y + this->offset.y;
    float extentY = this->halfExtents.y;
    if (!(centerY - extentY < y)) {
        return 0;
    }
    if (!(y < centerY + extentY)) {
        return 0;
    }

    float centerZ = this->position.z + this->offset.z;
    float extentZ = this->halfExtents.z;
    if (!(centerZ - extentZ < z)) {
        return 0;
    }
    if (z < centerZ + extentZ) {
        return 1;
    }
    return 0;
}

// Moves the point onto the nearest of the six faces.
Vector BoundingAAB::projectCollisionOnSurface(const Vector &point) {
    float centerX = this->position.x + this->offset.x;
    float centerY = this->position.y + this->offset.y;
    float centerZ = this->position.z + this->offset.z;
    Vector faceOffsets[6] = {
            {point.x - (centerX + this->halfExtents.x), 0.0f, 0.0f},
            {point.x - (centerX - this->halfExtents.x), 0.0f, 0.0f},
            {0.0f, point.y - (centerY + this->halfExtents.y), 0.0f},
            {0.0f, point.y - (centerY - this->halfExtents.y), 0.0f},
            {0.0f, 0.0f, point.z - (centerZ + this->halfExtents.z)},
            {0.0f, 0.0f, point.z - (centerZ - this->halfExtents.z)},
    };
    float distances[6];
    distances[0] = AbyssEngine::AEMath::Absf(faceOffsets[0].x);
    distances[1] = AbyssEngine::AEMath::Absf(faceOffsets[1].x);
    distances[2] = AbyssEngine::AEMath::Absf(faceOffsets[2].y);
    distances[3] = AbyssEngine::AEMath::Absf(faceOffsets[3].y);
    distances[4] = AbyssEngine::AEMath::Absf(faceOffsets[4].z);
    distances[5] = AbyssEngine::AEMath::Absf(faceOffsets[5].z);

    float closest = distances[0];
    int closestIndex = 0;
    for (int i = 1; i != 6; i++) {
        if (distances[i] < closest) {
            closest = distances[i];
            closestIndex = i;
        }
    }
    return point - faceOffsets[closestIndex];
}

BoundingAAB::BoundingAAB(float x, float y, float z, float ex, float ey, float ez,
                         float width, float height, float depth)
    : BoundingVolume(x, y, z, ex, ey, ez) {
    float halfWidth = width * 0.5f;
    float extentX = width * -0.5f;
    if (0.0f < halfWidth) {
        extentX = halfWidth;
    }

    float halfHeight = height * 0.5f;
    float extentY = height * -0.5f;
    if (0.0f < halfHeight) {
        extentY = halfHeight;
    }

    float halfDepth = depth * 0.5f;
    float extentZ = depth * -0.5f;
    if (0.0f < halfDepth) {
        extentZ = halfDepth;
    }

    this->halfExtents.x = extentX;
    this->halfExtents.y = extentY;
    this->halfExtents.z = extentZ;
}

BoundingAAB::~BoundingAAB() {
}

Vector BoundingAAB::getCollisionNormal(const Vector &) {
    Vector out;
    out.x = 0.0f;
    out.y = 0.0f;
    out.z = 0.0f;
    return out;
}

int BoundingAAB::collide(float x, float y, float z) {
    if (this->outerCollide(x, y, z) == 0) {
        return 0;
    }
    return BoundingVolume::collide(x, y, z);
}

void BoundingAAB::update(float x, float y, float z) {
    BoundingVolume::update(x, y, z);
}
