#include "class.h"

extern "C" void Level_wanted_action(Level *self, int a, int b);

void Level::killWanted() {
    if (field_29d == 0) {
        field_29d = 1;
        return Level_wanted_action(this, 0x11, 0);
    }
}
