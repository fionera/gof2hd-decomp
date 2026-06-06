#include "class.h"

namespace AbyssEngine {
namespace AEMath {

static bool __attribute__((noinline)) MatrixNotEqualTail(const Matrix &lhs, const Matrix &rhs)
{
    return lhs.m[11] != rhs.m[11];
}

bool operator!=(const Matrix &lhs, const Matrix &rhs)
{
    if (__builtin_expect(((((lhs.m[0] == rhs.m[0] &&
                            lhs.m[1] == rhs.m[1]) &&
                           lhs.m[2] == rhs.m[2]) &&
                          (lhs.m[3] == rhs.m[3] &&
                           lhs.m[4] == rhs.m[4])) &&
                         (lhs.m[5] == rhs.m[5] &&
                          (lhs.m[6] == rhs.m[6] &&
                           lhs.m[7] == rhs.m[7]))) &&
                        (lhs.m[8] == rhs.m[8] &&
                         (lhs.m[9] == rhs.m[9] &&
                          lhs.m[10] == rhs.m[10])), 0)) {
        return MatrixNotEqualTail(lhs, rhs);
    }

    return true;
}

} // namespace AEMath
} // namespace AbyssEngine
