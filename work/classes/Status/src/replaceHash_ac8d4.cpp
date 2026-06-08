#include "class.h"

// 12-byte AbyssEngine::String, built/destroyed via engine wrappers.
struct String12 { uint32_t a, b, c; };

extern "C" {
int  String12_IndexOf(String12 *self, String12 *needle);                  // 0x6f3a0
void String12_SubString(String12 *out, String12 *self, int start, int len); // 0x6f604
void String12_ctor_copy(String12 *self, const String12 *src, bool copy);  // 0x6f028
void String12_concat(String12 *out, String12 *left, String12 *right);     // 0x6ef98 operator+
void String12_dtor(String12 *self);                                       // 0x6ee30 ~String
void String12_dtor_v(String12 *self);                                     // dtor via DAT (blx r4)
}

// Status::replaceHash(String haystack, String needle, String replacement) -> String (sret)
//   Replaces the first occurrence of `needle` in `haystack` with `replacement`. If `needle`
//   is absent, returns a copy of `haystack`. `out` is the hidden return slot.
extern "C" void Status_replaceHash3(void *self, String12 *out, String12 *haystack,
                                    String12 *needle, String12 *replacement)
{
    (void)self;
    int idx = String12_IndexOf(haystack, needle);
    if (idx < 0) {
        // no match -> copy haystack into the result.
        String12_ctor_copy(out, haystack, false);
        return;
    }

    // prefix = haystack[0 .. idx)
    String12 prefix;
    String12_SubString(&prefix, haystack, 0, idx);

    if (prefix.b == 0) {
        // needle at position 0: result = replacement + haystack[idx+len ..)
        String12 repl;
        String12_ctor_copy(&repl, replacement, false);
        String12 suffix;
        String12_SubString(&suffix, haystack, needle->c + idx, haystack->c);
        String12_concat(out, &repl, &suffix);
        String12_dtor_v(&suffix);
        String12_dtor_v(&repl);
    } else {
        // result = prefix + replacement + haystack[idx+len ..)
        String12 repl;
        String12_ctor_copy(&repl, replacement, false);
        String12 mid;
        String12_concat(&mid, &prefix, &repl);
        String12 suffix;
        String12_SubString(&suffix, haystack, needle->c + idx, haystack->c);
        String12_concat(out, &mid, &suffix);
        String12_dtor_v(&suffix);
        String12_dtor_v(&mid);
        String12_dtor_v(&repl);
    }
    String12_dtor(&prefix);
}
