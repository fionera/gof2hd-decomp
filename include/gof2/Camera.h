#ifndef GOF2_CAMERA_H
#define GOF2_CAMERA_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- AbyssEngine::Camera (Android libgof2hdaa.so, armv7 Thumb).
// The ctor default-constructs an AEMath::Matrix at +0x0c, then installs the
// perspective projection via SetPerspective(fov, aspect, near, far).

namespace AbyssEngine {

class Camera {
public:
    byte           field_0x0[0xc];      // +0x0  (camera header / position+orientation block)
    AEMath::Matrix projection;           // +0xc  projection matrix

    Camera(float fov, float aspect, float nearPlane, float farPlane, float param5);
    void SetPerspective(float fov, float aspect, float nearPlane, float farPlane);
    void shake(float dx, float dy, float dz);
};

} // namespace AbyssEngine

#endif
