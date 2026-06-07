#include "class.h"

namespace AbyssEngine {

void Transform::SetAnimationSpeed(float value) {
    *(float *)((char *)this + 0xf0) = value;
}

} // namespace AbyssEngine
