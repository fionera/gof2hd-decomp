#ifndef GOF2_CAMERA_H
#define GOF2_CAMERA_H
#include <cstdint>
#include "mathtypes.h"   // AEMath::Matrix (by value)

// Galaxy on Fire 2 -- AbyssEngine::Camera.
// The ctor default-constructs the projection Matrix at +0x0c, then installs the
// perspective projection via the free AbyssEngine::CameraSetPerspective().

namespace AbyssEngine {

class Camera {
public:
    float          position[3];          // camera position block (three floats)
    AEMath::Matrix projection;           // projection matrix

    Camera(float fov, float aspect, float nearPlane, float farPlane, float param5);
};

} // namespace AbyssEngine

#endif
