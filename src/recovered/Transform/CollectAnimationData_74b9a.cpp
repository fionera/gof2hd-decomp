#include "class.h"

namespace AbyssEngine {

void Transform::CollectAnimationData() {
    char *self = (char *)this;
    longlong *length = (longlong *)(self + 0xf8);

    for (uint i = 0; i < *(uint *)(self + 0x3c); ++i) {
        char *mesh = (*(char ***)(self + 0x40))[i];
        if (mesh != 0) {
            Transform *child = *(Transform **)(mesh + 0x34);
            if (child != 0) {
                child->CollectAnimationData();
                child = *(Transform **)((*(char ***)(self + 0x40))[i] + 0x34);
                longlong childLength = *(longlong *)((char *)child + 0xf8);
                if (*length < childLength) {
                    *length = childLength;
                }
            }
        }
    }

    for (uint i = 0; i < *(uint *)(self + 0x4c); ++i) {
        Transform *child = (*(Transform ***)(self + 0x50))[i];
        child->CollectAnimationData();
        child = (*(Transform ***)(self + 0x50))[i];
        longlong childLength = *(longlong *)((char *)child + 0xf8);
        if (*length < childLength) {
            *length = childLength;
        }
    }

    int count = *(int *)(self + 0x11c);
    if (count != 0) {
        char *keyFrame = *(char **)(*(int *)(self + 0x120) + count * 4 - 4);
        longlong keyTime = *(longlong *)(keyFrame + 0x50);
        if (*length < keyTime) {
            *length = keyTime;
        }
    }
}

} // namespace AbyssEngine
