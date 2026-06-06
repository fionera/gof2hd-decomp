#include "class.h"

extern "C" unsigned int __stack_chk_guard;
extern "C" void __stack_chk_fail() __attribute__((noreturn));

namespace AbyssEngine {
namespace AEMath {

void VectorCross(Vector *result, const Vector &lhs, const Vector &rhs);
void VectorNormalize(Vector *result, const Vector &value);
void MatrixSetRotation(Matrix *result, Matrix &matrix, const Vector &right, const Vector &up, const Vector &dir);

void MatrixSetRotation(Matrix *result, Matrix &matrix, const Vector &dir)
{
    volatile unsigned int cookie = __stack_chk_guard;
    Vector right;
    Vector adjustedUp;
    Vector work;

    work.x = 0.0f;
    work.y = 1.0f;
    work.z = 0.0f;

    VectorCross(&adjustedUp, work, dir);
    VectorNormalize(&right, adjustedUp);
    VectorCross(&work, dir, right);
    VectorNormalize(&adjustedUp, work);
    MatrixSetRotation(result, matrix, right, adjustedUp, dir);

    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}

} // namespace AEMath
} // namespace AbyssEngine
