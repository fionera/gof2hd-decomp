#include "gof2/engine/render/Camera.h"

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

// AbyssEngine::Camera::shake(float, float, float)
// Applies a per-frame jitter offset to the camera position block (+0x0, three
// floats). PlayerEgo::shake() feeds in a random (dx,dy,dz) whose magnitude scales
// with the configured shake intensity; here we simply add it to the position.
void Camera::shake(float dx, float dy, float dz)
{
    float *pos = (float *)this->field_0x0;
    pos[0] += dx;
    pos[1] += dy;
    pos[2] += dz;
}

} // namespace AbyssEngine
