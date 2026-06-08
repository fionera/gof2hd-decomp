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
    self->f_18 = p1;
    char tmp[12];
    String_copy_ctor(tmp, &p2, false);
    String_assign((char *)self + 0xc, tmp);
    String_dtor(tmp);
    self->f_44 = p4;
    self->f_1c = p3;
    self->f_20 = p5;
    self->f_24 = p6;
    self->f_28 = p7;
    self->f_2c = p8;
    self->f_30 = p9;
    self->f_34 = p10;
    self->f_0 = p11[0];
    self->f_4 = p11[1];
    int v2 = p11[2];
    self->f_38 = p12;
    self->f_8 = v2;
    self->f_3c = p14;
    self->f_40 = p13;
    return self;
}
