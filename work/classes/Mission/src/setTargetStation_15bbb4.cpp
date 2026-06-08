#include "class.h"

struct Galaxy;
struct Station;

// Galaxy singleton pointer global (hidden -> single pc-relative deref to the value).
__attribute__((visibility("hidden"))) extern Galaxy **g_galaxy;
extern "C" Station *Galaxy_getStation(Galaxy *g, int idx);
extern "C" void Station_getName(void *out, Station *s);
extern "C" void String_assign(void *self, void *rhs);
extern "C" void String_dtor(void *s);

// Sets the target station index (+0x3c) and caches that station's name into the
// targetStationName String (+0x40). The char-array-backed String temp triggers
// the -Oz/-fstack-protector canary the target emits.
extern "C" void Mission_setTargetStation(Mission *self, int idx) {
    unsigned char name[sizeof(String12)] __attribute__((aligned(4)));
    Galaxy **gp = g_galaxy;
    self->f_3c = idx;
    Station *st = Galaxy_getStation(*gp, idx);
    Station_getName(name, st);
    String_assign((char *)self + 0x40, name);
    String_dtor(name);
}
