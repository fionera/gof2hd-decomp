#include "class.h"

extern "C" void String_copy_ctor(void *out, const void *src, bool);  // 0x6f028
extern "C" void String_dtor(void *s);                                // 0x6ee30
extern "C" void Layout_drawWindowImpl5(Layout *self, void *str, int a, int b,
                                       int p4, int p5, int flag);  // 0x74e0c
__attribute__((visibility("hidden"))) extern int *gW1;  // ldr [0xe3774]
__attribute__((visibility("hidden"))) extern int *gW2;  // ldr [0xe3778]
__attribute__((visibility("hidden"))) extern int *gW3;  // ldr [0xe377c]

// Layout::drawWindow(String, bool flag)
//   -> drawWindow(this, copy, 0, 0, *gW2, *gW3 - (*gW1)[2], flag)
extern "C" void Layout_drawWindow2(Layout *self, const void *param, int flag) {
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    String_copy_ctor(tmp, param, false);
    int p4 = *gW2;
    int p5 = *gW3 - ((int *)gW1)[2];
    Layout_drawWindowImpl5(self, tmp, 0, 0, p4, p5, flag);
    String_dtor(tmp);
}
