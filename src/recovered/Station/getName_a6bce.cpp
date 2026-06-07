#include "class.h"

extern "C" void String_copy_ctor(void *out, void *src, bool);
struct RetStr { uint32_t a, b, c; };

// Station::getName() -> String by value (sret in r0, this in r1).
extern "C" RetStr Station_getName(Station *self)
{
    RetStr r;
    String_copy_ctor(&r, self, false);
    return r;
}
