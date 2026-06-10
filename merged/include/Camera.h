#ifndef WORK_CLASSES_CAMERA_SRC_CLASS_H
#define WORK_CLASSES_CAMERA_SRC_CLASS_H

// Galaxy on Fire 2 -- AbyssEngine::Camera (Android libgof2hdaa.so, armv7 Thumb).
// The ctor default-constructs an AEMath::Matrix at +0x0c, then installs the
// perspective projection via SetPerspective(fov, aspect, near, far).

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {

namespace AEMath {
struct Matrix;
}

struct Camera {
    Camera(float fov, float aspect, float nearPlane, float farPlane, float param5);
    void SetPerspective(float fov, float aspect, float nearPlane, float farPlane);
};

} // namespace AbyssEngine

extern "C" {
// AEMath::Matrix default ctor (mangled).
void _ZN11AbyssEngine6AEMath6MatrixC1Ev(AbyssEngine::AEMath::Matrix *self);
// Camera::SetPerspective (mangled).
void _ZN11AbyssEngine6Camera14SetPerspectiveEffff(
    AbyssEngine::Camera *self, float fov, float aspect, float nearPlane, float farPlane);
}

#endif
