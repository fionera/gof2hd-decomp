#include "class.h"

// AbyssEngine::String::IndexOf(unsigned int start, AbyssEngine::String const&)
// Return the first index >= start where needle occurs, or 0xffffffff if not found.
extern "C" unsigned int String_IndexOf_from(String *self, unsigned int start, String *needle)
{
    while (start < F<uint32_t>(self, 0x8) &&
           F<uint32_t>(needle, 0x8) <= F<uint32_t>(self, 0x8) - start) {
        short first = *F<short *>(needle, 0x4);
        if (first == *(short *)((char *)F<void *>(self, 0x4) + start * 2)) {
            unsigned int k = 0;
            while (first == *(short *)((char *)F<void *>(self, 0x4) + start * 2 + k * 2)) {
                if (F<uint32_t>(needle, 0x8) <= k + 1)
                    return start;
                first = F<short *>(needle, 0x4)[k + 1];
                k = k + 1;
            }
            start = start + k;
        } else {
            start = start + 1;
        }
    }
    return 0xffffffff;
}
