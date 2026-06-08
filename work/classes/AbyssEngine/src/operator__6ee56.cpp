#include "class.h"

// AbyssEngine::operator*(AbyssEngine::Quaternion const&, float)
// Quaternion scaled by a scalar (only .y/.w lanes materialized, see operator-).
namespace AbyssEngine {

extern "C" void _ZN11AbyssEngine10QuaternionC1Effff(Quaternion *self, float x, float y, float z,
                                                    float w);

Quaternion operator*(const Quaternion &a, float s)
{
    Quaternion r;
    float yv = *(const float *)((const char *)&a + 4) * s;
    float wv = *(const float *)((const char *)&a + 0xc) * s;
    _ZN11AbyssEngine10QuaternionC1Effff(&r, wv, 0.0f, yv, 0.0f);
    return r;
}

} // namespace AbyssEngine
