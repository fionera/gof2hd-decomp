#include "class.h"

extern "C" void PaintCanvas_ReleaseAllResources(void *canvas);
extern "C" void Globals_releaseResources_tail(void *arg);
// Two hidden PC-relative pointer-to-pointer globals (deref'd twice).
extern void *const gRR_canvas __attribute__((visibility("hidden")));
extern void *const gRR_arg __attribute__((visibility("hidden")));

extern "C" void Globals_releaseResources()
{
    PaintCanvas_ReleaseAllResources(*(void **)gRR_canvas);
    return Globals_releaseResources_tail(*(void **)gRR_arg);
}
