#include "quaternion.h"

namespace AbyssEngine {

float Quaternion::operator[](int index) const {
    return (&x)[index];
}

} // namespace AbyssEngine
