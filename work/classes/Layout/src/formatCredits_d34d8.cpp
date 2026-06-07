#include "class.h"

extern "C" void Layout_formatNumber(void *out, int n);   // 0x74df4 (formatNumber)
extern "C" void String_cstr_ctor(void *out, const char *s, bool);  // 0x6ee18
extern "C" void String_concat(void *ret, void *a, void *b);        // operator+ 0x6ef98
extern "C" void String_dtor(void *s);                              // 0x6ee30

// Layout::formatCredits(int) -> out = formatNumber(n) + "$"
extern "C" void Layout_formatCredits(void *out, int n) {
    unsigned char num[sizeof(String12)] __attribute__((aligned(4)));
    unsigned char suffix[sizeof(String12)] __attribute__((aligned(4)));
    Layout_formatNumber(num, n);
    String_cstr_ctor(suffix, "$", false);
    String_concat(out, num, suffix);
    String_dtor(suffix);
    String_dtor(num);
}
