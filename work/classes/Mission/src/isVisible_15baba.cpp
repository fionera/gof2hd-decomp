#include "class.h"

extern "C" uint8_t Mission_isVisible(Mission *self) {
    return self->f_74;
}
