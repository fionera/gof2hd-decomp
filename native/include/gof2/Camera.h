#ifndef GOF2_CAMERA_H
#define GOF2_CAMERA_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- AbyssEngine::Camera (Android libgof2hdaa.so, armv7 Thumb).
// The ctor default-constructs an AEMath::Matrix at +0x0c, then installs the
// perspective projection via SetPerspective(fov, aspect, near, far).


namespace AbyssEngine {

namespace AEMath {
struct Matrix;
}



} // namespace AbyssEngine

extern "C" {
// AEMath::Matrix default ctor (mangled).
void _ZN11AbyssEngine6AEMath6MatrixC1Ev(AbyssEngine::AEMath::Matrix *self);
// Camera::SetPerspective (mangled).
void _ZN11AbyssEngine6Camera14SetPerspectiveEffff(
    AbyssEngine::Camera *self, float fov, float aspect, float nearPlane, float farPlane);
}

struct Camera { void* _opaque; };  // no offset accesses observed
#endif
