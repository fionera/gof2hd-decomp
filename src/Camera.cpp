#include "gof2/Camera.h"




// ---- Camera_7b724.cpp ----
namespace AbyssEngine {

// Engine free function that actually programs the projection matrix (recovered in AbyssEngine.cpp).
// Camera::SetPerspective is a thin thunk onto it.
float CameraSetPerspective(float fov, float aspectNum, float aspectDen, float near, Camera *cam);

// AbyssEngine::Camera::Camera(float, float, float, float, float)
// Default-constructs the projection Matrix at +0x0c, then sets up the perspective.
Camera::Camera(float fov, float aspect, float nearPlane, float farPlane, float param5)
{
    this->projection.initIdentity();
    (void)param5;
    SetPerspective(fov, aspect, nearPlane, farPlane);
}

void Camera::SetPerspective(float fov, float aspect, float nearPlane, float farPlane)
{
    CameraSetPerspective(fov, aspect, nearPlane, farPlane, this);
}

} // namespace AbyssEngine
