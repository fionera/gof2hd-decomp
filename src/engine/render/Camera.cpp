#include "engine/render/Camera.h"

namespace AbyssEngine {

// Engine free function that actually programs the projection matrix (recovered in AbyssEngine.cpp).
// The ctor forwards its five floats plus `this` to it.
float CameraSetPerspective(float fov, float aspect, float nearPlane, float farPlane, float param5,
                           Camera *cam);

// AbyssEngine::Camera::Camera(float, float, float, float, float)
// Default-constructs the projection Matrix at +0x0c, then sets up the perspective
// via the engine free function (there is no Camera::SetPerspective method).
Camera::Camera(float fov, float aspect, float nearPlane, float farPlane, float param5)
{
    CameraSetPerspective(fov, aspect, nearPlane, farPlane, param5, this);
}

} // namespace AbyssEngine
