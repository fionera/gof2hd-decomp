#include "engine/render/Camera.h"
#include "engine/core/AbyssEngine.h"
#include "engine/file/AEFile.h"
#include "engine/core/GameText.h"
#include "engine/file/FileInterfaceAndroid.h"
#include "engine/math/AEMath.h"
#include "engine/math/Transform.h"
#include "game/core/String.h"
#include "engine/render/Mesh.h"
#include "engine/render/Engine.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/FBOContainer.h"
#include "engine/math/Quaternion.h"
#include <GLES2/gl2.h>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <cstring>

// Camera::Camera is inline in Camera.h (the original emits it weak in PaintCanvas.cpp).

namespace AbyssEngine {
    int CameraIsPointinViewFrustum(const Vector &point, Matrix *extra, Camera *cam) {
        if (Engine::vfc == 0)
            return 1;

        const Matrix &view = cam->projection;
        // Point in camera space (computed but unused).
        Vector camPoint = {0.0f, 0.0f, 0.0f};
        if (extra != 0) {
            Matrix combined = view;
            combined *= *extra;
            camPoint = AEMath::MatrixInverseTransformVector(combined, point);
        } else {
            camPoint = AEMath::MatrixInverseTransformVector(view, point);
        }

        Vector toPoint = point - AEMath::MatrixGetPosition(view);
        float depth = AEMath::VectorDot(toPoint, AEMath::VectorNormalize(-AEMath::MatrixGetDir(view)));
        if (depth > cam->position[2] || depth < cam->position[1])
            return 0;

        float vertical = AEMath::VectorDot(toPoint, AEMath::VectorNormalize(AEMath::MatrixGetUp(view)));
        float vLimit = depth * cam->frustumTanHalfFov;
        if (vertical > vLimit || vertical < -(depth * cam->frustumTanHalfFov))
            return 0;

        float horizontal = AEMath::VectorDot(toPoint, AEMath::VectorNormalize(AEMath::MatrixGetRight(view)));
        return !(horizontal > vLimit * cam->frustumAspect) && !(horizontal < -(vLimit * cam->frustumAspect));
    }
}

namespace AbyssEngine {
    namespace AEMath {
    }

    void CameraSetPerspective(float fov, float aspectNum, float aspectDen, float near, Camera *cam) {
        if (cam != 0) {
            float *f = (float *) cam;
            f[1] = fov;
            f[2] = aspectNum;

            float s = AbyssEngine::AEMath::Sinf(f[0] * 0.5f);
            float c = AbyssEngine::AEMath::Cosf(f[0] * 0.5f);
            float scale = s / c;
            f[0x12] = scale;
            f[0x13] = (aspectDen / near) * scale;
            f[0x14] = aspectDen / near;

            float at = AbyssEngine::AEMath::ATanf((aspectDen / near) * scale);
            f[0x15] = 1.0f / AbyssEngine::AEMath::Cosf(at);
        }
    }
}

namespace AbyssEngine {
    namespace AEMath {
    }

    void CameraSetPerspective(float p1, float aspectNum, float fov, float aspectDen, float near,
                              Camera *cam) {
        if (cam != 0) {
            float *f = (float *) cam;
            f[0] = p1;
            f[1] = aspectNum;
            f[2] = fov;

            float half = p1 * 0.5f;
            float s = AbyssEngine::AEMath::Sinf(half);
            float c = AbyssEngine::AEMath::Cosf(half);
            f[0x12] = s / c;
            f[0x13] = (aspectDen / near) * (s / c);
            f[0x14] = aspectDen / near;

            f[0x16] = 1.0f / AbyssEngine::AEMath::Cosf(half);
            f[0x15] = 1.0f / AbyssEngine::AEMath::Cosf(AbyssEngine::AEMath::ATanf(f[0x12] * f[0x14]));
        }
    }
}

namespace AbyssEngine {
    bool CameraIsSphereinViewFrustum(const Vector &center, float radius, Matrix *extra, Camera *cam) {
        if (!(radius != 0.0f && Engine::vfc != 0))
            return 1;

        const Matrix &view = cam->projection;
        // Point in camera space (computed but unused).
        Vector camPoint = {0.0f, 0.0f, 0.0f};
        if (extra != 0) {
            Matrix combined = view;
            combined *= *extra;
            camPoint = AEMath::MatrixInverseTransformVector(combined, center);
        } else {
            camPoint = AEMath::MatrixInverseTransformVector(view, center);
        }

        Vector toCenter = center - AEMath::MatrixGetPosition(view);
        float depth = AEMath::VectorDot(toCenter, AEMath::VectorNormalize(-AEMath::MatrixGetDir(view)));
        if (depth > cam->position[2] + radius)
            return 0;
        if (depth < cam->position[1] - radius)
            return 0;

        float horizontal = AEMath::VectorDot(toCenter, AEMath::VectorNormalize(AEMath::MatrixGetRight(view)));
        float hLimit = depth * cam->frustumTanHalfFov * cam->frustumAspect;
        float hPad = cam->frustumInvCosX * radius;
        if (horizontal > hLimit + hPad)
            return 0;
        if (horizontal < -hLimit - hPad)
            return 0;

        float vertical = AEMath::VectorDot(toCenter, AEMath::VectorNormalize(AEMath::MatrixGetUp(view)));
        float vLimit = depth * cam->frustumTanHalfFov;
        float vPad = cam->frustumInvCosY * radius;
        if (vertical > vLimit + vPad || vertical < -vLimit - vPad)
            return 0;
        return 1;
    }
}
