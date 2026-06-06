#include "quaternion.h"

extern "C" float (*gof2_sinf)(float);
extern "C" float (*gof2_cosf)(float);

namespace AbyssEngine {

void Quaternion::Set(float x_angle, float y_angle, float z_angle) {
    float (*pSin)(float) = gof2_sinf;
    float fVar1 = pSin(z_angle * 0.5f);
    float fVar2 = pSin(y_angle * 0.5f);
    float (*pCos)(float) = gof2_cosf;
    float fVar3 = pSin(x_angle * 0.5f);
    float fVar4 = pCos(z_angle * 0.5f);
    float fVar5 = pCos(y_angle * 0.5f);
    float fVar6 = pCos(x_angle * 0.5f);
    x = fVar3 * fVar4 * fVar5 - fVar1 * fVar2 * fVar6;
    y = -(fVar2 * fVar4 * fVar6) + -(fVar3 * fVar1 * fVar5);
    z = fVar1 * fVar5 * fVar6 - fVar3 * fVar2 * fVar4;
    w = fVar4 * fVar5 * fVar6 + fVar1 * fVar2 * fVar3;
}

} // namespace AbyssEngine
