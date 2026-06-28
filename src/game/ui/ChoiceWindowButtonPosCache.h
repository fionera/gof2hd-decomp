#ifndef GOF2_CHOICEWINDOWBUTTONPOSCACHE_H
#define GOF2_CHOICEWINDOWBUTTONPOSCACHE_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "ScrollTouchWindow.h"
#include "TouchButton.h"

struct ChoiceWindowButtonPosCache {
    TouchButton *field_0x0;
    int field_0x4;

    union {
        // lint: union_decl (multi-name alias for one slot; right/single button X/Y all used cross-file: ChoiceWindow.cpp)
        int rightButtonPosX;
        int rightButtonPosY;
        int singleButtonPosX;
        int singleButtonPosY;
    };

    union {
        // lint: union_decl (multi-name alias for one slot; left button X/Y used cross-file: ChoiceWindow.cpp)
        int leftButtonPosX;
        int leftButtonPosY;
    };
};
#endif
