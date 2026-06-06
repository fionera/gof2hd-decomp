#include "class.h"
extern "C" bool hasCloak(Ship *self) {
    if (__builtin_expect(self->hasCloakFlag != 0, 1)) goto yes;
    if (self->index == 0x31 || self->index == 0x2c) goto yes;
    return false;
yes:
    return true;
}
