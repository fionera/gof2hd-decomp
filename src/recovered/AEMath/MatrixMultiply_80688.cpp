#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Matrix MatrixMultiply(const Matrix &lhs, const Matrix &rhs)
{
    return operator*(lhs, rhs);
}

} // namespace AEMath
} // namespace AbyssEngine
