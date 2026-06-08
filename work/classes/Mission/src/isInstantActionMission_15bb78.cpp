#include "class.h"

extern "C" uint8_t Mission_isInstantActionMission(Mission *self) {
    return self->f_5c;
}
