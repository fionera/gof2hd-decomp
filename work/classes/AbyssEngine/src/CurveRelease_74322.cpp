#include "class.h"

void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;

// AbyssEngine::CurveRelease(AbyssEngine::Curve**)
// Free each keyframe object (tag byte 1/2/3 -> single delete), then the entry array and the
// Curve itself.
namespace AbyssEngine {

void CurveRelease(Curve **slot)
{
    Curve *curve = (Curve *)*slot;
    if (curve == 0)
        return;

    void *data = 0;
    for (unsigned int i = 0; (data = pp(curve, 0x4)), i < (unsigned int)u16(curve, 0x0); ++i) {
        char *entry = *(char **)((char *)data + i * 4);
        char tag = entry[0];
        if (tag == 3 || tag == 2)
            operator delete(entry);
        else if (tag == 1)
            operator delete(entry);
        curve = (Curve *)*slot;
    }

    if (data != 0)
        operator delete[](data);
    curve = (Curve *)*slot;
    pp(curve, 0x4) = 0;

    if (*slot != 0)
        operator delete((void *)*slot);
    *slot = 0;
}

} // namespace AbyssEngine
