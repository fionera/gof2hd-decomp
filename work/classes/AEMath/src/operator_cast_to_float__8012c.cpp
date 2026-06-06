#include "class.h"

namespace AbyssEngine {
namespace AEMath {

Matrix::operator float *()
{
    return m;
}

} // namespace AEMath
} // namespace AbyssEngine
