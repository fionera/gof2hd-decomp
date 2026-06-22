#ifndef GOF2_CAMERA_H
#define GOF2_CAMERA_H
#include <cstdint>

#include "engine/math/Matrix.h"

namespace AbyssEngine {
    class Camera {
    public:
        float position[3]; // camera position block (three floats)
        AEMath::Matrix projection; // projection matrix

        Camera(float fov, float aspect, float nearPlane, float farPlane, float param5);
    };
} // namespace AbyssEngine

#endif
