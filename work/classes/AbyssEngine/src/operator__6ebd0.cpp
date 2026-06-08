#include "class.h"

// AbyssEngine::operator-(AbyssEngine::Quaternion const&, AbyssEngine::Quaternion const&)
// Component-wise quaternion subtraction. The shipped code only materializes the .y (+4)
// and .w (+0xc) lanes (the .x/.z incoming float regs are passed through untouched), so we
// reproduce that by constructing the result through the 4-float Quaternion ctor.
namespace AbyssEngine {

extern "C" void _ZN11AbyssEngine10QuaternionC1Effff(Quaternion *self, float x, float y, float z,
                                                    float w);

Quaternion operator-(const Quaternion &a, const Quaternion &b)
{
    Quaternion r;
    float yv = *(const float *)((const char *)&a + 4) - *(const float *)((const char *)&b + 4);
    float wv = *(const float *)((const char *)&a + 0xc) - *(const float *)((const char *)&b + 0xc);
    _ZN11AbyssEngine10QuaternionC1Effff(&r, wv, 0.0f, yv, 0.0f);
    return r;
}

} // namespace AbyssEngine
