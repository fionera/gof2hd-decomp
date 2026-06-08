#include "class.h"

// Engine String helpers used by the thousands-separator formatter.
extern "C" void String_from_uint(void *out, unsigned v);            // 0x6f658 String(uint)
extern "C" void String_cstr_ctor(void *out, const char *s, bool);   // 0x6ee18
extern "C" void String_default_ctor(void *out);                     // 0x6efbc String()
extern "C" void String_copy_ctor(void *out, const void *src, bool); // 0x6f028
extern "C" void String_assign(void *dst, const void *src);          // 0x6f2b0 operator=
extern "C" void String_concat(void *ret, void *a, void *b);         // 0x6ef98 operator+
extern "C" void String_subString(void *out, const void *src, int start, int len); // 0x6f604
extern "C" void String_dtor(void *s);                               // 0x6ee30 / 0x6ee30
extern "C" unsigned short GameText_getLanguage();                   // 0x6f544

// Hidden globals from formatNumber disasm.
__attribute__((visibility("hidden"))) extern int *g_fnGuard;        // @0xe32cc (stack guard [0])
__attribute__((visibility("hidden"))) extern const char g_fnEmpty[];   // @0xe32e4
__attribute__((visibility("hidden"))) extern const char g_fnSepA[];    // @0xe3308 (langs 0/10/11)
__attribute__((visibility("hidden"))) extern const char g_fnSepB[];    // @0xe3316 (other langs)
__attribute__((visibility("hidden"))) extern const char g_fnOverflow[];// @0xe33f6

// Layout::formatNumber(int): build a grouped decimal string into `out`.
extern "C" void Layout_formatNumber(void *out, int value) {
    int *guard = g_fnGuard;
    int g0 = *guard;

    unsigned mag = (value < 0) ? (unsigned)(-value) : (unsigned)value;

    unsigned char digits[sizeof(String12)] __attribute__((aligned(4)));  // aSStack_34
    String_from_uint(digits, mag);
    int len = *(int *)((char *)digits + 8);

    String_cstr_ctor(out, g_fnEmpty, false);

    unsigned char sep[sizeof(String12)] __attribute__((aligned(4)));      // aSStack_40
    unsigned short lang = GameText_getLanguage();
    if (lang < 0xc && ((1u << (lang & 0xff)) & 0xc01u) != 0)
        String_cstr_ctor(sep, g_fnSepA, false);
    else
        String_cstr_ctor(sep, g_fnSepB, false);

    if (len < 4) {
        String_assign(out, digits);
    } else {
        int i = len;
        while (i > 2) {
            i -= 3;
            unsigned char grp[sizeof(String12)] __attribute__((aligned(4)));  // aSStack_64
            String_subString(grp, digits, i, len);

            unsigned char prefix[sizeof(String12)] __attribute__((aligned(4))); // aSStack_70
            if (*(unsigned *)((char *)out + 8) < 2)
                String_default_ctor(prefix);
            else
                String_copy_ctor(prefix, sep, false);

            unsigned char t1[sizeof(String12)] __attribute__((aligned(4)));  // aSStack_58
            String_concat(t1, prefix, grp);
            unsigned char t2[sizeof(String12)] __attribute__((aligned(4)));  // aSStack_4c
            String_concat(t2, t1, out);
            String_assign(out, t2);

            String_dtor(t2);
            String_dtor(t1);
            String_dtor(prefix);
            String_dtor(grp);
        }
        unsigned char head[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_4c
        String_subString(head, digits, 0, i);
        if (*(int *)((char *)head + 8) != 0) {
            unsigned char j1[sizeof(String12)] __attribute__((aligned(4))); // aSStack_64
            String_concat(j1, head, sep);
            unsigned char j2[sizeof(String12)] __attribute__((aligned(4))); // aSStack_58
            String_concat(j2, j1, out);
            String_assign(out, j2);
            String_dtor(j2);
            String_dtor(j1);
        }
        String_dtor(head);
    }

    if ((unsigned)value > 0x7fffffff) {
        unsigned char ov[sizeof(String12)] __attribute__((aligned(4)));     // aSStack_58
        String_cstr_ctor(ov, g_fnOverflow, false);
        unsigned char r[sizeof(String12)] __attribute__((aligned(4)));      // aSStack_4c
        String_concat(r, ov, out);
        String_assign(out, r);
        String_dtor(r);
        String_dtor(ov);
    }

    String_dtor(sep);
    String_dtor(digits);

    if (*guard != g0)
        __stack_chk_fail();
}
