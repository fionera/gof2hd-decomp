#include "class.h"

// Vtable pointer stored at +0 (target loads a PC-relative GOT slot and adds 8).
__attribute__((visibility("hidden"))) extern void *g_String_vtable;

// AbyssEngine::String::String() - default constructor: empty string.
extern "C" void String_ctor(String *self)
{
    F<void *>(self, 0x0) = g_String_vtable;
    F<void *>(self, 0x4) = 0;
    F<uint32_t>(self, 0x8) = 0;
}
