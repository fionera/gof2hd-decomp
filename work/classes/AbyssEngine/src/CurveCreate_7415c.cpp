#include "class.h"

void *operator new(size_t_ size);
void *operator new[](size_t_ size);
extern "C" void *__aeabi_memcpy4(void *dst, const void *src, size_t_ n);

// AbyssEngine::CurveCreate(void**, unsigned short, AbyssEngine::Curve**)
// Allocate a Curve {count@0, void** data@4}, copy `count` 4-byte entries from the source.
namespace AbyssEngine {

int CurveCreate(void **src, unsigned short count, Curve **out)
{
    Curve *curve = (Curve *)operator new(8);
    u32(curve, 0x4) = 0;
    *out = curve;
    u16(curve, 0x0) = count;

    void *data = operator new[]((unsigned int)count << 2);
    pp(curve, 0x4) = data;
    __aeabi_memcpy4(data, src, (unsigned int)count << 2);
    return 1;
}

} // namespace AbyssEngine
