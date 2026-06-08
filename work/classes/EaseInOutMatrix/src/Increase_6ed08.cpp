#include "class.h"
#include <arm_neon.h>

namespace AbyssEngine {

// NOTE: the target was built with fast-math; its `fminf(t, 1.25f)` lowers to a
// scalar `vmin.f32 d0,d0,d2` + `vstr s0`. Under the fixed -Oz (no -ffast-math)
// harness, clang lowers the equivalent NEON min to vdup/vst1 form instead, so a
// byte-exact match is not reachable with the available flags. Kept best-effort.
void EaseInOutMatrix::Increase(float dt) {
    float t = f32(this, 0x74) + (dt * 0.5f) / f32(this, 0xf0);
    f32(this, 0x74) = vget_lane_f32(vmin_f32(vdup_n_f32(t), vdup_n_f32(1.25f)), 0);
    UpdateCurrentValue();
}

} // namespace AbyssEngine
