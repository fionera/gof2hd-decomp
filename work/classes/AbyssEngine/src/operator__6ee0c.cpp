#include "class.h"

// AbyssEngine::operator+(AbyssEngine::Quaternion const&, AbyssEngine::Quaternion const&)
// Component-wise quaternion addition (only .y/.w lanes materialized, see operator-).
namespace AbyssEngine {

extern "C" void _ZN11AbyssEngine10QuaternionC1Effff(Quaternion *self, float x, float y, float z,
                                                    float w);

Quaternion operator+(const Quaternion &a, const Quaternion &b)
{
    Quaternion r;
    float yv = *(const float *)((const char *)&a + 4) + *(const float *)((const char *)&b + 4);
    float wv = *(const float *)((const char *)&a + 0xc) + *(const float *)((const char *)&b + 0xc);
    _ZN11AbyssEngine10QuaternionC1Effff(&r, wv, 0.0f, yv, 0.0f);
    return r;
}

} // namespace AbyssEngine
