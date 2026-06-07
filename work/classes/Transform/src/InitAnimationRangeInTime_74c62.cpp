#include "class.h"

namespace AbyssEngine {

void Transform::InitAnimationRangeInTime() {
    longlong length = *(longlong *)((char *)this + 0xf8);
    if (length == 0) {
        return;
    }

    longlong start = *(int *)((char *)this + 0xe8);
    *(longlong *)((char *)this + 0x100) = start;
    *(longlong *)((char *)this + 0x108) = length;
    *(longlong *)((char *)this + 0x110) = start;

    for (uint i = 0; i < *(uint *)((char *)this + 0x3c); ++i) {
        char **meshes = *(char ***)((char *)this + 0x40);
        char *mesh = meshes[i];
        if (mesh != 0) {
            Transform *child = *(Transform **)(mesh + 0x34);
            if (child != 0) {
                child->InitAnimationRangeInTime();
            }
        }
    }
    for (uint i = 0; i < *(uint *)((char *)this + 0x4c); ++i) {
        Transform **children = *(Transform ***)((char *)this + 0x50);
        children[i]->InitAnimationRangeInTime();
    }
    Update(0, false);
}

} // namespace AbyssEngine
