#include "class.h"

// AbyssEngine::CameraSetPerspective(float fov, float aspectNum, float aspectDen, float near,
//                                   AbyssEngine::Camera*)
// Store the field-of-view/aspect into the Camera and precompute the projection scale terms
// (slots [0x12]..[0x15] of the float-view of the Camera). Mirrors the shipped trig sequence.
namespace AbyssEngine {

namespace AEMath {
extern "C" float _ZN11AbyssEngine6AEMath4SinfEf(float);
extern "C" float _ZN11AbyssEngine6AEMath4CosfEf(float);
extern "C" float _ZN11AbyssEngine6AEMath5ATanfEf(float);
}

float CameraSetPerspective(float fov, float aspectNum, float aspectDen, float near, Camera *cam)
{
    float ret = fov;
    if (cam != 0) {
        float *f = (float *)cam;
        f[1] = near;
        f[2] = fov;

        float s = AEMath::_ZN11AbyssEngine6AEMath4SinfEf(fov);
        float c = AEMath::_ZN11AbyssEngine6AEMath4CosfEf(f[0] * 0.5f);
        float scale = s / c;
        f[0x12] = scale;
        f[0x13] = (aspectNum / aspectDen) * scale;
        f[0x14] = aspectNum / aspectDen;

        float at = AEMath::_ZN11AbyssEngine6AEMath5ATanfEf(scale);
        float ca = AEMath::_ZN11AbyssEngine6AEMath4CosfEf(at);
        ret = 1.0f / ca;
        f[0x15] = ret;
    }
    return ret;
}

} // namespace AbyssEngine
