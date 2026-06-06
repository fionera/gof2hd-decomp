#include "quaternion.h"
#include <arm_neon.h>

extern "C" void Quaternion_Normalized(AbyssEngine::Quaternion *result,
                                      AbyssEngine::Quaternion *self);

namespace AbyssEngine {

__attribute__((stack_protect))
void Quaternion::Lerp(const float *a, const float *b, float t) {
    float32x4_t bv = vld1q_f32(b);
    float32x4_t av = vld1q_f32(a);
    float dot = vgetq_lane_f32(av, 1) * vgetq_lane_f32(bv, 1);
    dot += vgetq_lane_f32(av, 0) * vgetq_lane_f32(bv, 0);
    dot += vgetq_lane_f32(av, 2) * vgetq_lane_f32(bv, 2);
    dot += vgetq_lane_f32(av, 3) * vgetq_lane_f32(bv, 3);

    float32x4_t delta;
    if (dot < 0.0f) {
        delta = vsubq_f32(vnegq_f32(bv), av);
    } else {
        delta = vsubq_f32(bv, av);
    }

    float result[4] __attribute__((aligned(16)));
    vst1q_u64(reinterpret_cast<unsigned long long *>(result),
              vreinterpretq_u64_f32(vmlaq_f32(av, vdupq_n_f32(t), delta)));
    Quaternion_Normalized(this, reinterpret_cast<Quaternion *>(result));
}

} // namespace AbyssEngine
