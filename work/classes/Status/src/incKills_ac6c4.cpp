#include "class.h"
extern "C" void *g_incKillsHook;
extern "C" void incKills_notify(void *arg);
void Status::incKills() {
    kills = kills + 1;
    return incKills_notify(g_incKillsHook);
}
