#include "Camera.h"




// ---- Camera_7b724.cpp ----
namespace AbyssEngine {

// AbyssEngine::Camera::Camera(float, float, float, float, float)
// Default-constructs the projection Matrix at +0x0c, then sets up the perspective.
Camera::Camera(float fov, float aspect, float nearPlane, float farPlane, float param5)
{
    _ZN11AbyssEngine6AEMath6MatrixC1Ev((AEMath::Matrix *)((char *)this + 0xc));
    (void)param5;
    SetPerspective(fov, aspect, nearPlane, farPlane);
}

void Camera::SetPerspective(float fov, float aspect, float nearPlane, float farPlane)
{
    _ZN11AbyssEngine6Camera14SetPerspectiveEffff(this, fov, aspect, nearPlane, farPlane);
}

} // namespace AbyssEngine
