#include "class.h"

namespace AbyssEngine {

void Transform::SetAnimationRangeInTime(longlong start, longlong end) {
    char *self = (char *)this;
    longlong length = *(longlong *)(self + 0xf8);
    if (length == 0) {
        return;
    }

    longlong rangeEnd = end;
    if (end < 0 || length < end) {
        rangeEnd = length;
    }
    *(longlong *)(self + 0x108) = rangeEnd;

    longlong rangeStart = start;
    if (length < start || start < 0) {
        rangeStart = 0;
    }
    *(longlong *)(self + 0x100) = rangeStart;

    longlong current = *(longlong *)(self + 0x110);
    if (end < current) {
        current = end;
    }
    if (current < start) {
        current = start;
    }
    *(longlong *)(self + 0x110) = current;

    for (uint i = 0; i < *(uint *)(self + 0x3c); ++i) {
        char *mesh = (*(char ***)(self + 0x40))[i];
        if (mesh != 0) {
            Transform *child = *(Transform **)(mesh + 0x34);
            if (child != 0) {
                child->SetAnimationRangeInTime(start, end);
            }
        }
    }
    for (uint i = 0; i < *(uint *)(self + 0x4c); ++i) {
        (*(Transform ***)(self + 0x50))[i]->SetAnimationRangeInTime(start, end);
    }
    Update(0, false);
}

} // namespace AbyssEngine
