#include "class.h"

namespace AbyssEngine {
namespace AEMath {

// AbyssEngine::AEMath::Vector::operator[](int) -> reference to component.
float &Vector::operator[](int index)
{
    return (&x)[index];
}

} // namespace AEMath
} // namespace AbyssEngine
