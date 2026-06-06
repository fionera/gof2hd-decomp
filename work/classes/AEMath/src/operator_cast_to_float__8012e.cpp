#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Matrix::operator const float *() const
{
    return m;
}

} // namespace AEMath
} // namespace AbyssEngine
