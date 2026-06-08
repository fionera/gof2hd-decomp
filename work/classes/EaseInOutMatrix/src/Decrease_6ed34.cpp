#include "class.h"
#include <arm_neon.h>

namespace AbyssEngine {

// See Increase: target uses fast-math `fmaxf(t, 0.75f)` -> scalar vmax.f32; not
// byte-reachable under the fixed -Oz harness. Kept best-effort.
void EaseInOutMatrix::Decrease(float dt) {
    float t = this->f_74 + (dt * -0.5f) / this->f_f0;
    this->f_74 = vget_lane_f32(vmax_f32(vdup_n_f32(t), vdup_n_f32(0.75f)), 0);
    UpdateCurrentValue();
}

} // namespace AbyssEngine
