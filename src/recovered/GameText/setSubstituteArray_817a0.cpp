#include "class.h"

struct Arr { uint32_t size; int *data; uint32_t size2; };

extern "C" void ArrayRemoveAll_int(Arr *a);
extern "C" void ArrayAdd_int(int v, Arr *a);

// GameText::setSubstituteArray(int*, unsigned) -- replaces the substitute Array<int> at this+0.
extern "C" void GameText_setSubstituteArray(GameText *self, int *param_1, unsigned param_2)
{
    Arr *a = (Arr *)self;
    if (param_2 != 0) {
        if ((param_2 & 1) != 0) return;
        ArrayRemoveAll_int(a);
    }
    for (; param_2 != 0; --param_2) {
        int v = *param_1++;
        ArrayAdd_int(v, a);
    }
}
