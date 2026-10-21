#include "engine/math/Transform.h"
#include "engine/core/ArrayReleaseClasses.h"
#include "engine/core/AbyssEngine.h"
#include "engine/core/KeyFrame.h"
#include "engine/render/Mesh.h"
#include "engine/render/Engine.h"

static bool g_transform_matrix_flag = false;

namespace AbyssEngine {
    int CameraIsSphereinViewFrustum(const AEMath::Vector &center, float radius,
                                    AEMath::Matrix *matrix, Camera *camera);

    void Transform::SetCurrentAnimationTime(longlong value) {
        this->currentTime = value;
    }

    void Transform::SetAnimationRangeInKeyFrames(int first, int last) {
        longlong start;
        if (first < 0 || (int) this->keyFrames.size() <= first) {
            start = 0;
        } else {
            start = this->keyFrames[first]->timestamp;
        }

        longlong end;
        if (last >= 0 && last <= (int) this->keyFrames.size()) {
            end = this->keyFrames[last]->timestamp;
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
        if (index >= 0 && index < (int) this->keyFrames.size()) {
            longlong time = this->keyFrames[index]->timestamp;
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
            longlong keyTime = this->keyFrames[count - 1]->timestamp;
            if (*length < keyTime) {
                *length = keyTime;
            }
        }
    }

    Transform::~Transform() {
        if (this->keyFramesShared == false) {
            ArrayReleaseClasses(this->keyFrames);
        }
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
        KeyFrame *key = keyFrame;

        Array<KeyFrame *> &items = this->keyFrames;
        int i = 0;
        while (i + 1 < index) {
            KeyFrame *next = items[i + 1];
            KeyFrame *prev = items[i];
            longlong keyTime = key->timestamp;
            float a = (float) (keyTime - next->timestamp);
            float b = (float) (keyTime - prev->timestamp);
            float t = 1.0f - a / b;
            uint flags0 = key->channelFlags;
            uint flags1 = key->channelFlagsHi;
            uint next0 = next->channelFlags;
            uint next1 = next->channelFlagsHi;

            if ((flags0 & 0x40) && !(next0 & 0x40))
                next->rotation.x = lerp_float(prev->rotation.x, key->rotation.x, t);
            if ((flags0 & 0x80) && !(next0 & 0x80))
                next->rotation.y = lerp_float(prev->rotation.y, key->rotation.y, t);
            if ((flags0 & 0x100) && !(next0 & 0x100))
                next->rotation.z = lerp_float(prev->rotation.z, key->rotation.z, t);
            if ((flags0 & 0x1) && !(next0 & 0x1))
                next->translation.x = lerp_float(prev->translation.x, key->translation.x, t);
            if ((flags0 & 0x2) && !(next0 & 0x2))
                next->translation.y = lerp_float(prev->translation.y, key->translation.y, t);
            if ((flags0 & 0x4) && !(next0 & 0x4))
                next->translation.z = lerp_float(prev->translation.z, key->translation.z, t);
            if ((flags0 & 0x8) && !(next0 & 0x8))
                next->scale.x = lerp_float(prev->scale.x, key->scale.x, t);
            if ((flags0 & 0x10) && !(next0 & 0x10))
                next->scale.y = lerp_float(prev->scale.y, key->scale.y, t);
            if ((flags0 & 0x20) && !(next0 & 0x20))
                next->scale.z = lerp_float(prev->scale.z, key->scale.z, t);
            if ((flags0 & 0x200) && !(next0 & 0x200))
                next->alpha = lerp_float(prev->alpha, key->alpha, t);
            if ((flags0 & 0x10000) && !(next0 & 0x10000))
                next->localRotation.x = lerp_float(prev->localRotation.x, key->localRotation.x, t);
            if ((flags0 & 0x20000) && !(next0 & 0x20000))
                next->localRotation.y = lerp_float(prev->localRotation.y, key->localRotation.y, t);
            if ((flags0 & 0x40000) && !(next0 & 0x40000))
                next->localRotation.z = lerp_float(prev->localRotation.z, key->localRotation.z, t);
            if ((flags0 & 0x400) && !(next0 & 0x400))
                next->localTranslation.x = lerp_float(prev->localTranslation.x, key->localTranslation.x, t);
            if ((flags0 & 0x800) && !(next0 & 0x800))
                next->localTranslation.y = lerp_float(prev->localTranslation.y, key->localTranslation.y, t);
            if ((flags0 & 0x1000) && !(next0 & 0x1000))
                next->localTranslation.z = lerp_float(prev->localTranslation.z, key->localTranslation.z, t);
            if ((flags0 & 0x2000) && !(next0 & 0x2000))
                next->localScale.x = lerp_float(prev->localScale.x, key->localScale.x, t);
            if ((flags0 & 0x4000) && !(next0 & 0x4000))
                next->localScale.y = lerp_float(prev->localScale.y, key->localScale.y, t);
            if ((flags0 & 0x8000) && !(next0 & 0x8000))
                next->localScale.z = lerp_float(prev->localScale.z, key->localScale.z, t);
            next->channelFlags = next0 | flags0;
            next->channelFlagsHi = next1 | flags1;
            ++i;
        }

        uint count = (uint) this->keyFrames.size();
        while ((uint)++index < count
        )
        {
            KeyFrame *dst = items[index];
            uint flags0 = key->channelFlags;
            if ((flags0 & 0x40) && !(dst->channelFlags & 0x40)) dst->rotation.x = key->rotation.x;
            if ((flags0 & 0x80) && !(dst->channelFlags & 0x80)) dst->rotation.y = key->rotation.y;
            if ((flags0 & 0x100) && !(dst->channelFlags & 0x100)) dst->rotation.z = key->rotation.z;
            if ((flags0 & 0x1) && !(dst->channelFlags & 0x1)) dst->translation.x = key->translation.x;
            if ((flags0 & 0x2) && !(dst->channelFlags & 0x2)) dst->translation.y = key->translation.y;
            if ((flags0 & 0x4) && !(dst->channelFlags & 0x4)) dst->translation.z = key->translation.z;
            if ((flags0 & 0x8) && !(dst->channelFlags & 0x8)) dst->scale.x = key->scale.x;
            if ((flags0 & 0x10) && !(dst->channelFlags & 0x10)) dst->scale.y = key->scale.y;
            if ((flags0 & 0x20) && !(dst->channelFlags & 0x20)) dst->scale.z = key->scale.z;
            if ((flags0 & 0x200) && !(dst->channelFlags & 0x200)) dst->alpha = key->alpha;
            if ((flags0 & 0x10000) && !(dst->channelFlags & 0x10000))
                dst->localRotation.x = key->localRotation.x;
            if ((flags0 & 0x20000) && !(dst->channelFlags & 0x20000))
                dst->localRotation.y = key->localRotation.y;
            if ((flags0 & 0x40000) && !(dst->channelFlags & 0x40000))
                dst->localRotation.z = key->localRotation.z;
            if ((flags0 & 0x400) && !(dst->channelFlags & 0x400)) dst->localTranslation.x = key->localTranslation.x;
            if ((flags0 & 0x800) && !(dst->channelFlags & 0x800)) dst->localTranslation.y = key->localTranslation.y;
            if ((flags0 & 0x1000) && !(dst->channelFlags & 0x1000)) dst->localTranslation.z = key->localTranslation.z;
            if ((flags0 & 0x2000) && !(dst->channelFlags & 0x2000)) dst->localScale.x = key->localScale.x;
            if ((flags0 & 0x4000) && !(dst->channelFlags & 0x4000)) dst->localScale.y = key->localScale.y;
            if ((flags0 & 0x8000) && !(dst->channelFlags & 0x8000)) dst->localScale.z = key->localScale.z;
        }
    }

    void Transform::SetAnimationLength(longlong value) {
        this->animationLength = value;
        this->rangeEnd = value;
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
        ArrayAdd((KeyFrame *) 0, this->keyFrames);
        for (int i = (int) this->keyFrames.size() - 1; i > index; i--)
            this->keyFrames[i] = this->keyFrames[i - 1];
        this->keyFrames[index] = keyFrame;
    }

    void Transform::Update(longlong time, bool updateBounds) {
        if (this->animating && this->animationLength >= 1) {
            longlong current = this->currentTime +
                               (longlong) ((float) time * this->animationSpeed);
            this->currentTime = current;

            if (this->rangeEnd < current) {
                if (this->flags == 2) {
                    if (this->rangeEnd != 0) {
                        current = this->rangeStart + current % this->rangeEnd;
                    } else {
                        current = this->rangeStart;
                    }
                } else {
                    this->animating = false;
                    current = this->rangeEnd;
                }
                this->currentTime = current;
            }

            InternUpdate(current, updateBounds);
        }

        if (updateBounds) {
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
                    mesh->animation->Update(time, false);

                    childSphere.center.x = mesh->boundsCenterX;
                    childSphere.center.y = mesh->boundsCenterY;
                    childSphere.center.z = mesh->boundsCenterZ;
                    childSphere.radius = mesh->boundsRadius;
                    childSphere.radius2 = mesh->boundsRadiusSq;
                    childSphere.center = AEMath::MatrixTransformVector(
                        mesh->animation->rotationMatrix, childSphere.center);
                } else {
                    childSphere.center.x = mesh->boundsCenterX;
                    childSphere.center.y = mesh->boundsCenterY;
                    childSphere.center.z = mesh->boundsCenterZ;
                    childSphere.radius = mesh->boundsRadius;
                    childSphere.radius2 = mesh->boundsRadiusSq;
                }
                this->bounds().Merge(childSphere);
            }

            for (uint i = 0; i < this->children.size(); ++i) {
                Transform *child = this->children[i];
                child->Update(time, false);

                AEMath::BSphere childSphere = child->bounds();
                childSphere.center = AEMath::MatrixTransformVector(
                    child->rotationMatrix, childSphere.center);
                this->bounds().Merge(childSphere);
            }
        }
    }

    void Transform::DebugOut(int value) {}

    Transform::Transform(Transform *other) {
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
            this->flags = other->flags;

            for (uint i = 0; i < other->meshes.size(); ++i) {
                Mesh *mesh = new Mesh(other->meshes[i]);
                ArrayAdd(mesh, this->meshes);
            }
            ArraySet(other->keyFrames, this->keyFrames);
            for (uint i = 0; i < other->children.size(); ++i) {
                Transform * child = new Transform(other->children[i]);
                ArrayAdd(child, this->children);
            }
            this->bounds() = other->bounds();
            this->visible = true;
            this->vfcEnabled = true;
            this->keyFramesShared = true;
        }
    }

    void Transform::SetAnimationState(AnimationMode mode, void *) { // lint: void_ptr method parameter baked into mangled symbol
        // lint: void_ptr method parameter baked into mangled symbol
        if (mode == AnimationMode_0) {
            this->animating = false;
            this->flags = mode;
        } else if (mode == static_cast<AnimationMode>(3)) {
            this->animating = true;
            this->currentTime = this->rangeStart;
        }
    }

    Transform::Transform() {
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
        for (int i = 0; i < 15; ++i) {
            identity.m[i] = 0.0f;
        }
        identity.m[0] = 1.0f;
        identity.m[5] = 1.0f;
        identity.m[10] = 1.0f;
        identity.m[14] = 1.0f;
        this->localMatrix = identity;
        this->rotationMatrix = identity;

        this->flags = 2;
        this->localMatrix.m[5] = 1.0f;
        this->vfcEnabled = true;
        this->keyFramesShared = false;
        this->visible = true;
        this->animationStart = 0;
    }

    static float clamp_positive_byte(float value) {
        int v = (int) (value * 255.0f);
        return (float) (v < 0 ? 0 : v);
    }

    void Transform::InternUpdate(longlong time, bool updateBounds) {
        if (this->keyFrames.size() != 0) {
            longlong current = this->rangeStart;
            if (current < time)
                current = time;
            KeyFrame *last = this->keyFrames[this->keyFrames.size() - 1];
            if (last->timestamp < current)
                current = last->timestamp;

            int index = -1;
            KeyFrame **cursor = this->keyFrames.data();
            KeyFrame *next;
            do {
                next = *cursor++;
                ++index;
            } while (next->timestamp < current);
            this->currentKeyFrameIndex = index;

            if (index == 0) {
                AEMath::Matrix identity;
                this->localMatrix = identity;
                if (Engine::enableShader == 0)
                    this->localMatrix.m[5] = -1.0f;
                return;
            }

            if (current == 0) {
                KeyFrame *key = next;
                this->rotation = Quaternion(key->rotation);
                this->translation = this->keyFrames[this->currentKeyFrameIndex]->translation;
                this->scale = this->keyFrames[this->currentKeyFrameIndex]->scale;
                key = this->keyFrames[this->currentKeyFrameIndex];
                this->alpha = key->alpha;
                this->localRotation = Quaternion(key->localRotation);
                this->localTranslation = this->keyFrames[this->currentKeyFrameIndex]->localTranslation;
                this->localScale = this->keyFrames[this->currentKeyFrameIndex]->localScale;
            } else {
                KeyFrame *prev = cursor[-2];
                longlong prevTime = prev->timestamp;
                Quaternion fromRotation(prev->rotation);
                float elapsed = (float) (current - prevTime);
                float span = (float) (next->timestamp - prevTime);
                Quaternion toRotation(this->keyFrames[this->currentKeyFrameIndex]->rotation);
                float t = elapsed / span;
                this->rotation.Lerp(fromRotation, toRotation, t);

                this->translation = AEMath::VectorLerp(this->keyFrames[this->currentKeyFrameIndex - 1]->translation,
                                                       this->keyFrames[this->currentKeyFrameIndex]->translation, t);
                this->scale = AEMath::VectorLerp(this->keyFrames[this->currentKeyFrameIndex - 1]->scale,
                                                 this->keyFrames[this->currentKeyFrameIndex]->scale, t);
                KeyFrame *from = this->keyFrames[this->currentKeyFrameIndex - 1];
                KeyFrame *to = this->keyFrames[this->currentKeyFrameIndex];
                this->alpha = from->alpha + t * (to->alpha - from->alpha);

                Quaternion fromLocal(from->localRotation);
                Quaternion toLocal(this->keyFrames[this->currentKeyFrameIndex]->localRotation);
                this->localRotation.Lerp(fromLocal, toLocal, t);

                this->localTranslation = AEMath::VectorLerp(
                    this->keyFrames[this->currentKeyFrameIndex - 1]->localTranslation,
                    this->keyFrames[this->currentKeyFrameIndex]->localTranslation, t);
                this->localScale = AEMath::VectorLerp(this->keyFrames[this->currentKeyFrameIndex - 1]->localScale,
                                                      this->keyFrames[this->currentKeyFrameIndex]->localScale, t);
            }

            unsigned int a = (unsigned int) (this->alpha * 255.0f);
            this->color = (int) (a | (a << 16) | (a << 24) | (a << 8));

            // Rotation part: the quaternion matrix with the engine's Y/Z axis swap applied.
            AEMath::Matrix rot;
            this->rotation.Convert(rot);
            this->rotationMatrix.m[0] = rot.m[0];
            this->rotationMatrix.m[1] = rot.m[2];
            this->rotationMatrix.m[2] = -rot.m[1];
            this->rotationMatrix.m[3] = rot.m[3];
            this->rotationMatrix.m[4] = rot.m[8];
            this->rotationMatrix.m[5] = rot.m[10];
            this->rotationMatrix.m[6] = -rot.m[9];
            this->rotationMatrix.m[7] = rot.m[11];
            this->rotationMatrix.m[8] = -rot.m[4];
            this->rotationMatrix.m[9] = -rot.m[6];
            this->rotationMatrix.m[10] = rot.m[5];
            this->rotationMatrix.m[11] = -rot.m[7];
            AEMath::MatrixSetScaling(this->rotationMatrix, this->scale.x, this->scale.y, this->scale.z);
            AEMath::MatrixSetTranslation(this->rotationMatrix, this->translation.x, this->translation.y,
                                         this->translation.z);

            // Local (UV) rotation from the local quaternion.
            float x = this->localRotation.x;
            float y = this->localRotation.y;
            float z = this->localRotation.z;
            float w = this->localRotation.w;
            float xx = x * x, yy = y * y, zz = z * z;
            float zw = z * w, xy = x * y, yw = y * w, xz = x * z, xw = x * w, yz = y * z;
            this->localMatrix.m[0] = (float) (1.0 - 2.0 * ((double) yy + (double) zz));
            this->localMatrix.m[1] = (float) (2.0 * ((double) xy - (double) zw));
            this->localMatrix.m[2] = (float) (2.0 * ((double) xz + (double) yw));
            this->localMatrix.m[3] = 0.0f;
            this->localMatrix.m[4] = (float) (2.0 * ((double) xy + (double) zw));
            this->localMatrix.m[5] = (float) (1.0 - 2.0 * ((double) xx + (double) zz));
            this->localMatrix.m[6] = (float) (2.0 * ((double) yz - (double) xw));
            this->localMatrix.m[7] = 0.0f;
            this->localMatrix.m[8] = (float) (2.0 * ((double) xz - (double) yw));
            this->localMatrix.m[9] = (float) (2.0 * ((double) yz + (double) xw));
            this->localMatrix.m[10] = (float) (1.0 - 2.0 * ((double) xx + (double) yy));
            this->localMatrix.m[11] = 0.0f;

            AEMath::Matrix toCenter;
            toCenter.m[3] = -0.5f;
            toCenter.m[7] = -0.5f;
            AEMath::Matrix fromCenter;
            fromCenter.m[3] = 0.5f;
            fromCenter.m[7] = 0.5f;
            AEMath::Matrix offset;
            offset.m[3] = 0.5f + this->localScale.x * this->localTranslation.x;
            offset.m[7] = 0.5f + this->localScale.y * this->localTranslation.y;
            AEMath::Matrix scaling;
            AEMath::Matrix centered;
            AEMath::MatrixSetScaling(scaling, this->localScale.x, this->localScale.y, 1.0f);
            centered = fromCenter * this->localMatrix * toCenter;
            if (Engine::enableShader != 0) {
                this->localMatrix = centered * offset * scaling * toCenter;
            } else {
                AEMath::Matrix flip;
                flip.m[5] = -1.0f;
                this->localMatrix = centered * offset * scaling * flip * toCenter;
            }
        }

        if (!updateBounds) {
            AEMath::BSphere empty = {{0.0f, 0.0f, 0.0f}, 0.0f, 1.0f};
            this->bounds() = empty;

            for (uint i = 0; i < this->meshes.size(); ++i) {
                Mesh *mesh = this->meshes[i];
                if (mesh != 0 && mesh->animation != 0) {
                    mesh->animation->InternUpdate(time, false);
                    AEMath::BSphere sphere = *reinterpret_cast<AEMath::BSphere *>(&this->meshes[i]->boundsCenterX);
                    sphere.center = AEMath::MatrixTransformVector(this->meshes[i]->animation->rotationMatrix,
                                                                  sphere.center);
                    this->bounds().Merge(sphere);
                } else {
                    AEMath::BSphere sphere = *reinterpret_cast<AEMath::BSphere *>(&mesh->boundsCenterX);
                    this->bounds().Merge(sphere);
                }
            }

            for (uint i = 0; i < this->children.size(); ++i) {
                this->children[i]->InternUpdate(time, false);
                AEMath::BSphere sphere = this->children[i]->bounds();
                sphere.center = AEMath::MatrixTransformVector(this->children[i]->rotationMatrix, sphere.center);
                this->bounds().Merge(sphere);
            }
        }
    }

    void Transform::AddKeyFrame(const AEMath::Vector &a, const AEMath::Vector &b,
                                const AEMath::Vector &c, const AEMath::Vector &d,
                                const AEMath::Vector &e, const AEMath::Vector &f,
                                int time) {
        KeyFrame *keyFrame = new KeyFrame();
        keyFrame->rotation = b;
        keyFrame->translation = c;
        keyFrame->scale = a;
        keyFrame->localRotation = e;
        keyFrame->localTranslation = f;
        keyFrame->localScale = d;
        longlong timestamp = time;
        keyFrame->timestamp = timestamp;
        ArrayAdd(keyFrame, this->keyFrames);
        if (this->animationLength < timestamp) {
            this->animationLength = timestamp;
        }
    }

    static float kAngleDivisor = 57.295780f;

    static void copy_vec(AEMath::Vector &dst, const AEMath::Vector &src) {
        dst = src;
    }

    void Transform::InsertKeyFrame(const float *values, longlong flags, int time) {
        ++AbyssEngine::lauf;

        longlong timestamp = time;
        if (this->animationLength < timestamp) {
            this->animationLength = timestamp;
        }

        uint index = 0;
        while (index < this->keyFrames.size()) {
            KeyFrame *existing = this->keyFrames[index];
            longlong existingTime = existing->timestamp;
            if (existingTime >= timestamp) {
                if (existingTime == timestamp) {
                    existing->channelFlags |= (uint) flags;
                    existing->channelFlagsHi |= (uint)((ulonglong) flags >> 32);
                    if (flags == 0x10) {
                        existing->scale.y = values[0];
                        if (this->boundingRadius2 < values[0]) this->boundingRadius2 = values[0];
                    } else if (flags == 0x20) {
                        existing->scale.z = values[0];
                        if (this->boundingRadius2 < values[0]) this->boundingRadius2 = values[0];
                    } else if (flags == 0x38) {
                        existing->scale.x = values[0];
                        existing->scale.y = values[1];
                        existing->scale.z = values[2];
                        float max = values[1] < values[0] ? values[0] : values[1];
                        if (values[2] > max) max = values[2];
                        if (this->boundingRadius2 < max) this->boundingRadius2 = max;
                    } else if (flags == 0x40) {
                        existing->rotation.x = values[0];
                    } else if (flags == 0x80) {
                        existing->rotation.y = values[0];
                    } else if (flags == 0x100) {
                        existing->rotation.z = values[0];
                    } else if (flags == 0x1c0) {
                        existing->rotation.x = values[0];
                        existing->rotation.y = values[1];
                        existing->rotation.z = values[2];
                    } else if (flags == 0x200) {
                        existing->alpha = values[0] / kAngleDivisor;
                    } else if (flags == 0x400) {
                        existing->localTranslation.x = -values[0];
                    } else if (flags == 0x800) {
                        existing->localTranslation.y = values[0];
                    } else if (flags == 0x1000) {
                        existing->localTranslation.z = values[0];
                    } else if (flags == 0x1c00) {
                        existing->localTranslation.x = values[0];
                        existing->localTranslation.y = values[1];
                        existing->localTranslation.z = values[2];
                    } else if (flags == 0x2000) {
                        existing->localScale.x = values[0];
                    } else if (flags == 0x4000) {
                        existing->localScale.y = values[0];
                    } else if (flags == 0x8000) {
                        existing->localScale.z = values[0];
                    } else if (flags == 0xe000) {
                        existing->localScale.x = values[0];
                        existing->localScale.y = values[1];
                        existing->localScale.z = values[2];
                    } else if (flags == 0x10000) {
                        existing->localRotation.x = values[0];
                    } else if (flags == 0x20000) {
                        existing->localRotation.y = values[0];
                    } else if (flags == 0x40000) {
                        existing->localRotation.z = values[0];
                    } else if (flags == 0x70000) {
                        existing->localRotation.x = values[0];
                        existing->localRotation.y = values[1];
                        existing->localRotation.z = values[2];
                    }
                    UpdateKeyFrames(existing, index);
                    return;
                }
                break;
            }
            ++index;
        }

        KeyFrame *key = new KeyFrame();
        key->timestamp = timestamp;
        InsertKeyFrame(key, index);

        if (index != 0) {
            Array<KeyFrame *> &items = this->keyFrames;
            KeyFrame *prev = items[index - 1];
            if (index < this->keyFrames.size() - 1) {
                KeyFrame *next = items[index + 1];
                float t = (float) (timestamp - prev->timestamp) /
                          (float) (next->timestamp - prev->timestamp);
                key->translation = AEMath::VectorLerp(prev->translation, next->translation, t);
                key->scale = AEMath::VectorLerp(prev->scale, next->scale, t);
                key->rotation = AEMath::VectorLerp(prev->rotation, next->rotation, t);
                key->alpha = prev->alpha + t * (next->alpha - prev->alpha);
                key->localTranslation = AEMath::VectorLerp(prev->localTranslation, next->localTranslation, t);
                key->localScale = AEMath::VectorLerp(prev->localScale, next->localScale, t);
                key->localRotation = AEMath::VectorLerp(prev->localRotation, next->localRotation, t);
            } else {
                copy_vec(key->translation, prev->translation);
                copy_vec(key->scale, prev->scale);
                copy_vec(key->rotation, prev->rotation);
                key->alpha = prev->alpha;
                copy_vec(key->localTranslation, prev->localTranslation);
                copy_vec(key->localScale, prev->localScale);
                copy_vec(key->localRotation, prev->localRotation);
            }
            key->channelFlags |= prev->channelFlags;
            key->channelFlagsHi |= prev->channelFlagsHi;
        }

        key->channelFlags |= (uint) flags;
        key->channelFlagsHi |= (uint)((ulonglong) flags >> 32);
        if (flags == 0x10) {
            key->scale.y = values[0];
            if (this->boundingRadius2 < values[0]) this->boundingRadius2 = values[0];
        } else if (flags == 0x20) {
            key->scale.z = values[0];
            if (this->boundingRadius2 < values[0]) this->boundingRadius2 = values[0];
        } else if (flags == 0x38) {
            key->scale.x = values[0];
            key->scale.y = values[1];
            key->scale.z = values[2];
            float max = values[1] < values[0] ? values[0] : values[1];
            if (values[2] > max) max = values[2];
            if (this->boundingRadius2 < max) this->boundingRadius2 = max;
        } else if (flags == 0x40) {
            key->rotation.x = values[0];
        } else if (flags == 0x80) {
            key->rotation.y = values[0];
        } else if (flags == 0x100) {
            key->rotation.z = values[0];
        } else if (flags == 0x1c0) {
            key->rotation.x = values[0];
            key->rotation.y = values[1];
            key->rotation.z = values[2];
        } else if (flags == 0x200) {
            key->alpha = values[0] / kAngleDivisor;
        } else if (flags == 0x400) {
            key->localTranslation.x = -values[0];
        } else if (flags == 0x800) {
            key->localTranslation.y = values[0];
        } else if (flags == 0x1000) {
            key->localTranslation.z = values[0];
        } else if (flags == 0x1c00) {
            key->localTranslation.x = values[0];
            key->localTranslation.y = values[1];
            key->localTranslation.z = values[2];
        } else if (flags == 0x2000) {
            key->localScale.x = values[0];
        } else if (flags == 0x4000) {
            key->localScale.y = values[0];
        } else if (flags == 0x8000) {
            key->localScale.z = values[0];
        } else if (flags == 0xe000) {
            key->localScale.x = values[0];
            key->localScale.y = values[1];
            key->localScale.z = values[2];
        } else if (flags == 0x10000) {
            key->localRotation.x = values[0];
        } else if (flags == 0x20000) {
            key->localRotation.y = values[0];
        } else if (flags == 0x40000) {
            key->localRotation.z = values[0];
        } else if (flags == 0x70000) {
            key->localRotation.x = values[0];
            key->localRotation.y = values[1];
            key->localRotation.z = values[2];
        }
        UpdateKeyFrames(key, index);
    }

    static float old_angle_divisor = 57.295780f;

    void Transform::InsertKeyFrame_old(const float *values, longlong flags, int time) {
        ++AbyssEngine::lauf;

        longlong timestamp = time;
        if (this->animationLength < timestamp) {
            this->animationLength = timestamp;
        }

        uint index = 0;
        while (index < this->keyFrames.size() &&
               this->keyFrames[index]->timestamp < timestamp) {
            ++index;
        }

        KeyFrame *key = new KeyFrame();
        if (index != 0) {
            KeyFrame *prev = this->keyFrames[index - 1];
            key->translation = prev->translation;
            key->scale = prev->scale;
            key->rotation = prev->rotation;
            key->timestamp = prev->timestamp;
            key->channelFlags = prev->channelFlags;
            key->channelFlagsHi = prev->channelFlagsHi;
        }

        key->timestamp = timestamp;
        key->channelFlags |= (uint) flags;
        key->channelFlagsHi |= (uint)((ulonglong) flags >> 32);
        if (flags == 0x10) {
            key->scale.y = values[0];
            if (this->boundingRadius2 < values[0]) this->boundingRadius2 = values[0];
        } else if (flags == 0x20) {
            key->scale.z = values[0];
            if (this->boundingRadius2 < values[0]) this->boundingRadius2 = values[0];
        } else if (flags == 0x38) {
            key->scale.x = values[0];
            key->scale.y = values[1];
            key->scale.z = values[2];
            float max = values[1] < values[0] ? values[0] : values[1];
            if (values[2] > max) max = values[2];
            if (this->boundingRadius2 < max) this->boundingRadius2 = max;
        } else if (flags == 0x40) {
            key->rotation.x = values[0];
        } else if (flags == 0x80) {
            key->rotation.y = values[0];
        } else if (flags == 0x100) {
            key->rotation.z = values[0];
        } else if (flags == 0x1c0) {
            key->rotation.x = values[0];
            key->rotation.y = values[1];
            key->rotation.z = values[2];
        } else if (flags == 0x200) {
            key->alpha = values[0] / old_angle_divisor;
        } else if (flags == 0x400) {
            key->localTranslation.x = -values[0];
        } else if (flags == 0x800) {
            key->localTranslation.y = values[0];
        } else if (flags == 0x1000) {
            key->localTranslation.z = values[0];
        } else if (flags == 0x1c00) {
            key->localTranslation.x = -values[0];
            key->localTranslation.y = values[1];
            key->localTranslation.z = values[2];
        } else if (flags == 0x2000) {
            key->localScale.x = values[0];
        } else if (flags == 0x4000) {
            key->localScale.y = -values[0];
        } else if (flags == 0x8000) {
            key->localScale.z = values[0];
        } else if (flags == 0xe000) {
            key->localScale.x = values[0];
            key->localScale.y = -values[1];
            key->localScale.z = values[2];
        } else if (flags == 0x10000) {
            key->localRotation.x = values[0];
        } else if (flags == 0x20000) {
            key->localRotation.y = values[0];
        } else if (flags == 0x40000) {
            key->localRotation.z = values[0];
        } else if (flags == 0x70000) {
            key->localRotation.x = values[0];
            key->localRotation.y = values[1];
            key->localRotation.z = values[2];
        }

        if (this->keyFrames.size() == 0) {
            ArrayAdd(key, this->keyFrames);
        } else if (index == this->keyFrames.size()) {
            KeyFrame *last = this->keyFrames[this->keyFrames.size() - 1];
            uint mask = key->channelFlags;

            float *keyFields = &key->translation.x;
            const float *lastFields = &last->translation.x;
            for (int field = 0; field <= 0x48 / 4; ++field) {
                if ((mask & (1u << field)) == 0) {
                    keyFields[field] = lastFields[field];
                }
            }
            ArrayAdd(key, this->keyFrames);
        } else {
            KeyFrame *existing = this->keyFrames[index];
            if (existing->timestamp == timestamp) {
                existing->channelFlags |= key->channelFlags;
                existing->channelFlagsHi |= key->channelFlagsHi;
                if (flags == 0x10) {
                    existing->scale.y = values[0];
                    if (this->boundingRadius2 < values[0]) this->boundingRadius2 = values[0];
                } else if (flags == 0x20) {
                    existing->scale.z = values[0];
                    if (this->boundingRadius2 < values[0]) this->boundingRadius2 = values[0];
                } else if (flags == 0x38) {
                    existing->scale.x = values[0];
                    existing->scale.y = values[1];
                    existing->scale.z = values[2];
                    float max = values[1] < values[0] ? values[0] : values[1];
                    if (values[2] > max) max = values[2];
                    if (this->boundingRadius2 < max) this->boundingRadius2 = max;
                } else if (flags == 0x40) {
                    existing->rotation.x = values[0];
                } else if (flags == 0x80) {
                    existing->rotation.y = values[0];
                } else if (flags == 0x100) {
                    existing->rotation.z = values[0];
                } else if (flags == 0x1c0) {
                    existing->rotation.x = values[0];
                    existing->rotation.y = values[1];
                    existing->rotation.z = values[2];
                } else if (flags == 0x200) {
                    existing->alpha = values[0] / old_angle_divisor;
                } else if (flags == 0x400) {
                    existing->localTranslation.x = -values[0];
                } else if (flags == 0x800) {
                    existing->localTranslation.y = values[0];
                } else if (flags == 0x1000) {
                    existing->localTranslation.z = values[0];
                } else if (flags == 0x1c00) {
                    existing->localTranslation.x = -values[0];
                    existing->localTranslation.y = values[1];
                    existing->localTranslation.z = values[2];
                } else if (flags == 0x2000) {
                    existing->localScale.x = values[0];
                } else if (flags == 0x4000) {
                    existing->localScale.y = -values[0];
                } else if (flags == 0x8000) {
                    existing->localScale.z = values[0];
                } else if (flags == 0xe000) {
                    existing->localScale.x = values[0];
                    existing->localScale.y = -values[1];
                    existing->localScale.z = values[2];
                } else if (flags == 0x10000) {
                    existing->localRotation.x = values[0];
                } else if (flags == 0x20000) {
                    existing->localRotation.y = values[0];
                } else if (flags == 0x40000) {
                    existing->localRotation.z = values[0];
                } else if (flags == 0x70000) {
                    existing->localRotation.x = values[0];
                    existing->localRotation.y = values[1];
                    existing->localRotation.z = values[2];
                }
                UpdateKeyFrames(existing, index);
                delete key;
                return;
            }
            InsertKeyFrame(key, index);
            UpdateKeyFrames(key, index);
        }
    }

    int Transform::InCameraVF(AEMath::Matrix *matrix, Camera *camera) {
        if (camera == 0 || this->vfcEnabled == false ||
            (this->meshes.size() == 1 &&
             this->meshes[0]->vertexCount == 0)) {
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

        return CameraIsSphereinViewFrustum(center, this->boundingRadius2 * x, 0, camera);
    }
}
