#include "class.h"

// AbyssEngine::CameraSetPerspective(float, float aspectNum, float fov, float aspectDen,
//                                   float near, AbyssEngine::Camera*)
// Six-argument overload: additionally precomputes slot [0x16]. Cosf is reached through a
// runtime function-pointer in the shipped code; modeled here as the same AEMath::Cosf.
namespace AbyssEngine {

namespace AEMath {
extern "C" float _ZN11AbyssEngine6AEMath4SinfEf(float);
extern "C" float _ZN11AbyssEngine6AEMath4CosfEf(float);
extern "C" float _ZN11AbyssEngine6AEMath5ATanfEf(float);
}

float CameraSetPerspective(float p1, float aspectNum, float fov, float aspectDen, float near,
                           Camera *cam)
{
    float ret = p1;
    if (cam != 0) {
        float *f = (float *)cam;
        f[0] = fov;
        f[1] = aspectNum;
        f[2] = fov;

        float s = AEMath::_ZN11AbyssEngine6AEMath4SinfEf(fov);
        float c = AEMath::_ZN11AbyssEngine6AEMath4CosfEf(f[0] * 0.5f);
        f[0x12] = s / c;
        f[0x13] = (aspectDen / near) * (s / c);
        f[0x14] = aspectDen / near;

        float c2 = AEMath::_ZN11AbyssEngine6AEMath4CosfEf(f[0] * 0.5f);
        f[0x16] = 1.0f / c2;

        AEMath::_ZN11AbyssEngine6AEMath5ATanfEf(f[0x12] * f[0x14]);
        float c3 = AEMath::_ZN11AbyssEngine6AEMath4CosfEf(f[0] * 0.5f);
        ret = 1.0f / c3;
        f[0x15] = ret;
    }
    return ret;
}

} // namespace AbyssEngine
