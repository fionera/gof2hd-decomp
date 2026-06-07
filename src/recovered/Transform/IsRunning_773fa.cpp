#include "class.h"

namespace AbyssEngine {

int Transform::IsRunning() {
    longlong start = *(longlong *)((char *)this + 0x100);
    longlong current = *(longlong *)((char *)this + 0x110);
    if (start < current &&
        current < *(longlong *)((char *)this + 0x108) &&
        *(bool *)((char *)this + 0xed)) {
        return 1;
    }
    return 0;
}

} // namespace AbyssEngine
