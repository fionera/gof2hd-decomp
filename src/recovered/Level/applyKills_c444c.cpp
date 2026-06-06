#include "class.h"

struct Status {
    int getMission();
    void addKills(int n);
};

__attribute__((visibility("hidden"))) extern Status **g_status_applyKills;

void Level::applyKills() {
    Status **slot = g_status_applyKills;
    if ((*slot)->getMission() != 0) {
        (*slot)->addKills(kills);
        kills = 0;
    }
}
