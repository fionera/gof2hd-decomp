#ifndef GOF2_KEYCODE_H
#define GOF2_KEYCODE_H
#include "game/core/String.h"

#include "engine/core/AEString.h"

namespace AbyssEngine {
    class KeyCode {
    public:
        int code;
        String name;
        // copy assignment is implicit: the original emits it WEAK in the ApplicationManager TU
    };
}

#endif
