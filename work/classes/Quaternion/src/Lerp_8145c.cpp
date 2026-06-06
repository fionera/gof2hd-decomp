#include "quaternion.h"
#include <arm_neon.h>

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned int delta) __attribute__((noreturn));
extern "C" void quaternion_normalized(AbyssEngine::Quaternion *out,
                                      AbyssEngine::Quaternion *self);

namespace AbyssEngine {

void Quaternion::Lerp(const float *a, const float *b, float t) {
    typedef float Vec4 __attribute__((vector_size(16)));

    void *volatile cookie = __stack_chk_guard;

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

    float32x4_t result = vmlaq_f32(av, vdupq_n_f32(t), delta);
    alignas(16) float storage[4];
    *reinterpret_cast<Vec4 *>(storage) = result;
    quaternion_normalized(this, reinterpret_cast<Quaternion *>(storage));

    unsigned int guardDelta = (unsigned int)cookie - (unsigned int)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
