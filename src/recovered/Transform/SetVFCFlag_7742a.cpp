#include "class.h"

namespace AbyssEngine {

void Transform::SetVFCFlag(bool value) {
    for (uint i = 0; i < *(uint *)((char *)this + 0x3c); ++i) {
        char **meshes = *(char ***)((char *)this + 0x40);
        char *mesh = meshes[i];
        if (mesh != 0) {
            Transform *child = *(Transform **)(mesh + 0x34);
            if (child != 0) {
                child->SetVFCFlag(value);
            }
        }
    }
    for (uint i = 0; i < *(uint *)((char *)this + 0x4c); ++i) {
        Transform **children = *(Transform ***)((char *)this + 0x50);
        Transform *child = children[i];
        if (child != 0) {
            child->SetVFCFlag(value);
        }
    }
    *(bool *)((char *)this + 0x17c) = value;
}

} // namespace AbyssEngine
