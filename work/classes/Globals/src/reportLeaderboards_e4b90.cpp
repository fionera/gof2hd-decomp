#include "class.h"

struct Status;
extern "C" int Status_getKills(Status *s);
// Two hidden PC-relative globals, each a pointer-to-pointer (deref'd twice).
extern void *const gLB_status __attribute__((visibility("hidden")));
extern void *const gLB_dest __attribute__((visibility("hidden")));

extern "C" void Globals_reportLeaderboards()
{
    int kills = Status_getKills(*(Status **)gLB_status);
    *(int *)gLB_dest = kills;
}
