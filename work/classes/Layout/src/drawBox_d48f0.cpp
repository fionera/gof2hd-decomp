#include "class.h"

extern "C" void String_copy_ctor(void *out, const void *src, bool);  // 0x6f028
extern "C" void String_dtor(void *s);                                // 0x6ee30
extern "C" void Layout_drawBoxImpl(Layout *self, int p2, int p3, int p4,
                                   int p5, int p6, void *str, int flag);  // 0x74e84

// Layout::drawBox(int, int, int, int, int, String) -> drawBox(..., copy, 1)
extern "C" void Layout_drawBox6(Layout *self, int p2, int p3, int p4,
                                int p5, int p6, const void *str) {
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    String_copy_ctor(tmp, str, false);
    Layout_drawBoxImpl(self, p2, p3, p4, p5, p6, tmp, 1);
    String_dtor(tmp);
}
