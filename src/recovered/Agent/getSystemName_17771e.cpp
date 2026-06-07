#include "class.h"

extern "C" void String_copy_ctor(void *out, void *src, bool);
struct RetStr { uint32_t a, b, c; };

extern "C" RetStr Agent_getSystemName(Agent *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x18, false);
    return r;
}
