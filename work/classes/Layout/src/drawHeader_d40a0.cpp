#include "class.h"

extern "C" void String_cstr_ctor(void *out, const char *s, bool);  // 0x6ee18
extern "C" void String_dtor(void *s);                              // 0x6ee30
extern "C" void Layout_drawHeaderImpl(Layout *self, void *str, int flag); // 0x74e6c

// Layout::drawHeader() -> drawHeader(this, String(""), 0)
extern "C" void Layout_drawHeader0(Layout *self) {
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    String_cstr_ctor(tmp, "", false);
    Layout_drawHeaderImpl(self, tmp, 0);
    String_dtor(tmp);
}
