#include "class.h"

// AbyssEngine::String::String(out, String* src, bool) -> void   (0x6f028)
extern "C" void String_ctor_copy(void *out, const void *src, bool);
// AbyssEngine::String::String(out, const char* cstr, bool) -> void   (0x6ee18)
extern "C" void String_ctor_cstr(void *out, const char *cstr, bool);

// Fallback literal accessed PC-relative.
extern const char kEmpty[] __attribute__((visibility("hidden")));

// AutoPilotList::getTargetString() -> String by value (sret in r0, this in r1).
// Returns a copy of the selected entry's String if the index is in range, else the
// fallback literal.
extern "C" RetStr AutoPilotList_getTargetString(AutoPilotList *self) {
    RetStr r;
    int idx = F<int>(self, 0x0);
    Array<void *> *entries = F<Array<void *> *>(self, 0x10);
    if (idx >= 0 && (uint32_t)idx < entries->length)
        String_ctor_copy(&r, entries->data[idx], false);
    else
        String_ctor_cstr(&r, kEmpty, false);
    return r;
}
