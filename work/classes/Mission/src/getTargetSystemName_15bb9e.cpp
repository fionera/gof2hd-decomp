#include "class.h"

extern "C" void String_copy_ctor(void *out, void *src, bool);
struct RetStr { uint32_t a, b, c; };

extern "C" RetStr Mission_getTargetSystemName(Mission *self) {
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x4c, false);
    return r;
}
