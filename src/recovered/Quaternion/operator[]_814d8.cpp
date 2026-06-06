#include "quaternion.h"

namespace AbyssEngine {

float &Quaternion::operator[](int index) {
    return (&x)[index];
}

} // namespace AbyssEngine
