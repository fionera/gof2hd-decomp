#ifndef GOF2_CAMERA_H
#define GOF2_CAMERA_H
#include <cstdint>

#include "engine/math/Matrix.h"

#include "engine/math/AEMath.h"

namespace AbyssEngine {
    class Camera {
    public:
        float position[3];
        AEMath::Matrix projection;

        float frustumTanHalfFov;
        float field_0x4c;
        float frustumAspect;
        float frustumInvCosX;
        float frustumInvCosY;

        Camera(float fov, float aspect, float nearPlane, float farPlane, float param5);
    };

#if __SIZEOF_POINTER__ == 4
    static_assert(__builtin_offsetof(Camera, projection) == 0x0c, "Camera::projection offset");
    static_assert(__builtin_offsetof(Camera, frustumTanHalfFov) == 0x48, "Camera::frustumTanHalfFov offset");
    static_assert(__builtin_offsetof(Camera, frustumInvCosY) == 0x58, "Camera::frustumInvCosY offset");
    static_assert(sizeof(Camera) == 0x5c, "Camera size");
#endif
}

#endif
