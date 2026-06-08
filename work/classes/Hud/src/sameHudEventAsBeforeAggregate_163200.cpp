#include "class.h"

extern "C" int String_Compare(String *a, String *b);   // AbyssEngine::String::Compare

extern "C" int Hud_sameHudEventAsBeforeAggregate(Hud *self, String *str)
{
    int i = *(int *)self->f_264;
    int e;
    do {
        i = i + -1;
        if (i < 1)
            return -1;
        e = I((void *)(I(self->f_264, 4)), i * 4);
    } while (e == 0 || String_Compare((String *)I((void *)e, 0x1c), str) != 0);
    return i;
}
