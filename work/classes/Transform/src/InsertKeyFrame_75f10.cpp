#include "class.h"

namespace AbyssEngine {

static float kAngleDivisor = 57.295780f;

static void copy_vec(char *dst, const char *src) {
    *(AEMath::Vector *)dst = *(const AEMath::Vector *)src;
}

static void lerp_vec(char *dst, const char *from, const char *to, float t) {
    AEMath::Vector value = AEMath::VectorLerp(*(const AEMath::Vector *)from,
                                              *(const AEMath::Vector *)to, t);
    *(AEMath::Vector *)dst = value;
}

static void apply_value_new(Transform *owner, char *key, const float *values, longlong flags) {
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
        *(float *)(key + 0x48) = values[0] / kAngleDivisor;
    } else if (flags == 0x400) {
        *(float *)(key + 0x24) = -values[0];
    } else if (flags == 0x800) {
        *(float *)(key + 0x28) = values[0];
    } else if (flags == 0x1000) {
        *(float *)(key + 0x2c) = values[0];
    } else if (flags == 0x1c00) {
        *(float *)(key + 0x24) = values[0];
        *(float *)(key + 0x28) = values[1];
        *(float *)(key + 0x2c) = values[2];
    } else if (flags == 0x2000) {
        *(float *)(key + 0x30) = values[0];
    } else if (flags == 0x4000) {
        *(float *)(key + 0x34) = values[0];
    } else if (flags == 0x8000) {
        *(float *)(key + 0x38) = values[0];
    } else if (flags == 0xe000) {
        *(float *)(key + 0x30) = values[0];
        *(float *)(key + 0x34) = values[1];
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

void Transform::InsertKeyFrame(const float *values, longlong flags, int time) {
    ++*g_transform_insert_counter;

    char *self = (char *)this;
    longlong timestamp = time;
    if (*(longlong *)(self + 0xf8) < timestamp) {
        *(longlong *)(self + 0xf8) = timestamp;
    }

    uint index = 0;
    while (index < *(uint *)(self + 0x11c)) {
        char *existing = (*(char ***)(self + 0x120))[index];
        longlong existingTime = *(longlong *)(existing + 0x50);
        if (existingTime >= timestamp) {
            if (existingTime == timestamp) {
                *(uint *)(existing + 0x58) |= (uint)flags;
                *(uint *)(existing + 0x5c) |= (uint)((ulonglong)flags >> 32);
                apply_value_new(this, existing, values, flags);
                UpdateKeyFrames((KeyFrame *)existing, index);
                return;
            }
            break;
        }
        ++index;
    }

    char *key = (char *)operator new(0x60);
    new (key) KeyFrame();
    *(longlong *)(key + 0x50) = timestamp;
    InsertKeyFrame((KeyFrame *)key, index);

    if (index != 0) {
        char **items = *(char ***)(self + 0x120);
        char *prev = items[index - 1];
        if (index < *(uint *)(self + 0x11c) - 1) {
            char *next = items[index + 1];
            float t = (float)(timestamp - *(longlong *)(prev + 0x50)) /
                      (float)(*(longlong *)(next + 0x50) - *(longlong *)(prev + 0x50));
            lerp_vec(key + 0x00, prev + 0x00, next + 0x00, t);
            lerp_vec(key + 0x0c, prev + 0x0c, next + 0x0c, t);
            lerp_vec(key + 0x18, prev + 0x18, next + 0x18, t);
            *(float *)(key + 0x48) = *(float *)(prev + 0x48) +
                                     t * (*(float *)(next + 0x48) - *(float *)(prev + 0x48));
            lerp_vec(key + 0x24, prev + 0x24, next + 0x24, t);
            lerp_vec(key + 0x30, prev + 0x30, next + 0x30, t);
            lerp_vec(key + 0x3c, prev + 0x3c, next + 0x3c, t);
        } else {
            copy_vec(key + 0x00, prev + 0x00);
            copy_vec(key + 0x0c, prev + 0x0c);
            copy_vec(key + 0x18, prev + 0x18);
            *(int *)(key + 0x48) = *(int *)(prev + 0x48);
            copy_vec(key + 0x24, prev + 0x24);
            copy_vec(key + 0x30, prev + 0x30);
            copy_vec(key + 0x3c, prev + 0x3c);
        }
        *(uint *)(key + 0x58) |= *(uint *)(prev + 0x58);
        *(uint *)(key + 0x5c) |= *(uint *)(prev + 0x5c);
    }

    *(uint *)(key + 0x58) |= (uint)flags;
    *(uint *)(key + 0x5c) |= (uint)((ulonglong)flags >> 32);
    apply_value_new(this, key, values, flags);
    UpdateKeyFrames((KeyFrame *)key, index);
}

} // namespace AbyssEngine
