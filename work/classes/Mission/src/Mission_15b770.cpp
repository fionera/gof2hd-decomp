#include "class.h"

struct Galaxy;
struct Station;

__attribute__((visibility("hidden"))) extern void *Mission_vtable;
__attribute__((visibility("hidden"))) extern Galaxy **g_galaxy;
extern "C" void String_default_ctor(void *s);
extern "C" void String_cstr_ctor(void *out, const char *s, bool);
extern "C" void String_assign(void *self, void *rhs);
extern "C" void String_dtor(void *s);
extern "C" Station *Galaxy_getStation(Galaxy *g, int idx);
extern "C" void Station_getName(void *out, Station *s);

// Mission::Mission(int id, int goods, int station) — the campaign-mission constructor.
extern "C" Mission *
Mission_ctor3(Mission *self, int id, int goods, int station) {
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    *(void **)self = (char *)Mission_vtable + 8;
    String_default_ctor((char *)self + 0x10);
    String_default_ctor((char *)self + 0x1c);
    String_default_ctor((char *)self + 0x40);
    String_default_ctor((char *)self + 0x4c);
    self->f_30 = goods;
    self->f_3c = station;
    self->f_28 = 0;
    self->f_8 = 0;
    self->f_c = id;
    if (station < 0) {
        String_cstr_ctor(tmp, "", false);
    } else {
        Station *st = Galaxy_getStation(*g_galaxy, station);
        Station_getName(tmp, st);
    }
    String_assign((char *)self + 0x40, tmp);
    String_dtor(tmp);
    String_cstr_ctor(tmp, "", false);
    String_assign((char *)self + 0x4c, tmp);
    String_dtor(tmp);
    self->f_74 = 1;
    self->f_64 = 1;
    self->f_4 = 0;
    self->f_5c = 0;
    self->f_70 = 0;
    self->f_38 = 0;
    return self;
}
