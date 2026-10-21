#ifndef GOF2_KEYCODE_H
#define GOF2_KEYCODE_H
#include "AEString.h"

namespace AbyssEngine {
    // One key-mapping entry of the [KEYMAP] config section: "<code>,<action name>".
    class KeyCode {
    public:
        int code;
        String name;
    };
}

#endif
