#include "class.h"
extern "C" unsigned int hasJumpDriveIntegrated(Ship *self) {
    unsigned int v = (unsigned int)self->index - 0x25u;
    if (v < 4) return 0xbu >> (v & 0xf) & 1;
    return 0;
}
