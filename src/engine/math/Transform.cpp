#include "engine/math/Transform.h"
#include "engine/core/KeyFrame.h"
#include "engine/render/Mesh.h"

// Engine globals: shared keyframe-insertion counter and the matrix-fixup toggle.
extern int *g_transform_insert_counter;
extern bool g_transform_matrix_flag;

namespace AbyssEngine {

void Transform::SetCurrentAnimationTime(longlong value) {
    this->currentTime = value;
}

void Transform::SetAnimationRangeInKeyFrames(int first, int last) {
    longlong start;
    if (first < 0 || (int)this->keyFrames.size() <= first) {
        start = 0;
    } else {
        char *keyFrame = (char *)this->keyFrames[first];
        start = *(longlong *)(keyFrame + 0x50);
    }

    longlong end;
    if (last >= 0 && last <= (int)this->keyFrames.size()) {
        char *keyFrame = (char *)this->keyFrames[last];
        end = *(longlong *)(keyFrame + 0x50);
    } else {
        end = 0;
    }
    SetAnimationRangeInTime(start, end);
}

void Transform::SetVisible(bool value) {
    this->visible = value;
}

void Transform::SetVFCFlag(bool value) {
    for (uint i = 0; i < this->meshes.size(); ++i) {
        Mesh *mesh = this->meshes[i];
        if (mesh != 0) {
            Transform *child = mesh->animation;
            if (child != 0) {
                child->SetVFCFlag(value);
            }
        }
    }
    for (uint i = 0; i < this->children.size(); ++i) {
        Transform *child = this->children[i];
        if (child != 0) {
            child->SetVFCFlag(value);
        }
    }
    this->vfcEnabled = value;
}

void Transform::PauseAnimationWithKeyFrame(int index) {
    if (index >= 0 && index < (int)this->keyFrames.size()) {
        char *keyFrame = (char *)this->keyFrames[index];
        longlong time = *(longlong *)(keyFrame + 0x50);
        this->currentTime = time;
        InternUpdate(time, false);
    }
    this->animating = false;
}

void Transform::PauseAnimationWithTimeStamp(longlong time) {
    this->currentTime = time;
    InternUpdate(time, false);
    this->animating = false;
}

void Transform::SetAnimationSpeed(float value) {
    this->animationSpeed = value;
}

void Transform::CollectAnimationData() {
    longlong *length = &this->animationLength;

    for (uint i = 0; i < this->meshes.size(); ++i) {
        Mesh *mesh = this->meshes[i];
        if (mesh != 0) {
            Transform *child = mesh->animation;
            if (child != 0) {
                child->CollectAnimationData();
                child = this->meshes[i]->animation;
                longlong childLength = child->animationLength;
                if (*length < childLength) {
                    *length = childLength;
                }
            }
        }
    }

    for (uint i = 0; i < this->children.size(); ++i) {
        Transform *child = this->children[i];
        child->CollectAnimationData();
        child = this->children[i];
        longlong childLength = child->animationLength;
        if (*length < childLength) {
            *length = childLength;
        }
    }

    size_t count = this->keyFrames.size();
    if (count != 0) {
        char *keyFrame = (char *)this->keyFrames[count - 1];
        longlong keyTime = *(longlong *)(keyFrame + 0x50);
        if (*length < keyTime) {
            *length = keyTime;
        }
    }
}

Transform::~Transform() {
    if (this->keyFramesShared == false) {
        for (KeyFrame *kf : this->keyFrames) {
            delete kf;
        }
        this->keyFrames.clear();
    }
    // rotation / localRotation are real members; their dtors run automatically.
}

void Transform::InitAnimationRangeInTime() {
    longlong length = this->animationLength;
    if (length == 0) {
        return;
    }

    longlong start = this->animationStart;
    this->rangeStart = start;
    this->rangeEnd = length;
    this->currentTime = start;

    for (uint i = 0; i < this->meshes.size(); ++i) {
        Mesh *mesh = this->meshes[i];
        if (mesh != 0) {
            Transform *child = mesh->animation;
            if (child != 0) {
                child->InitAnimationRangeInTime();
            }
        }
    }
    for (uint i = 0; i < this->children.size(); ++i) {
        this->children[i]->InitAnimationRangeInTime();
    }
    Update(0, false);
}

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

static float lerp_float(float from, float to, float t) {
    return from + t * (to - from);
}

void Transform::UpdateKeyFrames(KeyFrame *keyFrame, int index) {
    char *key = (char *)keyFrame;  // KeyFrame fields accessed via key+0xNN

    Array<KeyFrame *> &items = this->keyFrames;
    int i = 0;
    while (i + 1 < index) {
        char *next = (char *)items[i + 1];
        char *prev = (char *)items[i];
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

    uint count = (uint)this->keyFrames.size();
    while ((uint)++index < count) {
        char *dst = (char *)items[index];
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

void Transform::SetAnimationLength(longlong value) {
    this->rangeEnd = value;
    this->animationLength = value;
}

void Transform::SetAnimationRangeInTime(longlong start, longlong end) {
    longlong length = this->animationLength;
    if (length == 0) {
        return;
    }

    longlong rangeEnd = end;
    if (end < 0 || length < end) {
        rangeEnd = length;
    }
    this->rangeEnd = rangeEnd;

    longlong rangeStart = start;
    if (length < start || start < 0) {
        rangeStart = 0;
    }
    this->rangeStart = rangeStart;

    longlong current = this->currentTime;
    if (end < current) {
        current = end;
    }
    if (current < start) {
        current = start;
    }
    this->currentTime = current;

    for (uint i = 0; i < this->meshes.size(); ++i) {
        Mesh *mesh = this->meshes[i];
        if (mesh != 0) {
            Transform *child = mesh->animation;
            if (child != 0) {
                child->SetAnimationRangeInTime(start, end);
            }
        }
    }
    for (uint i = 0; i < this->children.size(); ++i) {
        this->children[i]->SetAnimationRangeInTime(start, end);
    }
    Update(0, false);
}

void Transform::InsertKeyFrame(KeyFrame *keyFrame, int index) {
    this->keyFrames.push_back((KeyFrame *)0);
    int count = (int)this->keyFrames.size();
    int from = count - 2;
    int to = count - 1;
    Array<KeyFrame *> &items = this->keyFrames;
    while (index < to) {
        items[to] = items[from];
        --from;
        --to;
    }
    items[index] = keyFrame;
}

void Transform::Update(longlong time, bool updateBounds) {
    InternUpdate(time, updateBounds);
}

// Diagnostic hook left in the shipping build: returns its argument unchanged.
int Transform::DebugOut(int value) {
    return value;
}

Transform::Transform(Transform *other) {
    // worldMatrix / rotationMatrix / localMatrix / rotation / localRotation are
    // real members and are default-constructed automatically.

    this->boundingCenter = AEMath::Vector{0.0f, 0.0f, 0.0f};
    this->boundingRadius = 0.0f;
    this->boundingRadius2 = 1.0f;
    this->scale.y = 0.0f;
    this->translation = AEMath::Vector{0.0f, 0.0f, 0.0f};
    this->localScale.y = 0.0f;
    this->localTranslation = AEMath::Vector{0.0f, 0.0f, 0.0f};

    if (other == 0) {
        Transform temp;
        temp.~Transform();
    } else {
        this->animating = true;
        this->animationStart = other->animationStart;
        this->animationSpeed = 1.0f;
        this->color = -1;
        this->animationLength = other->animationLength;
        this->rangeStart = other->rangeStart;
        this->rangeEnd = other->rangeEnd;
        this->currentTime = other->currentTime;
        this->currentKeyFrameIndex = other->currentKeyFrameIndex;
        this->rotation = other->rotation;
        this->translation = other->translation;
        this->scale = other->scale;
        this->rotationMatrix = other->rotationMatrix;
        this->localRotation = other->localRotation;
        this->localTranslation = other->localTranslation;
        this->localScale = other->localScale;
        this->localMatrix = other->localMatrix;
        this->renderMode = other->renderMode;

        for (uint i = 0; i < other->meshes.size(); ++i) {
            Mesh *mesh = new Mesh(other->meshes[i]);
            this->meshes.push_back(mesh);
        }
        this->keyFrames = other->keyFrames;
        for (uint i = 0; i < other->children.size(); ++i) {
            Transform *child = new Transform(other->children[i]);
            this->children.push_back(child);
        }
        this->bounds() = other->bounds();
        this->visible = true;
        this->vfcEnabled = true;
        this->keyFramesShared = true;
    }
}

void Transform::SetAnimationState(AnimationMode, void *) {
}

static inline __attribute__((always_inline)) void make_identity(AEMath::Matrix &m) {
    for (int i = 0; i < 15; ++i) {
        m.m[i] = 0.0f;
    }
    m.m[0] = 1.0f;
    m.m[5] = 1.0f;
    m.m[10] = 1.0f;
    m.m[14] = 1.0f;
}

Transform::Transform() {
    // worldMatrix / rotationMatrix / localMatrix / rotation / localRotation are
    // real members and are default-constructed automatically.

    this->boundingCenter = AEMath::Vector{0.0f, 0.0f, 0.0f};
    this->boundingRadius = 0.0f;
    this->boundingRadius2 = 1.0f;
    this->translation = AEMath::Vector{0.0f, 0.0f, 0.0f};
    this->scale.x = 0.0f;
    this->scale.y = 0.0f;

    this->animationLength = 0;
    this->localScale.y = 0.0f;
    this->color = -1;
    this->animating = true;
    this->animationSpeed = 1.0f;
    this->currentTime = 0;
    this->currentKeyFrameIndex = 0;
    this->rangeStart = 0;
    this->rangeEnd = 0;

    Quaternion q;
    this->localRotation = q;
    this->rotation = q;

    AEMath::Vector zero = {0.0f, 0.0f, 0.0f};
    this->localTranslation = zero;
    this->translation = zero;
    AEMath::Vector one = {1.0f, 1.0f, 1.0f};
    this->localScale = one;
    this->scale = one;

    AEMath::Matrix identity;
    make_identity(identity);
    this->localMatrix = identity;
    this->rotationMatrix = identity;

    this->renderMode = 2;
    this->localMatrix.m[5] = 1.0f;
    this->vfcEnabled = true;
    this->keyFramesShared = false;
    this->visible = true;
    this->animationStart = 0;
}

static float clamp_positive_byte(float value) {
    int v = (int)(value * 255.0f);
    return (float)(v < 0 ? 0 : v);
}

void Transform::InternUpdate(longlong time, bool updateBounds) {
    if (this->keyFrames.size() != 0) {
        longlong current = this->rangeStart;
        if (current < time) current = time;

        Array<KeyFrame *> &items = this->keyFrames;
        char *last = (char *)items[items.size() - 1];
        if (*(longlong *)(last + 0x50) < current) {  // KeyFrame+0x50 (timestamp)
            current = *(longlong *)(last + 0x50);     // KeyFrame+0x50
        }

        int index = 0;
        while (*(longlong *)((char *)items[index] + 0x50) < current) {  // KeyFrame+0x50
            ++index;
        }
        this->currentKeyFrameIndex = index;

        if (index == 0) {
            AEMath::Matrix identity;
            make_identity(identity);
            this->localMatrix = identity;
            if (!g_transform_matrix_flag) {
                this->localMatrix.m[5] = 1.0f;
            }
        } else if (current == 0) {
             // KeyFrame fields below (+0x18 rot, +0x00 pos, +0x0c scale, +0x48 alpha, +0x3c/+0x24/+0x30)
            char *key = (char *)items[index];
            this->rotation.Set(*(float *)(key + 0x18), *(float *)(key + 0x1c), *(float *)(key + 0x20));
            this->translation = *(AEMath::Vector *)(key + 0x00);
            this->scale = *(AEMath::Vector *)(key + 0x0c);
            this->alpha = *(float *)(key + 0x48);
            this->localRotation.Set(*(float *)(key + 0x3c), *(float *)(key + 0x40), *(float *)(key + 0x44));
            this->localTranslation = *(AEMath::Vector *)(key + 0x24);
            this->localScale = *(AEMath::Vector *)(key + 0x30);
        } else {
             // KeyFrame fields (prev/next) below
            char *prev = (char *)items[index - 1];
            char *next = (char *)items[index];
            float t = (float)(current - *(longlong *)(prev + 0x50)) /
                      (float)(*(longlong *)(next + 0x50) - *(longlong *)(prev + 0x50));

            Quaternion qa; qa.Set(*(float *)(prev + 0x18), *(float *)(prev + 0x1c), *(float *)(prev + 0x20));
            Quaternion qb; qb.Set(*(float *)(next + 0x18), *(float *)(next + 0x1c), *(float *)(next + 0x20));
            this->rotation.Lerp(qa, qb, t);

            this->translation = AEMath::VectorLerp(
                *(AEMath::Vector *)(prev + 0x00), *(AEMath::Vector *)(next + 0x00), t);
            this->scale = AEMath::VectorLerp(
                *(AEMath::Vector *)(prev + 0x0c), *(AEMath::Vector *)(next + 0x0c), t);
            this->alpha = *(float *)(prev + 0x48) +
                          t * (*(float *)(next + 0x48) - *(float *)(prev + 0x48));

            Quaternion qc; qc.Set(*(float *)(prev + 0x3c), *(float *)(prev + 0x40), *(float *)(prev + 0x44));
            Quaternion qd; qd.Set(*(float *)(next + 0x3c), *(float *)(next + 0x40), *(float *)(next + 0x44));
            this->localRotation.Lerp(qc, qd, t);

            this->localTranslation = AEMath::VectorLerp(
                *(AEMath::Vector *)(prev + 0x24), *(AEMath::Vector *)(next + 0x24), t);
            this->localScale = AEMath::VectorLerp(
                *(AEMath::Vector *)(prev + 0x30), *(AEMath::Vector *)(next + 0x30), t);
        }

        uint a = (uint)clamp_positive_byte(this->alpha);
        this->color = (int)(a | (a << 8) | (a << 16) | (a << 24));

        AEMath::Matrix rotationMat;
        make_identity(rotationMat);
        this->rotation.Convert(rotationMat);
        this->rotationMatrix = rotationMat;

        AEMath::Matrix scaleMat;
        AEMath::MatrixSetScaling(scaleMat, this->scale.x, this->scale.y, this->scale.z);
        AEMath::MatrixSetTranslation(scaleMat, this->translation.x,
                                     this->translation.y, this->translation.z);

        AEMath::Matrix local;
        this->localRotation.Convert(local);
        AEMath::Matrix composed = AEMath::operator*(local, scaleMat);
        this->localMatrix = composed;
    }

    if (!updateBounds) {
        AEMath::BSphere sphere;
        sphere.center.x = 0.0f;
        sphere.center.y = 0.0f;
        sphere.center.z = 0.0f;
        sphere.radius = 0.0f;
        sphere.radius2 = 1.0f;
        this->bounds() = sphere;

        for (uint i = 0; i < this->meshes.size(); ++i) {
            Mesh *mesh = this->meshes[i];
            AEMath::BSphere childSphere;
            if (mesh != 0 && mesh->animation != 0) {
                (mesh->animation)->InternUpdate(time, updateBounds);
            }
            childSphere = *(AEMath::BSphere *)&mesh->boundsCenterX;
            this->bounds().Merge(childSphere);
        }

        for (uint i = 0; i < this->children.size(); ++i) {
            Transform *child = this->children[i];
            child->InternUpdate(time, updateBounds);
            AEMath::BSphere childSphere = child->bounds();
            this->bounds().Merge(childSphere);
        }
    }
}

void Transform::AddKeyFrame(const AEMath::Vector &a, const AEMath::Vector &b,
                            const AEMath::Vector &c, const AEMath::Vector &d,
                            const AEMath::Vector &e, const AEMath::Vector &f,
                            int time) {
    char *keyFrame = (char *)new KeyFrame();
    *(AEMath::Vector *)(keyFrame + 0x18) = b;
    *(AEMath::Vector *)(keyFrame + 0x00) = c;
    *(AEMath::Vector *)(keyFrame + 0x0c) = a;
    *(AEMath::Vector *)(keyFrame + 0x3c) = e;
    *(AEMath::Vector *)(keyFrame + 0x24) = f;
    *(AEMath::Vector *)(keyFrame + 0x30) = d;
    longlong timestamp = time;
    *(longlong *)(keyFrame + 0x50) = timestamp;
    this->keyFrames.push_back((KeyFrame *)keyFrame);
    if (this->animationLength < timestamp) {
        this->animationLength = timestamp;
    }
}

static float kAngleDivisor = 57.295780f;

static void copy_vec(char *dst, const char *src) {
    *(AEMath::Vector *)dst = *(const AEMath::Vector *)src;
}

static inline __attribute__((always_inline)) void lerp_vec(char *dst, const char *from, const char *to, float t) {
    AEMath::Vector value = AEMath::VectorLerp(*(const AEMath::Vector *)from,
                                              *(const AEMath::Vector *)to, t);
    *(AEMath::Vector *)dst = value;
}

static inline __attribute__((always_inline)) void apply_value_new(Transform *owner, char *key, const float *values, longlong flags) {
    if (flags == 0x10) {
        *(float *)(key + 0x10) = values[0];
        if (owner->boundingRadius2 < values[0]) owner->boundingRadius2 = values[0];
    } else if (flags == 0x20) {
        *(float *)(key + 0x14) = values[0];
        if (owner->boundingRadius2 < values[0]) owner->boundingRadius2 = values[0];
    } else if (flags == 0x38) {
        *(float *)(key + 0x0c) = values[0];
        *(float *)(key + 0x10) = values[1];
        *(float *)(key + 0x14) = values[2];
        float max = values[1] < values[0] ? values[0] : values[1];
        if (values[2] > max) max = values[2];
        if (owner->boundingRadius2 < max) owner->boundingRadius2 = max;
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

    longlong timestamp = time;
    if (this->animationLength < timestamp) {
        this->animationLength = timestamp;
    }

    uint index = 0;
    while (index < this->keyFrames.size()) {
        char *existing = (char *)this->keyFrames[index];
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

    char *key = (char *)new KeyFrame();
    *(longlong *)(key + 0x50) = timestamp;
    InsertKeyFrame((KeyFrame *)key, index);

    if (index != 0) {
        Array<KeyFrame *> &items = this->keyFrames;
        char *prev = (char *)items[index - 1];
        if (index < this->keyFrames.size() - 1) {
            char *next = (char *)items[index + 1];
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

static float old_angle_divisor = 57.295780f;

static inline __attribute__((always_inline)) void apply_value_old(Transform *owner, char *key, const float *values, longlong flags) {
    if (flags == 0x10) {
        *(float *)(key + 0x10) = values[0];
        if (owner->boundingRadius2 < values[0]) owner->boundingRadius2 = values[0];
    } else if (flags == 0x20) {
        *(float *)(key + 0x14) = values[0];
        if (owner->boundingRadius2 < values[0]) owner->boundingRadius2 = values[0];
    } else if (flags == 0x38) {
        *(float *)(key + 0x0c) = values[0];
        *(float *)(key + 0x10) = values[1];
        *(float *)(key + 0x14) = values[2];
        float max = values[1] < values[0] ? values[0] : values[1];
        if (values[2] > max) max = values[2];
        if (owner->boundingRadius2 < max) owner->boundingRadius2 = max;
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

    longlong timestamp = time;
    if (this->animationLength < timestamp) {
        this->animationLength = timestamp;
    }

    uint index = 0;
    while (index < this->keyFrames.size() &&
           *(longlong *)((char *)this->keyFrames[index] + 0x50) < timestamp) {
        ++index;
    }

    char *key = (char *)new KeyFrame();
    if (index != 0) {
        char *prev = (char *)this->keyFrames[index - 1];
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

    if (this->keyFrames.size() == 0) {
        this->keyFrames.push_back((KeyFrame *)key);
    } else if (index == this->keyFrames.size()) {
        char *last = (char *)this->keyFrames[this->keyFrames.size() - 1];
        uint mask = *(uint *)(key + 0x58);
        for (int off = 0; off <= 0x48; off += 4) {
            if ((mask & (1u << (off / 4))) == 0) {
                *(int *)(key + off) = *(int *)(last + off);
            }
        }
        this->keyFrames.push_back((KeyFrame *)key);
    } else {
        char *existing = (char *)this->keyFrames[index];
        if (*(longlong *)(existing + 0x50) == timestamp) {
            *(uint *)(existing + 0x58) |= *(uint *)(key + 0x58);
            *(uint *)(existing + 0x5c) |= *(uint *)(key + 0x5c);
            apply_value_old(this, existing, values, flags);
            UpdateKeyFrames((KeyFrame *)existing, index);
            delete (KeyFrame *)key;
            return;
        }
        InsertKeyFrame((KeyFrame *)key, index);
        UpdateKeyFrames((KeyFrame *)key, index);
    }
}

int Transform::InCameraVF(AEMath::Matrix *matrix, Camera *camera) {
    if (camera == 0 || this->vfcEnabled == false ||
        (this->meshes.size() == 1 &&
         *(uint16_t *)((char *)this->meshes[0] + 2) == 0)) {
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
            this->worldMatrix, this->boundingCenter);
        center = transformed;
        rotated = AEMath::MatrixRotateVector(this->worldMatrix, radiusVector);
    } else {
        AEMath::Matrix combined = AEMath::operator*(*matrix, this->worldMatrix);
        AEMath::Vector transformed = AEMath::MatrixTransformVector(
            combined, this->boundingCenter);
        center = transformed;
        combined = AEMath::operator*(*matrix, this->worldMatrix);
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

    return CameraIsSphereinViewFrustum(center, this->boundingRadius2 * x, camera);
}

} // namespace AbyssEngine
