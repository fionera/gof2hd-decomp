#ifndef GOF2_MODULETRANSITIONTHUNK_H
#define GOF2_MODULETRANSITIONTHUNK_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "game/ui/TouchButton.h"

struct ModuleTransitionThunk {
    // Single pointer slot at offset 0. Canonical type is the 2-arg transition
    // function pointer. Call sites that invoke it with 3 args cast the slot to the
    // 3-arg signature; the call's opaque context arg (formerly field_0x0) is the
    // same slot read as a void* via reinterpret_cast<void *&>(transitionFn).
    void (*transitionFn)(void *app, int mode); // lint: void_ptr (opaque callback context; decompiler-conflated thunk slot)
};
#endif
