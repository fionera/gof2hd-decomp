#include "class.h"

extern "C" bool Mission_isEmpty(Mission *self) {
    return self->f_c == -1;
}
