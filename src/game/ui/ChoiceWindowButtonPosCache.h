#ifndef GOF2_CHOICEWINDOWBUTTONPOSCACHE_H
#define GOF2_CHOICEWINDOWBUTTONPOSCACHE_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "ScrollTouchWindow.h"
#include "TouchButton.h"

struct ChoiceWindowButtonPosCache {
    TouchButton *field_0x0;
    int field_0x4;

    int rightOrSingleButtonPos;

    int leftButtonPos;
};
#endif
