#include "class.h"

// AbyssEngine::String operations.
extern "C" void *String_default_ctor(void *self);                 // 0x6efbc -> this
extern "C" void String_copy_ctor(void *out, const void *src, bool); // 0x6f028
extern "C" void String_assign(void *dst, const void *rhs);        // 0x6f2b0
extern "C" void String_dtor(void *self);                          // 0x6ee30

// SolarSystem::SolarSystem(int, String, int, bool, int*6, int*, Array*, Array*, Array*)
// The String arg has a non-trivial copy ctor/dtor, so the ABI passes it by invisible
// reference (a pointer in r2). The on-stack `name` build uses a stack-protector canary.
//
// NOTE: near-match only. The field assignments and String build are semantically
// correct and the canary is emitted (char[] temp), but byte-exact is resistant:
// clang -Oz colors the param/this+0xc registers differently and coalesces the
// consecutive stacked-arg loads/field stores into ldmia/stmia + strd pairs, whereas
// the target keeps them as discrete ldr/str. This is dominated by register-allocation
// and frame-layout choices not reachable from source form.
extern "C" SolarSystem *
SolarSystem_ctor(SolarSystem *self, int p1, const String12 &p2, int p3, bool p4,
                 int p5, int p6, int p7, int p8, int p9, int p10,
                 int *p11, void *p12, void *p13, void *p14)
{
    String_default_ctor((char *)self + 0xc);
    F<int>(self, 0x18) = p1;
    char tmp[12];
    String_copy_ctor(tmp, &p2, false);
    String_assign((char *)self + 0xc, tmp);
    String_dtor(tmp);
    u8(self, 0x44) = p4;
    F<int>(self, 0x1c) = p3;
    F<int>(self, 0x20) = p5;
    F<int>(self, 0x24) = p6;
    F<int>(self, 0x28) = p7;
    F<int>(self, 0x2c) = p8;
    F<int>(self, 0x30) = p9;
    F<int>(self, 0x34) = p10;
    F<int>(self, 0x00) = p11[0];
    F<int>(self, 0x04) = p11[1];
    int v2 = p11[2];
    F<void *>(self, 0x38) = p12;
    F<int>(self, 0x08) = v2;
    F<void *>(self, 0x3c) = p14;
    F<void *>(self, 0x40) = p13;
    return self;
}
