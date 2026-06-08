#include "class.h"

// 12-byte AbyssEngine::String, built/destroyed via engine wrappers.
struct String12 { uint32_t a, b, c; };

extern "C" {
void String12_ctor_copy(String12 *self, const String12 *src, bool copy);   // 0x6f028
void String12_ctor_char(String12 *self, const char *text, bool copy);      // 0x6ee18
void String12_dtor(String12 *self);                                        // dtor (blx via DAT)
// 3-arg overload (Status::replaceHash(String,String,String)) at 0xac8d4.
void Status_replaceHash3(void *self, String12 *out, String12 *haystack, String12 *needle,
                         String12 *replacement);
}

// Status::replaceHash(AbyssEngine::String haystack, AbyssEngine::String needle) -> String (sret)
//   Forwards to the three-argument overload, substituting the hash placeholder with "".
//   `out` is the hidden return slot; haystack/needle are passed by value (as String12*).
extern "C" void Status_replaceHash2(void *self, String12 *out, String12 *haystack, String12 *needle)
{
    String12 h, n, empty;
    String12_ctor_copy(&h, haystack, false);
    String12_ctor_copy(&n, needle, false);
    String12_ctor_char(&empty, "", false);
    Status_replaceHash3(self, out, &h, &n, &empty);
    String12_dtor(&empty);
    String12_dtor(&n);
    String12_dtor(&h);
}
