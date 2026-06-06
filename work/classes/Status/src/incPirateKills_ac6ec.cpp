#include "class.h"
extern "C" void *g_incPirateKillsHook;
extern "C" void incPirateKills_notify(void *arg);
void Status::incPirateKills() {
    pirateKills = pirateKills + 1;
    return incPirateKills_notify(g_incPirateKillsHook);
}
