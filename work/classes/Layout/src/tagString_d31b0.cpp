#include "class.h"

extern "C" void String_cstr_ctor(void *out, const char *s, bool);  // 0x6ee18
extern "C" void String_copy_global(void *out, const void *g);      // 0x6f658 (String(const String&))
extern "C" void String_concat(void *ret, void *a, void *b);        // operator+ 0x6ef98
extern "C" void String_dtor(void *s);                              // ptr from 0xe3222

// Hidden globals from tagString disasm.
__attribute__((visibility("hidden"))) extern const void *g_tagBaseString;  // @0xe329c
__attribute__((visibility("hidden"))) extern const char g_tagLit0[];       // @0xe31d2
__attribute__((visibility("hidden"))) extern const char g_tagLit1[];       // @0xe31ec
__attribute__((visibility("hidden"))) extern const char g_tagLit2[];       // @0xe320c (return prefix, ctor'd but unused in concat below)

// Layout::tagString(String): build "<lit0><base><lit1>" + param into the r0 return slot.
// out  = param_1 (the String return value, arrives in r0).
// in   = the incoming String (r2 -> aSStack_28 source).
extern "C" void Layout_tagString(void *out, const void *in) {
    unsigned char s_lit0[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_4c
    unsigned char s_base[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_58
    unsigned char s_ab[sizeof(String12)]   __attribute__((aligned(4)));   // aSStack_40
    unsigned char s_lit1[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_64
    unsigned char s_abc[sizeof(String12)]  __attribute__((aligned(4)));   // aSStack_34
    unsigned char s_full[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_28
    unsigned char s_lit2[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_70

    String_cstr_ctor(s_lit0, g_tagLit0, false);
    String_copy_global(s_base, g_tagBaseString);
    String_concat(s_ab, s_lit0, s_base);
    String_cstr_ctor(s_lit1, g_tagLit1, false);
    String_concat(s_abc, s_ab, s_lit1);
    String_concat(s_full, s_abc, (void *)in);
    String_cstr_ctor(s_lit2, g_tagLit2, false);
    String_concat(out, s_full, (void *)s_lit2);

    String_dtor(s_lit2);
    String_dtor(s_full);
    String_dtor(s_abc);
    String_dtor(s_lit1);
    String_dtor(s_ab);
    String_dtor(s_base);
    String_dtor(s_lit0);
}
