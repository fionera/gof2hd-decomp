#include "class.h"

// Singleton holder: &g taken pc-rel, g is a pointer, *g is the flagged object pointer.
__attribute__((visibility("hidden"))) extern void **g_ModStation_suspendObj;
// Tail-called suspend veneer (0x1ac188): receives the object pointer (r0) and holder (r1).
extern "C" void ModStation_suspendTail(void *obj, void **holder);

// ModStation::OnSuspend()
extern "C" void ModStation_OnSuspend(ModStation *self)
{
    P(self, 0x128) = 0;
    void **holder = g_ModStation_suspendObj;
    void *obj = *holder;
    if (obj != 0)
        ModStation_suspendTail(obj, holder);
}
