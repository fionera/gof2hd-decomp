#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Vector MatrixGetUp(const Matrix &matrix) {
    Vector result;
    result.x = matrix.m[1];
    result.y = matrix.m[5];
    result.z = matrix.m[9];
    return result;
}

} // namespace AEMath
} // namespace AbyssEngine
