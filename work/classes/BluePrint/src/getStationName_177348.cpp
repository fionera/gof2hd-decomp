#include "class.h"

extern "C" void String_copy_ctor(void *out, void *src, bool);

// BluePrint::getStationName() -> String by value (sret in r0, this in r1).
extern "C" String12 BluePrint_getStationName(BluePrint *self)
{
    String12 r;
    String_copy_ctor(&r, (char *)self + 0x14, false);
    return r;
}
