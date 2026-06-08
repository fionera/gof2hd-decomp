#include "class.h"

extern "C" void *String_default_ctor(void *s);                 // String::String() -> this
extern "C" void String_from_cstr(void *out, const char *s, bool); // String::String(const char*, bool)
extern "C" void String_assign(void *dst, void *src);           // operator=(String*, String*)
extern "C" void String_dtor(void *s);

extern const char kStationDefaultName[] __attribute__((visibility("hidden")));

// Station::Station() — default ctor.
extern "C" void Station_ctor_default(Station *self)
{
    String_default_ctor(self);
    char tmp[12];
    String_from_cstr(tmp, kStationDefaultName, false);
    String_assign(self, tmp);
    String_dtor(tmp);
    F<int>(self, 0xc) = -1;
    F<int>(self, 0x10) = -1;
    F<int>(self, 0x20) = 0;
    F<int>(self, 0x18) = 0;
    F<uint8_t>(self, 0x1c) = 0;
    F<uint8_t>(self, 0x24) = 0;
    F<int>(self, 0x28) = 0;
    F<int>(self, 0x2c) = 0;
    F<int>(self, 0x30) = 0;
}
