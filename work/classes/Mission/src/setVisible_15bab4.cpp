#include "class.h"

extern "C" void Mission_setVisible(Mission *self, bool v) {
    self->f_74 = v;
}
