#include "class.h"

// Standing::rehabilitate(int race): reset the standing toward a faction's home
// race to a fixed value (-35 for the "even" slot, +35 for the "odd" slot).
extern "C" void Standing_rehabilitate(Standing *self, int race) {
    if (race == 0) {
        self->f_0[0] = -35;
    } else if (race == 1) {
        self->f_0[0] = 0x23;
    } else if (race == 2) {
        self->f_0[1] = -35;
    } else if (race == 3) {
        self->f_0[1] = 0x23;
    }
}
