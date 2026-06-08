#include "class.h"

extern "C" void *operator_new(uint32_t n);
extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" void String_dtor(void *s);
// Mission::Mission(int, String, int*, int, int, int, int) -> 0x15b64c. The by-value
// String is passed by invisible reference -> model it as a pointer parameter.
extern "C" void Mission_ctor7(Mission *self, int id, const void *name,
                              int a, int b, int c, int d, int e);

// Mission::clone(): deep-copies this mission into a freshly allocated Mission via
// the 7-arg constructor, then copies the instant-action flag. The on-stack String
// temp triggers the -Oz stack-protector canary.
extern "C" Mission *Mission_clone(Mission *self) {
    unsigned char name[sizeof(String12)] __attribute__((aligned(4)));
    Mission *m = (Mission *)operator_new(0x78);
    int id = self->f_c;
    String_copy_ctor(name, (char *)self + 0x10, false);
    Mission_ctor7(m, id, name,
                  self->f_28, self->f_2c, self->f_30,
                  self->f_3c, self->f_58);
    String_dtor(name);
    m->f_5c = self->f_5c;
    return m;
}
