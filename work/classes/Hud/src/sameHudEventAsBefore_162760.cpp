#include "class.h"

extern "C" int String_Compare(String *a, String *b);   // AbyssEngine::String::Compare

extern "C" unsigned int Hud_sameHudEventAsBefore(Hud *self, String *str)
{
    int i = *(int *)self->f_264;
    while (--i >= 1) {
        int e = ((int *)I(self->f_264, 4))[i];
        if (e != 0 && String_Compare((String *)I((void *)e, 0x1c), str) == 0)
            return 1;
    }
    return 0;
}
