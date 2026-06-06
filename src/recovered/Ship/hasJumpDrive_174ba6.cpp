#include "class.h"
extern "C" unsigned int hasJumpDrive(Ship *self) {
    if (self->hasJumpDriveFlag != 0) return 1;
    unsigned int v = (unsigned int)self->index - 0x25u;
    if (v < 4) return 0xbu >> (v & 0xf) & 1;
    return 0;
}
