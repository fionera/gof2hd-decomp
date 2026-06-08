#include "class.h"
#include <arm_neon.h>

// Standing::getMissionBonus(int race): standing toward the faction, normalized to
// [0, 1] (negative standings clamp to 0). Out-of-range race returns 0.
// NOTE: best-effort. The target loads positive factions' counts straight from memory
// into the FP register (vldr.32 + vcvt) while negating others through a GPR; clang
// under -Oz routes every case through a GPR before a shared vcvt, and encodes the 0.0
// floor as an immediate rather than a literal-pool load. Both are codegen choices not
// reachable from portable C / NEON intrinsics here.
extern "C" float Standing_getMissionBonus(Standing *self, unsigned race) {
    float s0;
    switch (race) {
    case 0:
        s0 = (float)self->f_0[0];
        break;
    case 1:
        s0 = (float)(-self->f_0[0]);
        break;
    case 2:
        s0 = (float)self->f_0[1];
        break;
    case 3:
        s0 = (float)(-self->f_0[1]);
        break;
    default:
        return 0.0f;
    }
    float r = s0 / 100.0f;
    return vget_lane_f32(vmax_f32(vdup_n_f32(r), vdup_n_f32(0.0f)), 0);
}
