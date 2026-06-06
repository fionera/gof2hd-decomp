#include "class.h"

namespace AbyssEngine {
namespace AEMath {

float &Vector::operator[](int index) {
    return (&x)[index];
}

} // namespace AEMath
} // namespace AbyssEngine
