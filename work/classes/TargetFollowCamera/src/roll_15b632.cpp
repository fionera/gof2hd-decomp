#include "class.h"

// this->[0x130] += delta  (float read-modify-write)
extern "C" void TFC_roll(TargetFollowCamera *self, float delta) {
    F<float>(self, 0x130) = F<float>(self, 0x130) + delta;
}
