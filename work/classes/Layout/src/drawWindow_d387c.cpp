#include "class.h"

extern "C" void String_copy_ctor(void *out, const void *src, bool);  // 0x6f028
extern "C" void String_dtor(void *s);                                // 0x6ee30
extern "C" void Layout_drawWindowImpl(Layout *self, void *str, int flag); // 0x74e24

// Layout::drawWindow(String) -> drawWindow(this, copy, 0). The on-stack String
// temp makes the compiler emit the -fstack-protector canary automatically.
extern "C" void Layout_drawWindow1(Layout *self, const void *param) {
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    String_copy_ctor(tmp, param, false);
    Layout_drawWindowImpl(self, tmp, 0);
    String_dtor(tmp);
}
