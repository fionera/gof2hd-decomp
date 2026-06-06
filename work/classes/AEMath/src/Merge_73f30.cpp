#include "class.h"

extern "C" unsigned int __stack_chk_guard;
extern "C" void __stack_chk_fail() __attribute__((noreturn));

namespace AbyssEngine {
namespace AEMath {

void BSphere::Merge(const ::AbyssEngine::Transform &transform) {
    volatile unsigned int cookie = __stack_chk_guard;

    BSphere other;

    other.radius2 = 1.0f;
    Vector temp = MatrixTransformVector(transform.matrix, transform.bounds.center);
    other.center.x = temp.x;
    other.center.y = temp.y;
    other.center.z = temp.z;

    temp.x = transform.bounds.radius;
    temp.y = transform.bounds.radius;
    temp.z = transform.bounds.radius;
    Vector rotated = MatrixRotateVector(transform.matrix, temp);

    float abs_x = -rotated.x;
    if (0.0f < rotated.x) {
        abs_x = rotated.x;
    }

    float abs_y = -rotated.y;
    if (0.0f < rotated.y) {
        abs_y = rotated.y;
    }

    float abs_z = -rotated.z;
    if (0.0f < rotated.z) {
        abs_z = rotated.z;
    }

    if (abs_y < abs_x) {
        abs_y = abs_x;
    }
    if (abs_z < abs_y) {
        abs_z = abs_y;
    }

    other.radius = abs_z;
    Merge(other);

    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}

} // namespace AEMath
} // namespace AbyssEngine
