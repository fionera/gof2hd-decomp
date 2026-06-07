#include "class.h"

extern "C" float WantedWindow_getRelativeScrollHeight(WantedWindow *self)
{
    long long pair = F<long long>(self, 0xa4);
    int content = (int)pair;
    int visible = (int)((unsigned long long)pair >> 32);
    if (content < visible) {
        union { uint32_t u; float f; } c;
        c.u = 0x4605e009u;
        return c.f;
    }
    int scroll = F<int>(self, 0x84);
    int num;
    if (scroll >= 1) {
        num = visible - scroll;
    } else if (scroll >= visible - content) {
        num = visible;
    } else {
        num = scroll + content;
    }
    return (float)num / (float)content;
}
