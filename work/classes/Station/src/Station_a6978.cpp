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
    self->f_c = -1;
    self->f_10 = -1;
    self->f_20 = 0;
    self->f_18 = 0;
    self->f_1c = 0;
    self->f_24 = 0;
    self->f_28 = 0;
    self->f_2c = 0;
    self->f_30 = 0;
}
