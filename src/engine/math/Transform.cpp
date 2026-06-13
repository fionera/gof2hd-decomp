#include "gof2/engine/math/Transform.h"




// ---- SetCurrentAnimationTime_77424.cpp ----
namespace AbyssEngine {

void Transform::SetCurrentAnimationTime(longlong value) {
    this->currentTime = value;
}

} // namespace AbyssEngine

// ---- SetAnimationRangeInKeyFrames_74fb4.cpp ----
namespace AbyssEngine {

void Transform::SetAnimationRangeInKeyFrames(int first, int last) {
    longlong start;
    if (first < 0 || this->keyFrameCount <= first) {
        start = 0;
    } else {
        char *keyFrame = (char *)this->keyFrames[first];
        start = *(longlong *)(keyFrame + 0x50);
    }

    longlong end;
    if (last >= 0 && last <= this->keyFrameCount) {
        char *keyFrame = (char *)this->keyFrames[last];
        end = *(longlong *)(keyFrame + 0x50);
    } else {
        end = 0;
    }
    SetAnimationRangeInTime(start, end);
}

} // namespace AbyssEngine

// ---- SetVisible_77630.cpp ----
namespace AbyssEngine {

void Transform::SetVisible(bool value) {
    this->visible = value;
}

} // namespace AbyssEngine

// ---- SetVFCFlag_7742a.cpp ----
namespace AbyssEngine {

void Transform::SetVFCFlag(bool value) {
    for (uint i = 0; i < this->meshCount; ++i) {
        char **meshes = this->meshes;
        char *mesh = meshes[i];
        if (mesh != 0) {
            Transform *child = *(Transform **)(mesh + 0x34);
            if (child != 0) {
                child->SetVFCFlag(value);
            }
        }
    }
    for (uint i = 0; i < this->childCount; ++i) {
        Transform **children = this->children;
        Transform *child = children[i];
        if (child != 0) {
            child->SetVFCFlag(value);
        }
    }
    this->vfcEnabled = value;
}

} // namespace AbyssEngine

// ---- PauseAnimationWithKeyFrame_7500e.cpp ----
namespace AbyssEngine {

void Transform::PauseAnimationWithKeyFrame(int index) {
    if (index >= 0 && index < this->keyFrameCount) {
        char *keyFrame = (char *)this->keyFrames[index];
        longlong time = *(longlong *)(keyFrame + 0x50);
        this->currentTime = time;
        InternUpdate(time, false);
    }
    this->animating = false;
}

} // namespace AbyssEngine

// ---- PauseAnimationWithTimeStamp_75888.cpp ----
namespace AbyssEngine {

void Transform::PauseAnimationWithTimeStamp(longlong time) {
    this->currentTime = time;
    InternUpdate(time, false);
    this->animating = false;
}

} // namespace AbyssEngine

// ---- SetAnimationSpeed_74c5c.cpp ----
namespace AbyssEngine {

void Transform::SetAnimationSpeed(float value) {
    this->animationSpeed = value;
}

} // namespace AbyssEngine

// ---- CollectAnimationData_74b9a.cpp ----
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
                longlong childLength = child->animationLength;
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
        longlong childLength = child->animationLength;
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

// ---- _Transform_775a8.cpp ----
namespace AbyssEngine {

Transform::~Transform() {
    char *self = (char *)this;
    Array<KeyFrame *> *keyFrames = (Array<KeyFrame *> *)(self + 0x11c);
    if (*(bool *)(self + 0x17d) == false) {
        ArrayReleaseClasses(keyFrames);
    }
    ((Quaternion *)(self + 0x150))->~Quaternion();
    ((Quaternion *)(self + 0x128))->~Quaternion();
    keyFrames->~vector();
    ((Array<Transform *> *)(self + 0x4c))->~vector();
    ((Array<Mesh *> *)(self + 0x3c))->~vector();
}

} // namespace AbyssEngine

// ---- InitAnimationRangeInTime_74c62.cpp ----
namespace AbyssEngine {

void Transform::InitAnimationRangeInTime() {
    longlong length = this->animationLength;
    if (length == 0) {
        return;
    }

    longlong start = this->animationStart;
    this->rangeStart = start;
    this->rangeEnd = length;
    this->currentTime = start;

    for (uint i = 0; i < this->meshCount; ++i) {
        char **meshes = this->meshes;
        char *mesh = meshes[i];
        if (mesh != 0) {
            Transform *child = *(Transform **)(mesh + 0x34);
            if (child != 0) {
                child->InitAnimationRangeInTime();
            }
        }
    }
    for (uint i = 0; i < this->childCount; ++i) {
        Transform **children = this->children;
        children[i]->InitAnimationRangeInTime();
    }
    Update(0, false);
}

} // namespace AbyssEngine

// ---- IsRunning_773fa.cpp ----
namespace AbyssEngine {

int Transform::IsRunning() {
    longlong start = this->rangeStart;
    longlong current = this->currentTime;
    if (start < current &&
        current < this->rangeEnd &&
        this->animating) {
        return 1;
    }
    return 0;
}

} // namespace AbyssEngine

// ---- UpdateKeyFrames_75a08.cpp ----
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

// ---- SetAnimationLength_75004.cpp ----
namespace AbyssEngine {

void Transform::SetAnimationLength(longlong value) {
    this->rangeEnd = value;
    this->animationLength = value;
}

} // namespace AbyssEngine

// ---- SetAnimationRangeInTime_74e90.cpp ----
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

// ---- InsertKeyFrame_759c4.cpp ----
namespace AbyssEngine {

void Transform::InsertKeyFrame(KeyFrame *keyFrame, int index) {
    ArrayAdd((KeyFrame *)0, *(Array<KeyFrame *> *)((char *)this + 0x11c));
    int count = this->keyFrameCount;
    int from = count - 2;
    int to = count - 1;
    while (index < to) {
        KeyFrame **items = this->keyFrames;
        items[to] = items[from];
        --from;
        --to;
    }
    KeyFrame **items = this->keyFrames;
    items[index] = keyFrame;
}

} // namespace AbyssEngine

// ---- Update_74cc8.cpp ----
namespace AbyssEngine {

void Transform::Update(longlong time, bool updateBounds) {
    InternUpdate(time, updateBounds);
}

// 32-bit-ABI faces of Update(). The decompiled call sites pass the 64-bit
// animation time as two 32-bit halves: Update32 receives it already as the
// `elapsed` longlong (Explosion advances a secondary mesh by the frame delta),
// while UpdatePtr reassembles it from the (lo, hi) words a sibling transform
// stores at +0xf8/+0xfc (its animationLength). Both then run the real Update.
void Transform::Update32(uint32_t /*high*/, longlong elapsed, uint32_t updateBounds) {
    Update(elapsed, updateBounds != 0);
}

void Transform::UpdatePtr(uint32_t lo, uint32_t hi, int updateBounds) {
    longlong time = (longlong)(((uint64_t)hi << 32) | (uint64_t)lo);
    Update(time, updateBounds != 0);
}

// Show / hide this transform. The active flag travels with the exhaust meshes, so
// activation is the same operation as toggling visibility.
void Transform::SetActive()   { setExhaustVisible(true); }
void Transform::SetInactive() { setExhaustVisible(false); }

// Toggle the visibility of the transform's exhaust meshes. PlayerFixedObject /
// PlayerEgo call this through the canvas-resolved transform to switch the engine
// glow on or off.
void Transform::setExhaustVisible(bool visible) {
    SetVisible(visible);
}

} // namespace AbyssEngine

// ---- Transform_74918.cpp ----
namespace AbyssEngine {

Transform::Transform(Transform *other) {
    char *self = (char *)this;

    new ((AEMath::Matrix *)(self + 0x00)) AEMath::Matrix();
    new ((Array<Mesh *> *)(self + 0x3c)) Array<Mesh *>();
    new ((Array<Transform *> *)(self + 0x4c)) Array<Transform *>();
    new ((AEMath::Matrix *)(self + 0x5c)) AEMath::Matrix();
    new ((AEMath::Matrix *)(self + 0x98)) AEMath::Matrix();

    *(float *)(self + 0xd4) = 0.0f;
    *(float *)(self + 0xd8) = 0.0f;
    *(float *)(self + 0xdc) = 0.0f;
    *(float *)(self + 0xe0) = 0.0f;
    *(float *)(self + 0xe4) = 1.0f;

    new ((Array<KeyFrame *> *)(self + 0x11c)) Array<KeyFrame *>();
    new ((Quaternion *)(self + 0x128)) Quaternion();
    *(longlong *)(self + 0x148) = 0;
    *(AEMath::Vector *)(self + 0x138) = AEMath::Vector{0.0f, 0.0f, 0.0f};
    new ((Quaternion *)(self + 0x150)) Quaternion();
    *(longlong *)(self + 0x170) = 0;
    *(AEMath::Vector *)(self + 0x160) = AEMath::Vector{0.0f, 0.0f, 0.0f};

    if (other == 0) {
        Transform temp;
        temp.~Transform();
    } else {
        char *src = (char *)other;
        *(bool *)(self + 0xed) = true;
        *(int *)(self + 0xe8) = *(int *)(src + 0xe8);
        *(float *)(self + 0xf0) = 1.0f;
        *(int *)(self + 0x48) = -1;
        *(longlong *)(self + 0xf8) = *(longlong *)(src + 0xf8);
        *(longlong *)(self + 0x100) = *(longlong *)(src + 0x100);
        *(longlong *)(self + 0x108) = *(longlong *)(src + 0x108);
        *(longlong *)(self + 0x110) = *(longlong *)(src + 0x110);
        *(int *)(self + 0x118) = *(int *)(src + 0x118);
        *(Quaternion *)(self + 0x128) = *(Quaternion *)(src + 0x128);
        *(AEMath::Vector *)(self + 0x138) = *(AEMath::Vector *)(src + 0x138);
        *(AEMath::Vector *)(self + 0x144) = *(AEMath::Vector *)(src + 0x144);
        *(AEMath::Matrix *)(self + 0x5c) = *(AEMath::Matrix *)(src + 0x5c);
        *(Quaternion *)(self + 0x150) = *(Quaternion *)(src + 0x150);
        *(AEMath::Vector *)(self + 0x160) = *(AEMath::Vector *)(src + 0x160);
        *(AEMath::Vector *)(self + 0x16c) = *(AEMath::Vector *)(src + 0x16c);
        *(AEMath::Matrix *)(self + 0x98) = *(AEMath::Matrix *)(src + 0x98);
        *(int *)(self + 0x58) = *(int *)(src + 0x58);

        for (uint i = 0; i < *(uint *)(src + 0x3c); ++i) {
            Mesh *mesh = (Mesh *)operator new(0x88);
            new (mesh) Mesh((*(Mesh ***)(src + 0x40))[i]);
            ArrayAdd(mesh, *(Array<Mesh *> *)(self + 0x3c));
        }
        ArraySet((Array<KeyFrame *> *)(src + 0x11c), (Array<KeyFrame *> *)(self + 0x11c));
        for (uint i = 0; i < *(uint *)(src + 0x4c); ++i) {
            Transform *child = (Transform *)operator new(0x180);
            new (child) Transform((*(Transform ***)(src + 0x50))[i]);
            ArrayAdd(child, *(Array<Transform *> *)(self + 0x4c));
        }
        *(AEMath::BSphere *)(self + 0xd4) = *(AEMath::BSphere *)(src + 0xd4);
        *(bool *)(self + 0xec) = true;
        *(uint16_t *)(self + 0x17c) = 0x101;
    }
}

} // namespace AbyssEngine

// ---- SetAnimationState_74c3c.cpp ----
namespace AbyssEngine {

void Transform::SetAnimationState(AnimationMode, void *) {
}

} // namespace AbyssEngine

// ---- Transform_746a0.cpp ----
namespace AbyssEngine {

static void make_identity(AEMath::Matrix &m) {
    for (int i = 0; i < 15; ++i) {
        m.m[i] = 0.0f;
    }
    m.m[0] = 1.0f;
    m.m[5] = 1.0f;
    m.m[10] = 1.0f;
    m.m[14] = 1.0f;
}

Transform::Transform() {
    char *self = (char *)this;

    new ((AEMath::Matrix *)(self + 0x00)) AEMath::Matrix();
    new ((Array<Mesh *> *)(self + 0x3c)) Array<Mesh *>();
    new ((Array<Transform *> *)(self + 0x4c)) Array<Transform *>();
    new ((AEMath::Matrix *)(self + 0x5c)) AEMath::Matrix();
    new ((AEMath::Matrix *)(self + 0x98)) AEMath::Matrix();

    *(float *)(self + 0xd4) = 0.0f;
    *(float *)(self + 0xd8) = 0.0f;
    *(float *)(self + 0xdc) = 0.0f;
    *(float *)(self + 0xe0) = 0.0f;
    *(float *)(self + 0xe4) = 1.0f;

    new ((Array<KeyFrame *> *)(self + 0x11c)) Array<KeyFrame *>();
    new ((Quaternion *)(self + 0x128)) Quaternion();
    *(float *)(self + 0x138) = 0.0f;
    *(float *)(self + 0x13c) = 0.0f;
    *(float *)(self + 0x140) = 0.0f;
    *(float *)(self + 0x144) = 0.0f;
    *(longlong *)(self + 0x148) = 0;
    new ((Quaternion *)(self + 0x150)) Quaternion();

    *(longlong *)(self + 0xf8) = 0;
    *(longlong *)(self + 0x170) = 0;
    *(int *)(self + 0x48) = -1;
    *(bool *)(self + 0xed) = true;
    *(float *)(self + 0xf0) = 1.0f;
    *(longlong *)(self + 0x110) = 0;
    *(int *)(self + 0x118) = 0;
    *(longlong *)(self + 0x100) = 0;
    *(longlong *)(self + 0x108) = 0;

    Quaternion q;
    *(Quaternion *)(self + 0x150) = q;
    *(Quaternion *)(self + 0x128) = q;
    q.~Quaternion();

    AEMath::Vector zero = {0.0f, 0.0f, 0.0f};
    *(AEMath::Vector *)(self + 0x160) = zero;
    *(AEMath::Vector *)(self + 0x138) = zero;
    AEMath::Vector one = {1.0f, 1.0f, 1.0f};
    *(AEMath::Vector *)(self + 0x16c) = one;
    *(AEMath::Vector *)(self + 0x144) = one;

    AEMath::Matrix identity;
    make_identity(identity);
    *(AEMath::Matrix *)(self + 0x98) = identity;
    *(AEMath::Matrix *)(self + 0x5c) = identity;

    *(int *)(self + 0x58) = 2;
    *(float *)(self + 0xac) = 1.0f;
    *(uint16_t *)(self + 0x17c) = 1;
    *(bool *)(self + 0xec) = true;
    *(int *)(self + 0xe8) = 0;
}

} // namespace AbyssEngine

// ---- InternUpdate_75048.cpp ----
namespace AbyssEngine {

static void identity_matrix(AEMath::Matrix &m) {
    for (int i = 0; i < 15; ++i) m.m[i] = 0.0f;
    m.m[0] = 1.0f;
    m.m[5] = 1.0f;
    m.m[10] = 1.0f;
    m.m[14] = 1.0f;
}

static float clamp_positive_byte(float value) {
    int v = (int)(value * 255.0f);
    return (float)(v < 0 ? 0 : v);
}

void Transform::InternUpdate(longlong time, bool updateBounds) {
    char *self = (char *)this;

    if (*(int *)(self + 0x11c) != 0) {
        longlong current = *(longlong *)(self + 0x100);
        if (current < time) current = time;

        char **items = *(char ***)(self + 0x120);
        char *last = items[*(int *)(self + 0x11c) - 1];
        if (*(longlong *)(last + 0x50) < current) {
            current = *(longlong *)(last + 0x50);
        }

        int index = 0;
        while (*(longlong *)(items[index] + 0x50) < current) {
            ++index;
        }
        *(int *)(self + 0x118) = index;

        if (index == 0) {
            AEMath::Matrix identity;
            identity_matrix(identity);
            *(AEMath::Matrix *)(self + 0x98) = identity;
            if (!g_transform_matrix_flag) {
                *(float *)(self + 0xac) = 1.0f;
            }
        } else if (current == 0) {
            char *key = items[index];
            Quaternion q; q.Set(*(float *)(key + 0x18), *(float *)(key + 0x1c), *(float *)(key + 0x20));
            *(Quaternion *)(self + 0x128) = q;
            q.~Quaternion();
            *(AEMath::Vector *)(self + 0x138) = *(AEMath::Vector *)(key + 0x00);
            *(AEMath::Vector *)(self + 0x144) = *(AEMath::Vector *)(key + 0x0c);
            *(float *)(self + 0x178) = *(float *)(key + 0x48);
            Quaternion q2; q2.Set(*(float *)(key + 0x3c), *(float *)(key + 0x40), *(float *)(key + 0x44));
            *(Quaternion *)(self + 0x150) = q2;
            q2.~Quaternion();
            *(AEMath::Vector *)(self + 0x160) = *(AEMath::Vector *)(key + 0x24);
            *(AEMath::Vector *)(self + 0x16c) = *(AEMath::Vector *)(key + 0x30);
        } else {
            char *prev = items[index - 1];
            char *next = items[index];
            float t = (float)(current - *(longlong *)(prev + 0x50)) /
                      (float)(*(longlong *)(next + 0x50) - *(longlong *)(prev + 0x50));

            Quaternion qa; qa.Set(*(float *)(prev + 0x18), *(float *)(prev + 0x1c), *(float *)(prev + 0x20));
            Quaternion qb; qb.Set(*(float *)(next + 0x18), *(float *)(next + 0x1c), *(float *)(next + 0x20));
            Quaternion q;
            q.Lerp(qa, qb, t);
            *(Quaternion *)(self + 0x128) = q;
            q.~Quaternion();
            qb.~Quaternion();
            qa.~Quaternion();

            *(AEMath::Vector *)(self + 0x138) = AEMath::VectorLerp(
                *(AEMath::Vector *)(prev + 0x00), *(AEMath::Vector *)(next + 0x00), t);
            *(AEMath::Vector *)(self + 0x144) = AEMath::VectorLerp(
                *(AEMath::Vector *)(prev + 0x0c), *(AEMath::Vector *)(next + 0x0c), t);
            *(float *)(self + 0x178) = *(float *)(prev + 0x48) +
                                       t * (*(float *)(next + 0x48) - *(float *)(prev + 0x48));

            Quaternion qc; qc.Set(*(float *)(prev + 0x3c), *(float *)(prev + 0x40), *(float *)(prev + 0x44));
            Quaternion qd; qd.Set(*(float *)(next + 0x3c), *(float *)(next + 0x40), *(float *)(next + 0x44));
            Quaternion qout;
            qout.Lerp(qc, qd, t);
            *(Quaternion *)(self + 0x150) = qout;
            qout.~Quaternion();
            qd.~Quaternion();
            qc.~Quaternion();

            *(AEMath::Vector *)(self + 0x160) = AEMath::VectorLerp(
                *(AEMath::Vector *)(prev + 0x24), *(AEMath::Vector *)(next + 0x24), t);
            *(AEMath::Vector *)(self + 0x16c) = AEMath::VectorLerp(
                *(AEMath::Vector *)(prev + 0x30), *(AEMath::Vector *)(next + 0x30), t);
        }

        uint alpha = (uint)clamp_positive_byte(*(float *)(self + 0x178));
        *(uint *)(self + 0x48) = alpha | (alpha << 8) | (alpha << 16) | (alpha << 24);

        AEMath::Matrix rotation;
        identity_matrix(rotation);
        ((Quaternion *)(self + 0x128))->Convert(rotation);
        *(AEMath::Matrix *)(self + 0x5c) = rotation;

        AEMath::Matrix scale;
        AEMath::MatrixSetScaling(scale, *(float *)(self + 0x144),
                                 *(float *)(self + 0x148), *(float *)(self + 0x14c));
        AEMath::MatrixSetTranslation(scale, *(float *)(self + 0x138),
                                     *(float *)(self + 0x13c), *(float *)(self + 0x140));

        AEMath::Matrix local;
        ((Quaternion *)(self + 0x150))->Convert(local);
        AEMath::Matrix composed = AEMath::operator*(local, scale);
        *(AEMath::Matrix *)(self + 0x98) = composed;
    }

    if (!updateBounds) {
        AEMath::BSphere sphere;
        sphere.center.x = 0.0f;
        sphere.center.y = 0.0f;
        sphere.center.z = 0.0f;
        sphere.radius = 0.0f;
        sphere.radius2 = 1.0f;
        *(AEMath::BSphere *)(self + 0xd4) = sphere;

        for (uint i = 0; i < *(uint *)(self + 0x3c); ++i) {
            char *mesh = (*(char ***)(self + 0x40))[i];
            AEMath::BSphere childSphere;
            if (mesh != 0 && *(Transform **)(mesh + 0x34) != 0) {
                (*(Transform **)(mesh + 0x34))->InternUpdate(time, updateBounds);
            }
            childSphere = *(AEMath::BSphere *)(mesh + 0x3c);
            ((AEMath::BSphere *)(self + 0xd4))->Merge(childSphere);
        }

        for (uint i = 0; i < *(uint *)(self + 0x4c); ++i) {
            Transform *child = (*(Transform ***)(self + 0x50))[i];
            child->InternUpdate(time, updateBounds);
            AEMath::BSphere childSphere = *(AEMath::BSphere *)((char *)child + 0xd4);
            ((AEMath::BSphere *)(self + 0xd4))->Merge(childSphere);
        }
    }
}

} // namespace AbyssEngine

// ---- AddKeyFrame_758a0.cpp ----
namespace AbyssEngine {

void Transform::AddKeyFrame(const AEMath::Vector &a, const AEMath::Vector &b,
                            const AEMath::Vector &c, const AEMath::Vector &d,
                            const AEMath::Vector &e, const AEMath::Vector &f,
                            int time) {
    char *keyFrame = (char *)operator new(0x60);
    new (keyFrame) KeyFrame();
    *(AEMath::Vector *)(keyFrame + 0x18) = b;
    *(AEMath::Vector *)(keyFrame + 0x00) = c;
    *(AEMath::Vector *)(keyFrame + 0x0c) = a;
    *(AEMath::Vector *)(keyFrame + 0x3c) = e;
    *(AEMath::Vector *)(keyFrame + 0x24) = f;
    *(AEMath::Vector *)(keyFrame + 0x30) = d;
    longlong timestamp = time;
    *(longlong *)(keyFrame + 0x50) = timestamp;
    ArrayAdd((KeyFrame *)keyFrame, *(Array<KeyFrame *> *)((char *)this + 0x11c));
    if (this->animationLength < timestamp) {
        this->animationLength = timestamp;
    }
}

} // namespace AbyssEngine

// ---- InsertKeyFrame_75f10.cpp ----
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
        if (owner->field_0xe4 < values[0]) owner->field_0xe4 = values[0];
    } else if (flags == 0x20) {
        *(float *)(key + 0x14) = values[0];
        if (owner->field_0xe4 < values[0]) owner->field_0xe4 = values[0];
    } else if (flags == 0x38) {
        *(float *)(key + 0x0c) = values[0];
        *(float *)(key + 0x10) = values[1];
        *(float *)(key + 0x14) = values[2];
        float max = values[1] < values[0] ? values[0] : values[1];
        if (values[2] > max) max = values[2];
        if (owner->field_0xe4 < max) owner->field_0xe4 = max;
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

// ---- InsertKeyFrame_old_7640c.cpp ----
namespace AbyssEngine {

static float old_angle_divisor = 57.295780f;

static void apply_value_old(Transform *owner, char *key, const float *values, longlong flags) {
    if (flags == 0x10) {
        *(float *)(key + 0x10) = values[0];
        if (owner->field_0xe4 < values[0]) owner->field_0xe4 = values[0];
    } else if (flags == 0x20) {
        *(float *)(key + 0x14) = values[0];
        if (owner->field_0xe4 < values[0]) owner->field_0xe4 = values[0];
    } else if (flags == 0x38) {
        *(float *)(key + 0x0c) = values[0];
        *(float *)(key + 0x10) = values[1];
        *(float *)(key + 0x14) = values[2];
        float max = values[1] < values[0] ? values[0] : values[1];
        if (values[2] > max) max = values[2];
        if (owner->field_0xe4 < max) owner->field_0xe4 = max;
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

// ---- InCameraVF_77478.cpp ----
namespace AbyssEngine {

int Transform::InCameraVF(AEMath::Matrix *matrix, Camera *camera) {
    if (camera == 0 || this->vfcEnabled == false ||
        (this->meshCount == 1 &&
         *(uint16_t *)(*(int *)(this->meshes) + 2) == 0)) {
        return 1;
    }

    AEMath::Vector center;
    center.x = 0.0f;
    center.y = 0.0f;
    center.z = 0.0f;

    AEMath::Vector radiusVector;
    radiusVector.x = this->boundingRadius;
    radiusVector.y = radiusVector.x;
    radiusVector.z = radiusVector.x;

    AEMath::Vector rotated;
    if (matrix == 0) {
        AEMath::Vector transformed = AEMath::MatrixTransformVector(
            *(AEMath::Matrix *)this, this->boundingCenter);
        center = transformed;
        rotated = AEMath::MatrixRotateVector(*(AEMath::Matrix *)this, radiusVector);
    } else {
        AEMath::Matrix combined = AEMath::operator*(*matrix, *(AEMath::Matrix *)this);
        AEMath::Vector transformed = AEMath::MatrixTransformVector(
            combined, this->boundingCenter);
        center = transformed;
        combined = AEMath::operator*(*matrix, *(AEMath::Matrix *)this);
        rotated = AEMath::MatrixRotateVector(combined, radiusVector);
    }

    radiusVector = rotated;
    float x = radiusVector.x < 0.0f ? -radiusVector.x : radiusVector.x;
    float y = radiusVector.y < 0.0f ? -radiusVector.y : radiusVector.y;
    if (x < y) {
        x = y;
    }
    float z = radiusVector.z < 0.0f ? -radiusVector.z : radiusVector.z;
    if (x < z) {
        x = z;
    }

    return CameraIsSphereinViewFrustum(center, this->field_0xe4 * x, camera);
}

} // namespace AbyssEngine
