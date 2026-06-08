#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_String_vtable;

extern "C" {
void *operator_new__(unsigned int n);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
String *String_ctor_wchar(String *self, const uint16_t *s, bool rev);
}

// AbyssEngine::String::SubString(unsigned int start, unsigned int end)
// out = self[start..end); empty string when end <= start.
extern "C" void String_SubString(String *out, String *self, unsigned int start, unsigned int end)
{
    if (start < end) {
        int n = (int)(end - start);
        unsigned int count = n + 1;
        unsigned int bytes = count * 2;
        if (count + count < count)
            bytes = 0xffffffff;
        void *buf = operator_new__(bytes);
        __aeabi_memcpy(buf, (char *)F<void *>(self, 0x4) + start * 2, n * 2);
        *(uint16_t *)((char *)buf + n * 2) = 0;
        // Construct the result in-place from the freshly built wide buffer.
        String_ctor_wchar(out, (uint16_t *)buf, false);
    } else {
        // Empty substring: vtable pointer + null data + zero length.
        F<void *>(out, 0x0) = g_String_vtable;
        F<void *>(out, 0x4) = 0;
        F<uint32_t>(out, 0x8) = 0;
    }
}
