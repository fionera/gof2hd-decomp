#include "class.h"

namespace AbyssEngine {

void Transform::SetAnimationLength(longlong value) {
    *(longlong *)((char *)this + 0x108) = value;
    *(longlong *)((char *)this + 0xf8) = value;
}

} // namespace AbyssEngine
