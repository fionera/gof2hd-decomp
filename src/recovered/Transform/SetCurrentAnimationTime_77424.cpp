#include "class.h"

namespace AbyssEngine {

void Transform::SetCurrentAnimationTime(longlong value) {
    *(longlong *)((char *)this + 0x110) = value;
}

} // namespace AbyssEngine
