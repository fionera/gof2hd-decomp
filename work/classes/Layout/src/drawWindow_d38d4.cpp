#include "class.h"

extern "C" void String_copy_ctor(void *out, const void *src, bool);  // 0x6f028
extern "C" void String_dtor(void *s);                                // 0x6ee30
extern "C" void Layout_drawWindowImpl5(Layout *self, void *str, int p3, int p4,
                                       int p5, int p6, int flag);  // 0x74e0c

// Layout::drawWindow(String, int p3, int p4, int p5, int p6)
//   -> drawWindow(this, copy, p3, p4, p5, p6, 1)
extern "C" void Layout_drawWindow5(Layout *self, const void *param, int p3, int p4,
                                   int p5, int p6) {
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    String_copy_ctor(tmp, param, false);
    Layout_drawWindowImpl5(self, tmp, p3, p4, p5, p6, 1);
    String_dtor(tmp);
}
