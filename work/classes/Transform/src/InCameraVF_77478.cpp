#include "class.h"

namespace AbyssEngine {

int Transform::InCameraVF(AEMath::Matrix *matrix, Camera *camera) {
    if (camera == 0 || *(bool *)((char *)this + 0x17c) == false ||
        (*(int *)((char *)this + 0x3c) == 1 &&
         *(uint16_t *)(*(int *)(*(int *)((char *)this + 0x40)) + 2) == 0)) {
        return 1;
    }

    AEMath::Vector center;
    center.x = 0.0f;
    center.y = 0.0f;
    center.z = 0.0f;

    AEMath::Vector radiusVector;
    radiusVector.x = *(float *)((char *)this + 0xe0);
    radiusVector.y = radiusVector.x;
    radiusVector.z = radiusVector.x;

    AEMath::Vector rotated;
    if (matrix == 0) {
        AEMath::Vector transformed = AEMath::MatrixTransformVector(
            *(AEMath::Matrix *)this, *(AEMath::Vector *)((char *)this + 0xd4));
        center = transformed;
        rotated = AEMath::MatrixRotateVector(*(AEMath::Matrix *)this, radiusVector);
    } else {
        AEMath::Matrix combined = AEMath::operator*(*matrix, *(AEMath::Matrix *)this);
        AEMath::Vector transformed = AEMath::MatrixTransformVector(
            combined, *(AEMath::Vector *)((char *)this + 0xd4));
        center = transformed;
        combined = AEMath::operator*(*matrix, *(AEMath::Matrix *)this);
        rotated = AEMath::MatrixRotateVector(combined, radiusVector);
    }

    radiusVector = rotated;
    float x = radiusVector.x < 0.0f ? -radiusVector.x : radiusVector.x;
    float y = radiusVector.y < 0.0f ? -radiusVector.y : radiusVector.y;
    if (x < y) {
        x = y;
    }
    float z = radiusVector.z < 0.0f ? -radiusVector.z : radiusVector.z;
    if (x < z) {
        x = z;
    }

    return CameraIsSphereinViewFrustum(center, *(float *)((char *)this + 0xe4) * x, camera);
}

} // namespace AbyssEngine
