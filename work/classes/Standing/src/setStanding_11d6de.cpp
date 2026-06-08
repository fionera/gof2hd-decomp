#include "class.h"

// Standing::setStanding(int race, int value): standings[race] = value.
extern "C" void Standing_setStanding(Standing *self, int race, int value) {
    self->f_0[race] = value;
}
