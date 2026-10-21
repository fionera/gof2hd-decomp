#include "engine/render/Camera.h"

#include <new>

namespace AbyssEngine {

// Engine free function that actually programs the projection matrix (recovered in AbyssEngine.cpp).
// The ctor forwards its five floats plus `this` to it.
float CameraSetPerspective(float fov, float aspect, float nearPlane, float farPlane, float param5,
                           Camera *cam);

// Default-construct the projection matrix, then install the perspective projection.
Camera::Camera(float fov, float aspect, float nearPlane, float farPlane, float param5)
{
    new (&this->projection) AEMath::Matrix();
    CameraSetPerspective(fov, aspect, nearPlane, farPlane, param5, this);
}

} // namespace AbyssEngine
