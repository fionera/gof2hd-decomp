#include "class.h"

namespace AbyssEngine {

static float lerp_float(float from, float to, float t) {
    return from + t * (to - from);
}

void Transform::UpdateKeyFrames(KeyFrame *keyFrame, int index) {
    char *self = (char *)this;
    char *key = (char *)keyFrame;

    int i = 0;
    while (i + 1 < index) {
        char **items = *(char ***)(self + 0x120);
        char *next = items[i + 1];
        char *prev = items[i];
        longlong keyTime = *(longlong *)(key + 0x50);
        float a = (float)(keyTime - *(longlong *)(next + 0x50));
        float b = (float)(keyTime - *(longlong *)(prev + 0x50));
        float t = 1.0f - a / b;
        uint flags0 = *(uint *)(key + 0x58);
        uint flags1 = *(uint *)(key + 0x5c);
        uint next0 = *(uint *)(next + 0x58);
        uint next1 = *(uint *)(next + 0x5c);

        if ((flags0 & 0x40) && !(next0 & 0x40)) *(float *)(next + 0x18) = lerp_float(*(float *)(prev + 0x18), *(float *)(key + 0x18), t);
        if ((flags0 & 0x80) && !(next0 & 0x80)) *(float *)(next + 0x1c) = lerp_float(*(float *)(prev + 0x1c), *(float *)(key + 0x1c), t);
        if ((flags0 & 0x100) && !(next0 & 0x100)) *(float *)(next + 0x20) = lerp_float(*(float *)(prev + 0x20), *(float *)(key + 0x20), t);
        if ((flags0 & 0x1) && !(next0 & 0x1)) *(float *)(next + 0x00) = lerp_float(*(float *)(prev + 0x00), *(float *)(key + 0x00), t);
        if ((flags0 & 0x2) && !(next0 & 0x2)) *(float *)(next + 0x04) = lerp_float(*(float *)(prev + 0x04), *(float *)(key + 0x04), t);
        if ((flags0 & 0x4) && !(next0 & 0x4)) *(float *)(next + 0x08) = lerp_float(*(float *)(prev + 0x08), *(float *)(key + 0x08), t);
        if ((flags0 & 0x8) && !(next0 & 0x8)) *(float *)(next + 0x0c) = lerp_float(*(float *)(prev + 0x0c), *(float *)(key + 0x0c), t);
        if ((flags0 & 0x10) && !(next0 & 0x10)) *(float *)(next + 0x10) = lerp_float(*(float *)(prev + 0x10), *(float *)(key + 0x10), t);
        if ((flags0 & 0x20) && !(next0 & 0x20)) *(float *)(next + 0x14) = lerp_float(*(float *)(prev + 0x14), *(float *)(key + 0x14), t);
        if ((flags0 & 0x200) && !(next0 & 0x200)) *(float *)(next + 0x48) = lerp_float(*(float *)(prev + 0x48), *(float *)(key + 0x48), t);
        if ((flags0 & 0x10000) && !(next0 & 0x10000)) *(float *)(next + 0x3c) = lerp_float(*(float *)(prev + 0x3c), *(float *)(key + 0x3c), t);
        if ((flags0 & 0x20000) && !(next0 & 0x20000)) *(float *)(next + 0x40) = lerp_float(*(float *)(prev + 0x40), *(float *)(key + 0x40), t);
        if ((flags0 & 0x40000) && !(next0 & 0x40000)) *(float *)(next + 0x44) = lerp_float(*(float *)(prev + 0x44), *(float *)(key + 0x44), t);
        if ((flags0 & 0x400) && !(next0 & 0x400)) *(float *)(next + 0x24) = lerp_float(*(float *)(prev + 0x24), *(float *)(key + 0x24), t);
        if ((flags0 & 0x800) && !(next0 & 0x800)) *(float *)(next + 0x28) = lerp_float(*(float *)(prev + 0x28), *(float *)(key + 0x28), t);
        if ((flags0 & 0x1000) && !(next0 & 0x1000)) *(float *)(next + 0x2c) = lerp_float(*(float *)(prev + 0x2c), *(float *)(key + 0x2c), t);
        if ((flags0 & 0x2000) && !(next0 & 0x2000)) *(float *)(next + 0x30) = lerp_float(*(float *)(prev + 0x30), *(float *)(key + 0x30), t);
        if ((flags0 & 0x4000) && !(next0 & 0x4000)) *(float *)(next + 0x34) = lerp_float(*(float *)(prev + 0x34), *(float *)(key + 0x34), t);
        if ((flags0 & 0x8000) && !(next0 & 0x8000)) *(float *)(next + 0x38) = lerp_float(*(float *)(prev + 0x38), *(float *)(key + 0x38), t);
        *(uint *)(next + 0x58) = next0 | flags0;
        *(uint *)(next + 0x5c) = next1 | flags1;
        ++i;
    }

    uint count = *(uint *)(self + 0x11c);
    while ((uint)++index < count) {
        char *dst = (*(char ***)(self + 0x120))[index];
        uint flags0 = *(uint *)(key + 0x58);
        if ((flags0 & 0x40) && !(*(uint *)(dst + 0x58) & 0x40)) *(int *)(dst + 0x18) = *(int *)(key + 0x18);
        if ((flags0 & 0x80) && !(*(uint *)(dst + 0x58) & 0x80)) *(int *)(dst + 0x1c) = *(int *)(key + 0x1c);
        if ((flags0 & 0x100) && !(*(uint *)(dst + 0x58) & 0x100)) *(int *)(dst + 0x20) = *(int *)(key + 0x20);
        if ((flags0 & 0x1) && !(*(uint *)(dst + 0x58) & 0x1)) *(int *)(dst + 0x00) = *(int *)(key + 0x00);
        if ((flags0 & 0x2) && !(*(uint *)(dst + 0x58) & 0x2)) *(int *)(dst + 0x04) = *(int *)(key + 0x04);
        if ((flags0 & 0x4) && !(*(uint *)(dst + 0x58) & 0x4)) *(int *)(dst + 0x08) = *(int *)(key + 0x08);
        if ((flags0 & 0x8) && !(*(uint *)(dst + 0x58) & 0x8)) *(int *)(dst + 0x0c) = *(int *)(key + 0x0c);
        if ((flags0 & 0x10) && !(*(uint *)(dst + 0x58) & 0x10)) *(int *)(dst + 0x10) = *(int *)(key + 0x10);
        if ((flags0 & 0x20) && !(*(uint *)(dst + 0x58) & 0x20)) *(int *)(dst + 0x14) = *(int *)(key + 0x14);
        if ((flags0 & 0x200) && !(*(uint *)(dst + 0x58) & 0x200)) *(int *)(dst + 0x48) = *(int *)(key + 0x48);
        if ((flags0 & 0x10000) && !(*(uint *)(dst + 0x58) & 0x10000)) *(int *)(dst + 0x3c) = *(int *)(key + 0x3c);
        if ((flags0 & 0x20000) && !(*(uint *)(dst + 0x58) & 0x20000)) *(int *)(dst + 0x40) = *(int *)(key + 0x40);
        if ((flags0 & 0x40000) && !(*(uint *)(dst + 0x58) & 0x40000)) *(int *)(dst + 0x44) = *(int *)(key + 0x44);
        if ((flags0 & 0x400) && !(*(uint *)(dst + 0x58) & 0x400)) *(int *)(dst + 0x24) = *(int *)(key + 0x24);
        if ((flags0 & 0x800) && !(*(uint *)(dst + 0x58) & 0x800)) *(int *)(dst + 0x28) = *(int *)(key + 0x28);
        if ((flags0 & 0x1000) && !(*(uint *)(dst + 0x58) & 0x1000)) *(int *)(dst + 0x2c) = *(int *)(key + 0x2c);
        if ((flags0 & 0x2000) && !(*(uint *)(dst + 0x58) & 0x2000)) *(int *)(dst + 0x30) = *(int *)(key + 0x30);
        if ((flags0 & 0x4000) && !(*(uint *)(dst + 0x58) & 0x4000)) *(int *)(dst + 0x34) = *(int *)(key + 0x34);
        if ((flags0 & 0x8000) && !(*(uint *)(dst + 0x58) & 0x8000)) *(int *)(dst + 0x38) = *(int *)(key + 0x38);
    }
}

} // namespace AbyssEngine
