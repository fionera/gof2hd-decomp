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
        float field_0x5c;

        Camera(float fov, float aspect, float nearPlane, float farPlane, float param5);
    };
}

#endif
