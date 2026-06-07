#include "class.h"

extern "C" void String_copy_ctor(void *out, void *src, bool);
struct RetStr { uint32_t a, b, c; };

extern "C" RetStr Agent_getWingmanName(Agent *self, int idx)
{
    void *src;
    if (idx == 1) {
        src = F<void *>(self, 0xc);
    } else if (idx == 0) {
        src = self;
    } else {
        src = F<void *>(self, 0x10);
    }
    RetStr r;
    String_copy_ctor(&r, src, false);
    return r;
}
