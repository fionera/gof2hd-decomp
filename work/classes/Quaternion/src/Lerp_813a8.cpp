#include "quaternion.h"
#include <arm_neon.h>

extern "C" unsigned int __stack_chk_guard;
extern "C" void __stack_chk_fail() __attribute__((noreturn));
extern "C" void quaternion_sub(AbyssEngine::Quaternion *out,
                               const AbyssEngine::Quaternion *a,
                               const AbyssEngine::Quaternion *b);
extern "C" void quaternion_normalized(AbyssEngine::Quaternion *out,
                                      AbyssEngine::Quaternion *self);

namespace AbyssEngine {

void Quaternion::Lerp(const Quaternion &a, const Quaternion &b, float t) {
    typedef float Vec4 __attribute__((vector_size(16)));

    volatile unsigned int cookie = __stack_chk_guard;
    alignas(16) unsigned char result_storage[sizeof(Quaternion)];
    alignas(16) unsigned char delta_storage[sizeof(Quaternion)];
    Quaternion *delta = reinterpret_cast<Quaternion *>(delta_storage);
    Quaternion *result = reinterpret_cast<Quaternion *>(result_storage);

    quaternion_sub(delta, &b, &a);

    float32x4_t delta_vec = *reinterpret_cast<Vec4 *>(delta);
    float32x4_t a_vec = vld1q_f32(&a.x);
    float32x4_t result_vec = vmlaq_f32(a_vec, vdupq_n_f32(t), delta_vec);
    *reinterpret_cast<Vec4 *>(result) = result_vec;
    quaternion_normalized(this, result);

    if (__stack_chk_guard == cookie) {
        return;
    }
    __stack_chk_fail();
}

} // namespace AbyssEngine
