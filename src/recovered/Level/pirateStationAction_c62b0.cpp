#include "class.h"

struct Station {
    int getPirateStationIndex();
};

struct Status {
    Station *getStation();
};

__attribute__((visibility("hidden"))) extern Status **g_status_pirate;
extern "C" void Level_pirateStationAction_tail(Level *self, int code, int arg);

void Level::pirateStationAction(bool param) {
    if (param) {
        if (field_1b0 != 0) {
            return;
        }
    } else {
        if (field_68 != 0) {
            return;
        }
        Status **slot = g_status_pirate;
        if ((*slot)->getStation()->getPirateStationIndex() < 0) {
            return;
        }
        int tbl = *(int *)((char *)*slot + 0x4c);
        int idx = (*slot)->getStation()->getPirateStationIndex();
        *(unsigned char *)(*(int *)(tbl + 4) + idx) = 1;
        *(unsigned char *)((char *)*slot + 0xf9) = 1;
    }
    Level_pirateStationAction_tail(this, param ? 3 : 4, 8);
}
