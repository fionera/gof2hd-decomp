#ifndef GOF2_MODULETRANSITIONTHUNK_H
#define GOF2_MODULETRANSITIONTHUNK_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "game/ui/TouchButton.h"

struct ModuleTransitionThunk {
    // Cross-file type-pun: transitionFn, transitionFn3 and field_0x0 are all
    // referenced together in MenuTouchWindow.cpp (e.g. transitionFn(field_0x0, 5)),
    // so the union cannot be collapsed. The two function-pointer aliases have
    // different arities; field_0x0 is the same slot reused as the call's context arg.
    union {
        // lint: union_decl transitionFn/transitionFn3 (different-arity fn ptrs) + field_0x0 (context slot) used together cross-file in MenuTouchWindow.cpp — genuine type-pun, layout-fixed
        void (*transitionFn)(void *app, int mode);

        // lint: void_ptr (opaque context arg; cross-file alias, no recoverable type)

        void (*transitionFn3)(void *app, int a, int b);

        // lint: void_ptr (opaque context arg; cross-file alias, no recoverable type)

        void *field_0x0; // lint: void_ptr (cross-file union member, referenced in MenuTouchWindow.cpp)
    };
};
#endif
