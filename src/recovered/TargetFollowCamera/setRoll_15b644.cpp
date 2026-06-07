#include "class.h"

// str.w r1,[r0,#0x130]
extern "C" void TFC_setRoll(TargetFollowCamera *self, float v) {
    F<float>(self, 0x130) = v;
}
