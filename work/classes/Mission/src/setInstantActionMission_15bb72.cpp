#include "class.h"

extern "C" void Mission_setInstantActionMission(Mission *self, bool v) {
    self->f_5c = v;
}
