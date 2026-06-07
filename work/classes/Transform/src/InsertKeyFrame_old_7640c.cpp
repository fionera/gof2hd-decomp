#include "class.h"

namespace AbyssEngine {

static float old_angle_divisor = 57.295780f;

static void apply_value_old(Transform *owner, char *key, const float *values, longlong flags) {
    if (flags == 0x10) {
        *(float *)(key + 0x10) = values[0];
        if (*(float *)((char *)owner + 0xe4) < values[0]) *(float *)((char *)owner + 0xe4) = values[0];
    } else if (flags == 0x20) {
        *(float *)(key + 0x14) = values[0];
        if (*(float *)((char *)owner + 0xe4) < values[0]) *(float *)((char *)owner + 0xe4) = values[0];
    } else if (flags == 0x38) {
        *(float *)(key + 0x0c) = values[0];
        *(float *)(key + 0x10) = values[1];
        *(float *)(key + 0x14) = values[2];
        float max = values[1] < values[0] ? values[0] : values[1];
        if (values[2] > max) max = values[2];
        if (*(float *)((char *)owner + 0xe4) < max) *(float *)((char *)owner + 0xe4) = max;
    } else if (flags == 0x40) {
        *(float *)(key + 0x18) = values[0];
    } else if (flags == 0x80) {
        *(float *)(key + 0x1c) = values[0];
    } else if (flags == 0x100) {
        *(float *)(key + 0x20) = values[0];
    } else if (flags == 0x1c0) {
        *(float *)(key + 0x18) = values[0];
        *(float *)(key + 0x1c) = values[1];
        *(float *)(key + 0x20) = values[2];
    } else if (flags == 0x200) {
        *(float *)(key + 0x48) = values[0] / old_angle_divisor;
    } else if (flags == 0x400) {
        *(float *)(key + 0x24) = -values[0];
    } else if (flags == 0x800) {
        *(float *)(key + 0x28) = values[0];
    } else if (flags == 0x1000) {
        *(float *)(key + 0x2c) = values[0];
    } else if (flags == 0x1c00) {
        *(float *)(key + 0x24) = -values[0];
        *(float *)(key + 0x28) = values[1];
        *(float *)(key + 0x2c) = values[2];
    } else if (flags == 0x2000) {
        *(float *)(key + 0x30) = values[0];
    } else if (flags == 0x4000) {
        *(float *)(key + 0x34) = -values[0];
    } else if (flags == 0x8000) {
        *(float *)(key + 0x38) = values[0];
    } else if (flags == 0xe000) {
        *(float *)(key + 0x30) = values[0];
        *(float *)(key + 0x34) = -values[1];
        *(float *)(key + 0x38) = values[2];
    } else if (flags == 0x10000) {
        *(float *)(key + 0x3c) = values[0];
    } else if (flags == 0x20000) {
        *(float *)(key + 0x40) = values[0];
    } else if (flags == 0x40000) {
        *(float *)(key + 0x44) = values[0];
    } else if (flags == 0x70000) {
        *(float *)(key + 0x3c) = values[0];
        *(float *)(key + 0x40) = values[1];
        *(float *)(key + 0x44) = values[2];
    }
}

void Transform::InsertKeyFrame_old(const float *values, longlong flags, int time) {
    ++*g_transform_insert_counter;

    char *self = (char *)this;
    longlong timestamp = time;
    if (*(longlong *)(self + 0xf8) < timestamp) {
        *(longlong *)(self + 0xf8) = timestamp;
    }

    uint index = 0;
    while (index < *(uint *)(self + 0x11c) &&
           *(longlong *)((*(char ***)(self + 0x120))[index] + 0x50) < timestamp) {
        ++index;
    }

    char *key = (char *)operator new(0x60);
    new (key) KeyFrame();
    if (index != 0) {
        char *prev = (*(char ***)(self + 0x120))[index - 1];
        *(AEMath::Vector *)(key + 0x00) = *(AEMath::Vector *)(prev + 0x00);
        *(AEMath::Vector *)(key + 0x0c) = *(AEMath::Vector *)(prev + 0x0c);
        *(AEMath::Vector *)(key + 0x18) = *(AEMath::Vector *)(prev + 0x18);
        *(longlong *)(key + 0x50) = *(longlong *)(prev + 0x50);
        *(longlong *)(key + 0x58) = *(longlong *)(prev + 0x58);
    }

    *(longlong *)(key + 0x50) = timestamp;
    *(uint *)(key + 0x58) |= (uint)flags;
    *(uint *)(key + 0x5c) |= (uint)((ulonglong)flags >> 32);
    apply_value_old(this, key, values, flags);

    if (*(uint *)(self + 0x11c) == 0) {
        ArrayAdd((KeyFrame *)key, *(Array<KeyFrame *> *)(self + 0x11c));
    } else if (index == *(uint *)(self + 0x11c)) {
        char *last = (*(char ***)(self + 0x120))[*(uint *)(self + 0x11c) - 1];
        uint mask = *(uint *)(key + 0x58);
        for (int off = 0; off <= 0x48; off += 4) {
            if ((mask & (1u << (off / 4))) == 0) {
                *(int *)(key + off) = *(int *)(last + off);
            }
        }
        ArrayAdd((KeyFrame *)key, *(Array<KeyFrame *> *)(self + 0x11c));
    } else {
        char *existing = (*(char ***)(self + 0x120))[index];
        if (*(longlong *)(existing + 0x50) == timestamp) {
            *(uint *)(existing + 0x58) |= *(uint *)(key + 0x58);
            *(uint *)(existing + 0x5c) |= *(uint *)(key + 0x5c);
            apply_value_old(this, existing, values, flags);
            UpdateKeyFrames((KeyFrame *)existing, index);
            operator delete(key);
            return;
        }
        InsertKeyFrame((KeyFrame *)key, index);
        UpdateKeyFrames((KeyFrame *)key, index);
    }
}

} // namespace AbyssEngine
