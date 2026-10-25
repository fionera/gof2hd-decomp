#include "engine/math/Transform.h"
#include "engine/core/ArrayReleaseClasses.h"
#include "engine/core/AbyssEngine.h"
#include "engine/core/KeyFrame.h"
#include "engine/render/Mesh.h"
#include "engine/render/Engine.h"

// The original 4076-byte InsertKeyFrame_old pushes .text onto the next 32-byte
// boundary.  Keep the GOT 72 bytes later as well, so the libc++ exception
// constructors' PC-relative GOT deltas remain byte-identical.
[[gnu::visibility("hidden"), gnu::section(".pad")]] char g_transform_matrix_flag[72] = {1};

namespace AbyssEngine {
    bool CameraIsSphereinViewFrustum(const AEMath::Vector &center, float radius,
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

    void Transform::UpdateKeyFrames(KeyFrame *keyFrame, int index) {
        // Re-blend the keys before the edited one for every channel it sets...
        for (int i = 0; i + 1 < index; i++) {
            KeyFrame *next = this->keyFrames[i + 1];
            longlong keyTime = keyFrame->timestamp;
            float toNext = (float) (keyTime - next->timestamp);
            KeyFrame *prev = this->keyFrames[i];
            float toPrev = (float) (keyTime - prev->timestamp);
            float t = 1.0f - toNext / toPrev;
            ulonglong channels = keyFrame->channels;
            if ((channels & 0x40) != 0 && (next->channels & 0x40) == 0)
                next->rotation.x = prev->rotation.x + t * (keyFrame->rotation.x - prev->rotation.x);
            if ((channels & 0x80) != 0 && (next->channels & 0x80) == 0)
                next->rotation.y = prev->rotation.y + t * (keyFrame->rotation.y - prev->rotation.y);
            if ((channels & 0x100) != 0 && (next->channels & 0x100) == 0)
                next->rotation.z = prev->rotation.z + t * (keyFrame->rotation.z - prev->rotation.z);
            if ((channels & 0x1) != 0 && (next->channels & 0x1) == 0)
                next->translation.x = prev->translation.x + t * (keyFrame->translation.x - prev->translation.x);
            if ((channels & 0x2) != 0 && (next->channels & 0x2) == 0)
                next->translation.y = prev->translation.y + t * (keyFrame->translation.y - prev->translation.y);
            if ((channels & 0x4) != 0 && (next->channels & 0x4) == 0)
                next->translation.z = prev->translation.z + t * (keyFrame->translation.z - prev->translation.z);
            if ((channels & 0x8) != 0 && (next->channels & 0x8) == 0)
                next->scale.x = prev->scale.x + t * (keyFrame->scale.x - prev->scale.x);
            if ((channels & 0x10) != 0 && (next->channels & 0x10) == 0)
                next->scale.y = prev->scale.y + t * (keyFrame->scale.y - prev->scale.y);
            if ((channels & 0x20) != 0 && (next->channels & 0x20) == 0)
                next->scale.z = prev->scale.z + t * (keyFrame->scale.z - prev->scale.z);
            if ((channels & 0x200) != 0 && (next->channels & 0x200) == 0)
                next->alpha = prev->alpha + t * (keyFrame->alpha - prev->alpha);
            if ((channels & 0x10000) != 0 && (next->channels & 0x10000) == 0)
                next->localRotation.x = prev->localRotation.x + t * (keyFrame->localRotation.x - prev->localRotation.x);
            if ((channels & 0x20000) != 0 && (next->channels & 0x20000) == 0)
                next->localRotation.y = prev->localRotation.y + t * (keyFrame->localRotation.y - prev->localRotation.y);
            if ((channels & 0x40000) != 0 && (next->channels & 0x40000) == 0)
                next->localRotation.z = prev->localRotation.z + t * (keyFrame->localRotation.z - prev->localRotation.z);
            if ((channels & 0x400) != 0 && (next->channels & 0x400) == 0)
                next->localTranslation.x = prev->localTranslation.x + t * (keyFrame->localTranslation.x - prev->localTranslation.x);
            if ((channels & 0x800) != 0 && (next->channels & 0x800) == 0)
                next->localTranslation.y = prev->localTranslation.y + t * (keyFrame->localTranslation.y - prev->localTranslation.y);
            if ((channels & 0x1000) != 0 && (next->channels & 0x1000) == 0)
                next->localTranslation.z = prev->localTranslation.z + t * (keyFrame->localTranslation.z - prev->localTranslation.z);
            if ((channels & 0x2000) != 0 && (next->channels & 0x2000) == 0)
                next->localScale.x = prev->localScale.x + t * (keyFrame->localScale.x - prev->localScale.x);
            if ((channels & 0x4000) != 0 && (next->channels & 0x4000) == 0)
                next->localScale.y = prev->localScale.y + t * (keyFrame->localScale.y - prev->localScale.y);
            if ((channels & 0x8000) != 0 && (next->channels & 0x8000) == 0)
                next->localScale.z = prev->localScale.z + t * (keyFrame->localScale.z - prev->localScale.z);
            next->channels |= channels;
        }

        // ...and copy them into the later keys that do not animate them yet.
        for (uint j = index + 1; j < this->keyFrames.size(); j++) {
            ulonglong channels = keyFrame->channels;
            if ((channels & 0x40) != 0 && (this->keyFrames[j]->channels & 0x40) == 0)
                this->keyFrames[j]->rotation.x = keyFrame->rotation.x;
            if ((channels & 0x80) != 0 && (this->keyFrames[j]->channels & 0x80) == 0)
                this->keyFrames[j]->rotation.y = keyFrame->rotation.y;
            if ((channels & 0x100) != 0 && (this->keyFrames[j]->channels & 0x100) == 0)
                this->keyFrames[j]->rotation.z = keyFrame->rotation.z;
            if ((channels & 0x1) != 0 && (this->keyFrames[j]->channels & 0x1) == 0)
                this->keyFrames[j]->translation.x = keyFrame->translation.x;
            if ((channels & 0x2) != 0 && (this->keyFrames[j]->channels & 0x2) == 0)
                this->keyFrames[j]->translation.y = keyFrame->translation.y;
            if ((channels & 0x4) != 0 && (this->keyFrames[j]->channels & 0x4) == 0)
                this->keyFrames[j]->translation.z = keyFrame->translation.z;
            if ((channels & 0x8) != 0 && (this->keyFrames[j]->channels & 0x8) == 0)
                this->keyFrames[j]->scale.x = keyFrame->scale.x;
            if ((channels & 0x10) != 0 && (this->keyFrames[j]->channels & 0x10) == 0)
                this->keyFrames[j]->scale.y = keyFrame->scale.y;
            if ((channels & 0x20) != 0 && (this->keyFrames[j]->channels & 0x20) == 0)
                this->keyFrames[j]->scale.z = keyFrame->scale.z;
            if ((channels & 0x200) != 0 && (this->keyFrames[j]->channels & 0x200) == 0)
                this->keyFrames[j]->alpha = keyFrame->alpha;
            if ((channels & 0x10000) != 0 && (this->keyFrames[j]->channels & 0x10000) == 0)
                this->keyFrames[j]->localRotation.x = keyFrame->localRotation.x;
            if ((channels & 0x20000) != 0 && (this->keyFrames[j]->channels & 0x20000) == 0)
                this->keyFrames[j]->localRotation.y = keyFrame->localRotation.y;
            if ((channels & 0x40000) != 0 && (this->keyFrames[j]->channels & 0x40000) == 0)
                this->keyFrames[j]->localRotation.z = keyFrame->localRotation.z;
            if ((channels & 0x400) != 0 && (this->keyFrames[j]->channels & 0x400) == 0)
                this->keyFrames[j]->localTranslation.x = keyFrame->localTranslation.x;
            if ((channels & 0x800) != 0 && (this->keyFrames[j]->channels & 0x800) == 0)
                this->keyFrames[j]->localTranslation.y = keyFrame->localTranslation.y;
            if ((channels & 0x1000) != 0 && (this->keyFrames[j]->channels & 0x1000) == 0)
                this->keyFrames[j]->localTranslation.z = keyFrame->localTranslation.z;
            if ((channels & 0x2000) != 0 && (this->keyFrames[j]->channels & 0x2000) == 0)
                this->keyFrames[j]->localScale.x = keyFrame->localScale.x;
            if ((channels & 0x4000) != 0 && (this->keyFrames[j]->channels & 0x4000) == 0)
                this->keyFrames[j]->localScale.y = keyFrame->localScale.y;
            if ((channels & 0x8000) != 0 && (this->keyFrames[j]->channels & 0x8000) == 0)
                this->keyFrames[j]->localScale.z = keyFrame->localScale.z;
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

        this->rangeStart = start >= 0 ? (length < start ? 0 : start) : 0;

        longlong current = this->currentTime;
        this->currentTime = current < start ? start : (current > end ? end : current);

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
            this->boundingSphere = AEMath::BSphere{{0.0f, 0.0f, 0.0f}, 0.0f, 1.0f};

            for (uint i = 0; i < this->meshes.size(); ++i) {
                Mesh *mesh = this->meshes[i];
                if (mesh != 0 && mesh->animation != 0) {
                    mesh->animation->Update(time, false);
                    AEMath::BSphere sphere = this->meshes[i]->bounds;
                    sphere.center = AEMath::MatrixTransformVector(this->meshes[i]->animation->rotationMatrix,
                                                                  sphere.center);
                    this->boundingSphere.Merge(sphere);
                } else {
                    AEMath::BSphere sphere = mesh->bounds;
                    this->boundingSphere.Merge(sphere);
                }
            }

            for (uint i = 0; i < this->children.size(); ++i) {
                this->children[i]->Update(time, false);
                AEMath::BSphere sphere = this->children[i]->boundingSphere;
                sphere.center = AEMath::MatrixTransformVector(this->children[i]->rotationMatrix, sphere.center);
                this->boundingSphere.Merge(sphere);
            }
        }
    }

    void Transform::DebugOut(int value) {}

    Transform::Transform(Transform *other)
        : translation(), scale(), localTranslation(),
          localScale() {
        if (other == 0) {
            // The original "delegates" by constructing (and immediately destroying) a temporary.
            Transform();
        } else {
            this->animationStart = other->animationStart;
            this->animating = true;
            this->animationSpeed = 1.0f;
            this->animationLength = other->animationLength;
            this->rangeStart = other->rangeStart;
            this->color = -1;
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
            this->boundingSphere = other->boundingSphere;
            this->visible = true;
            this->vfcEnabled = true;
            this->keyFramesShared = true;
        }
    }

    void Transform::SetAnimationState(AnimationMode mode, void *) { // lint: void_ptr method parameter baked into mangled symbol
        // lint: void_ptr method parameter baked into mangled symbol
        if (mode == static_cast<AnimationMode>(3)) {
            this->animating = true;
            this->currentTime = this->rangeStart;
        } else {
            if (mode == AnimationMode_0)
                this->animating = false;
            this->flags = mode;
        }
    }

    Transform::Transform()
        : translation(), scale(), localTranslation(),
          localScale() {
        this->animationLength = 0;
        this->color = -1;
        this->animating = true;
        this->rangeStart = 0;
        this->rangeEnd = 0;
        this->animationSpeed = 1.0f;
        this->currentTime = 0;
        this->currentKeyFrameIndex = 0;

        this->rotation = this->localRotation = Quaternion();
        this->translation = this->localTranslation = AEMath::Vector{0.0f, 0.0f, 0.0f};
        this->scale = this->localScale = AEMath::Vector{1.0f, 1.0f, 1.0f};
        this->rotationMatrix = this->localMatrix = AEMath::Matrix();

        this->localMatrix.m[5] = -1.0f;
        this->flags = 2;
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
            longlong nextTime;
            do {
                next = *cursor++;
                this->currentKeyFrameIndex = ++index;
                nextTime = next->timestamp;
            } while (nextTime < current);

            if (index == 0) {
                AEMath::Matrix identity;
                this->localMatrix = identity;
                if (Engine::enableShader == 0)
                    this->localMatrix.m.e5 = -1.0f;
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
                float elapsed = (float) (current - prevTime);
                float span = (float) (nextTime - prevTime);
                float t = elapsed / span;
                this->rotation = Quaternion::Lerp(Quaternion(prev->rotation),
                                                  Quaternion(this->keyFrames[this->currentKeyFrameIndex]->rotation), t);

                this->translation = AEMath::VectorLerp(this->keyFrames[this->currentKeyFrameIndex - 1]->translation,
                                                       this->keyFrames[this->currentKeyFrameIndex]->translation, t);
                this->scale = AEMath::VectorLerp(this->keyFrames[this->currentKeyFrameIndex - 1]->scale,
                                                 this->keyFrames[this->currentKeyFrameIndex]->scale, t);
                KeyFrame *from = this->keyFrames[this->currentKeyFrameIndex - 1];
                KeyFrame *to = this->keyFrames[this->currentKeyFrameIndex];
                this->alpha = from->alpha + t * (to->alpha - from->alpha);

                this->localRotation = Quaternion::Lerp(Quaternion(from->localRotation),
                                                       Quaternion(this->keyFrames[this->currentKeyFrameIndex]->localRotation), t);

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
            this->localMatrix.m[3] = this->localMatrix.m[7] = this->localMatrix.m[11] = 0.0f;
            this->localMatrix.m[0] = (float) (1.0 - 2.0 * ((double) yy + (double) zz));
            this->localMatrix.m[1] = (float) (2.0 * ((double) xy - (double) zw));
            this->localMatrix.m[2] = (float) (2.0 * ((double) xz + (double) yw));
            this->localMatrix.m[4] = (float) (2.0 * ((double) xy + (double) zw));
            this->localMatrix.m[5] = (float) (1.0 - 2.0 * ((double) xx + (double) zz));
            this->localMatrix.m[6] = (float) (2.0 * ((double) yz - (double) xw));
            this->localMatrix.m[8] = (float) (2.0 * ((double) xz - (double) yw));
            this->localMatrix.m[9] = (float) (2.0 * ((double) yz + (double) xw));
            this->localMatrix.m[10] = (float) (1.0 - 2.0 * ((double) xx + (double) yy));

            AEMath::Matrix toCenter;
            AEMath::Matrix fromCenter;
            AEMath::Matrix offset;
            AEMath::Matrix scaling;
            AEMath::Matrix centered;
            toCenter.m[3] = -0.5f;
            toCenter.m[7] = -0.5f;
            offset.m[3] = 0.5f + this->localScale.x * this->localTranslation.x;
            offset.m[7] = 0.5f + this->localScale.y * this->localTranslation.y;
            fromCenter.m[3] = 0.5f;
            fromCenter.m[7] = 0.5f;
            AEMath::MatrixSetScaling(scaling, this->localScale.x, this->localScale.y, 1.0f);
            centered = fromCenter * this->localMatrix * toCenter;
            if (Engine::enableShader == 0) {
                AEMath::Matrix flip;
                flip.m[5] = -1.0f;
                this->localMatrix = centered * offset * scaling * flip * toCenter;
            } else {
                this->localMatrix = centered * offset * scaling * toCenter;
            }
        }

        if (!updateBounds) {
            this->boundingSphere = AEMath::BSphere{{0.0f, 0.0f, 0.0f}, 0.0f, 1.0f};

            for (uint i = 0; i < this->meshes.size(); ++i) {
                Mesh *mesh = this->meshes[i];
                if (mesh != 0 && mesh->animation != 0) {
                    mesh->animation->InternUpdate(time, false);
                    AEMath::BSphere sphere = this->meshes[i]->bounds;
                    sphere.center = AEMath::MatrixTransformVector(this->meshes[i]->animation->rotationMatrix,
                                                                  sphere.center);
                    this->boundingSphere.Merge(sphere);
                } else {
                    AEMath::BSphere sphere = mesh->bounds;
                    this->boundingSphere.Merge(sphere);
                }
            }

            for (uint i = 0; i < this->children.size(); ++i) {
                this->children[i]->InternUpdate(time, false);
                AEMath::BSphere sphere = this->children[i]->boundingSphere;
                sphere.center = AEMath::MatrixTransformVector(this->children[i]->rotationMatrix, sphere.center);
                this->boundingSphere.Merge(sphere);
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
        if (this->animationLength < time)
            this->animationLength = time;

        uint index = 0;
        KeyFrame *key;
        for (; index < this->keyFrames.size(); ++index) {
            key = this->keyFrames[index];
            if (key->timestamp >= time) {
                if (key->timestamp == time)
                    goto store;
                break;
            }
        }

        key = new KeyFrame();
        key->timestamp = time;
        InsertKeyFrame(key, index);
        if (index != 0) {
            if (index < this->keyFrames.size() - 1) {
                KeyFrame *before = this->keyFrames[index - 1];
                longlong beforeTime = before->timestamp;
                float elapsed = (float) (time - beforeTime);
                KeyFrame *after = this->keyFrames[index + 1];
                float t = elapsed / (float) (after->timestamp - beforeTime);
                key->translation = AEMath::VectorLerp(before->translation, after->translation, t);
                key->scale = AEMath::VectorLerp(this->keyFrames[index - 1]->scale, this->keyFrames[index + 1]->scale, t);
                key->rotation = AEMath::VectorLerp(this->keyFrames[index - 1]->rotation,
                                                   this->keyFrames[index + 1]->rotation, t);
                before = this->keyFrames[index - 1];
                after = this->keyFrames[index + 1];
                key->alpha = before->alpha + t * (after->alpha - before->alpha);
                key->localTranslation = AEMath::VectorLerp(before->localTranslation, after->localTranslation, t);
                key->localScale = AEMath::VectorLerp(this->keyFrames[index - 1]->localScale,
                                                     this->keyFrames[index + 1]->localScale, t);
                key->localRotation = AEMath::VectorLerp(this->keyFrames[index - 1]->localRotation,
                                                        this->keyFrames[index + 1]->localRotation, t);
            } else {
                key->translation = this->keyFrames[index - 1]->translation;
                key->scale = this->keyFrames[index - 1]->scale;
                key->rotation = this->keyFrames[index - 1]->rotation;
                KeyFrame *before = this->keyFrames[index - 1];
                key->alpha = before->alpha;
                key->localTranslation = before->localTranslation;
                key->localScale = this->keyFrames[index - 1]->localScale;
                key->localRotation = this->keyFrames[index - 1]->localRotation;
            }
            key->channels |= this->keyFrames[index - 1]->channels;
        }

    store:
        key->channels |= flags;
        switch (flags) {
            case 0x1:
                key->translation.x = values[0];
                break;
            case 0x2:
                key->translation.y = values[0];
                break;
            case 0x4:
                key->translation.z = values[0];
                break;
            case 0x7:
                key->translation.x = values[0];
                key->translation.y = values[1];
                key->translation.z = values[2];
                break;
            case 0x8:
                {
                    float value = values[0];
                    key->scale.x = value;
                    if (value > this->boundingSphere.radius2)
                        this->boundingSphere.radius2 = value;
                }                break;
            case 0x10:
                {
                    float value = values[0];
                    key->scale.y = value;
                    if (value > this->boundingSphere.radius2)
                        this->boundingSphere.radius2 = value;
                }                break;
            case 0x20:
                {
                    float value = values[0];
                    key->scale.z = value;
                    if (value > this->boundingSphere.radius2)
                        this->boundingSphere.radius2 = value;
                }                break;
            case 0x38:
                key->scale.x = values[0];
                key->scale.y = values[1];
                key->scale.z = values[2];
                {
                    float largest = values[0] > values[1] ? values[0] : values[1];
                    largest = largest > values[2] ? largest : values[2];
                    if (largest > this->boundingSphere.radius2)
                        this->boundingSphere.radius2 = largest;
                }
                break;
            case 0x40:
                key->rotation.x = values[0];
                break;
            case 0x80:
                key->rotation.y = values[0];
                break;
            case 0x100:
                key->rotation.z = values[0];
                break;
            case 0x1c0:
                key->rotation.x = values[0];
                key->rotation.y = values[1];
                key->rotation.z = values[2];
                break;
            case 0x200:
                key->alpha = values[0] / 100.0f;
                break;
            case 0x400:
                key->localTranslation.x = -values[0];
                break;
            case 0x800:
                key->localTranslation.y = values[0];
                break;
            case 0x1000:
                key->localTranslation.z = values[0];
                break;
            case 0x1c00:
                key->localTranslation.x = values[0];
                key->localTranslation.y = values[1];
                key->localTranslation.z = values[2];
                break;
            case 0x2000:
                key->localScale.x = values[0];
                break;
            case 0x4000:
                key->localScale.y = values[0];
                break;
            case 0x8000:
                key->localScale.z = values[0];
                break;
            case 0xe000:
                key->localScale.x = values[0];
                key->localScale.y = values[1];
                key->localScale.z = values[2];
                break;
            case 0x10000:
                key->localRotation.x = values[0];
                break;
            case 0x20000:
                key->localRotation.y = values[0];
                break;
            case 0x40000:
                key->localRotation.z = values[0];
                break;
            case 0x70000:
                key->localRotation.x = values[0];
                key->localRotation.y = values[1];
                key->localRotation.z = values[2];
                break;
            default:
                break;
        }
        UpdateKeyFrames(key, index);
    }


    static float old_angle_divisor = 57.295780f;

    void Transform::InsertKeyFrame_old(const float *values, longlong flags, int time) {
        ++AbyssEngine::lauf;
        if (this->animationLength < time)
            this->animationLength = time;

        uint count = this->keyFrames.size();
        uint index = 0;
        while (index < count && this->keyFrames[index]->timestamp < time)
            ++index;

        KeyFrame *key = new KeyFrame();
        if (index != 0) {
            key->translation = this->keyFrames[index - 1]->translation;
            key->scale = this->keyFrames[index - 1]->scale;
            key->rotation = this->keyFrames[index - 1]->rotation;
            KeyFrame *previous = this->keyFrames[index - 1];
            key->timestamp = previous->timestamp;
            key->channels = previous->channels;
        }

        // Store the incoming channel values on the new key.
        switch (flags) {
            case 0x1:
                key->translation.x = values[0];
                break;
            case 0x2:
                key->translation.y = values[0];
                break;
            case 0x4:
                key->translation.z = values[0];
                break;
            case 0x7:
                key->translation.x = values[0];
                key->translation.y = values[1];
                key->translation.z = values[2];
                break;
            case 0x8:
                {
                    float value = values[0];
                    key->scale.x = value;
                    if (value > this->boundingSphere.radius2)
                        this->boundingSphere.radius2 = value;
                }                break;
            case 0x10:
                {
                    float value = values[0];
                    key->scale.y = value;
                    if (value > this->boundingSphere.radius2)
                        this->boundingSphere.radius2 = value;
                }                break;
            case 0x20:
                {
                    float value = values[0];
                    key->scale.z = value;
                    if (value > this->boundingSphere.radius2)
                        this->boundingSphere.radius2 = value;
                }                break;
            case 0x38:
                key->scale.x = values[0];
                key->scale.y = values[1];
                key->scale.z = values[2];
                {
                    float largest = values[0] > values[1] ? values[0] : values[1];
                    largest = largest > values[2] ? largest : values[2];
                    if (largest > this->boundingSphere.radius2)
                        this->boundingSphere.radius2 = largest;
                }
                break;
            case 0x40:
                key->rotation.x = values[0];
                break;
            case 0x80:
                key->rotation.y = values[0];
                break;
            case 0x100:
                key->rotation.z = values[0];
                break;
            case 0x1c0:
                key->rotation.x = values[0];
                key->rotation.y = values[1];
                key->rotation.z = values[2];
                break;
            case 0x200:
                key->alpha = values[0] / 100.0f;
                break;
            case 0x400:
                key->localTranslation.x = -values[0];
                break;
            case 0x800:
                key->localTranslation.y = values[0];
                break;
            case 0x1000:
                key->localTranslation.z = values[0];
                break;
            case 0x1c00:
                key->localTranslation.x = -values[0];
                key->localTranslation.y = values[1];
                key->localTranslation.z = values[2];
                break;
            case 0x2000:
                key->localScale.x = values[0];
                break;
            case 0x4000:
                key->localScale.y = -values[0];
                break;
            case 0x8000:
                key->localScale.z = values[0];
                break;
            case 0xe000:
                key->localScale.x = values[0];
                key->localScale.y = -values[1];
                key->localScale.z = values[2];
                break;
            case 0x10000:
                key->localRotation.x = values[0];
                break;
            case 0x20000:
                key->localRotation.y = values[0];
                break;
            case 0x40000:
                key->localRotation.z = values[0];
                break;
            case 0x70000:
                key->localRotation.x = values[0];
                key->localRotation.y = values[1];
                key->localRotation.z = values[2];
                break;
            default:
                break;
        }

        key->timestamp = time;
        key->channels |= flags;

        if (this->keyFrames.size() == 0) {
            ArrayAdd(key, this->keyFrames);
            return;
        }

        // Back-fill the channel into the earlier keys that never had it.
        uint i = 0;
        for (; i < this->keyFrames.size(); ++i) {
            if (!(this->keyFrames[i]->timestamp < time))
                break;
            if ((this->keyFrames[i]->channels & (ulonglong) flags) != 0)
                continue;
            if (i == 0)
                continue;
            KeyFrame *before = this->keyFrames[i - 1];
            float t = (float) (this->keyFrames[i]->timestamp - before->timestamp) /
                      (float) (time - before->timestamp);
            this->keyFrames[i]->channels |= flags;
            switch (flags) {
                case 0x1:
                    this->keyFrames[i]->translation.x = before->translation.x + t * (key->translation.x - before->translation.x);
                    break;
                case 0x2:
                    this->keyFrames[i]->translation.y = before->translation.y + t * (key->translation.y - before->translation.y);
                    break;
                case 0x4:
                    this->keyFrames[i]->translation.z = before->translation.z + t * (key->translation.z - before->translation.z);
                    break;
                case 0x7:
                    this->keyFrames[i]->translation = AEMath::VectorLerp(before->translation, key->translation, t);
                    break;
                case 0x8:
                    this->keyFrames[i]->scale.x = before->scale.x + t * (key->scale.x - before->scale.x);
                    break;
                case 0x10:
                    this->keyFrames[i]->scale.y = before->scale.y + t * (key->scale.y - before->scale.y);
                    break;
                case 0x20:
                    this->keyFrames[i]->scale.z = before->scale.z + t * (key->scale.z - before->scale.z);
                    break;
                case 0x38:
                    this->keyFrames[i]->scale = AEMath::VectorLerp(before->scale, key->scale, t);
                    break;
                case 0x40:
                    this->keyFrames[i]->rotation.x = before->rotation.x + t * (key->rotation.x - before->rotation.x);
                    break;
                case 0x80:
                    this->keyFrames[i]->rotation.y = before->rotation.y + t * (key->rotation.y - before->rotation.y);
                    break;
                case 0x100:
                    this->keyFrames[i]->rotation.z = before->rotation.z + t * (key->rotation.z - before->rotation.z);
                    break;
                case 0x1c0:
                    this->keyFrames[i]->rotation = AEMath::VectorLerp(before->rotation, key->rotation, t);
                    break;
                case 0x200:
                    this->keyFrames[i]->alpha = before->alpha + t * (key->alpha - before->alpha);
                    break;
                case 0x400:
                    this->keyFrames[i]->localTranslation.x = before->localTranslation.x + t * (key->localTranslation.x - before->localTranslation.x);
                    break;
                case 0x800:
                    this->keyFrames[i]->localTranslation.y = before->localTranslation.y + t * (key->localTranslation.y - before->localTranslation.y);
                    break;
                case 0x1000:
                    this->keyFrames[i]->localTranslation.z = before->localTranslation.z + t * (key->localTranslation.z - before->localTranslation.z);
                    break;
                case 0x1c00:
                    this->keyFrames[i]->localTranslation = AEMath::VectorLerp(before->localTranslation, key->localTranslation, t);
                    break;
                case 0x2000:
                    this->keyFrames[i]->localScale.x = before->localScale.x + t * (key->localScale.x - before->localScale.x);
                    break;
                case 0x4000:
                    this->keyFrames[i]->localScale.y = before->localScale.y + t * (key->localScale.y - before->localScale.y);
                    break;
                case 0x8000:
                    this->keyFrames[i]->localScale.z = before->localScale.z + t * (key->localScale.z - before->localScale.z);
                    break;
                case 0xe000:
                    this->keyFrames[i]->localScale = AEMath::VectorLerp(before->localScale, key->localScale, t);
                    break;
                case 0x10000:
                    this->keyFrames[i]->localRotation.x = before->localRotation.x + t * (key->localRotation.x - before->localRotation.x);
                    break;
                case 0x20000:
                    this->keyFrames[i]->localRotation.y = before->localRotation.y + t * (key->localRotation.y - before->localRotation.y);
                    break;
                case 0x40000:
                    this->keyFrames[i]->localRotation.z = before->localRotation.z + t * (key->localRotation.z - before->localRotation.z);
                    break;
                case 0x70000:
                    this->keyFrames[i]->localRotation = AEMath::VectorLerp(before->localRotation, key->localRotation, t);
                    break;
                default:
                    break;
            }
        }

        if (i == this->keyFrames.size()) {
            // Appending: inherit every channel the new key does not set from the last key.
            ulonglong lo = key->channels;
            if ((lo & 0x40) == 0)
                key->rotation.x = this->keyFrames[this->keyFrames.size() - 1]->rotation.x;
            if ((lo & 0x80) == 0)
                key->rotation.y = this->keyFrames[this->keyFrames.size() - 1]->rotation.y;
            if ((lo & 0x100) == 0)
                key->rotation.z = this->keyFrames[this->keyFrames.size() - 1]->rotation.z;
            if ((lo & 0x1) == 0)
                key->translation.x = this->keyFrames[this->keyFrames.size() - 1]->translation.x;
            if ((lo & 0x2) == 0)
                key->translation.y = this->keyFrames[this->keyFrames.size() - 1]->translation.y;
            if ((lo & 0x4) == 0)
                key->translation.z = this->keyFrames[this->keyFrames.size() - 1]->translation.z;
            if ((lo & 0x8) == 0)
                key->scale.x = this->keyFrames[this->keyFrames.size() - 1]->scale.x;
            if ((lo & 0x10) == 0)
                key->scale.y = this->keyFrames[this->keyFrames.size() - 1]->scale.y;
            if ((lo & 0x20) == 0)
                key->scale.z = this->keyFrames[this->keyFrames.size() - 1]->scale.z;
            if ((lo & 0x200) == 0)
                key->alpha = this->keyFrames[this->keyFrames.size() - 1]->alpha;
            if ((lo & 0x10000) == 0)
                key->localRotation.x = this->keyFrames[this->keyFrames.size() - 1]->localRotation.x;
            if ((lo & 0x20000) == 0)
                key->localRotation.y = this->keyFrames[this->keyFrames.size() - 1]->localRotation.y;
            if ((lo & 0x40000) == 0)
                key->localRotation.z = this->keyFrames[this->keyFrames.size() - 1]->localRotation.z;
            if ((lo & 0x400) == 0)
                key->localTranslation.x = this->keyFrames[this->keyFrames.size() - 1]->localTranslation.x;
            if ((lo & 0x800) == 0)
                key->localTranslation.y = this->keyFrames[this->keyFrames.size() - 1]->localTranslation.y;
            if ((lo & 0x1000) == 0)
                key->localTranslation.z = this->keyFrames[this->keyFrames.size() - 1]->localTranslation.z;
            if ((lo & 0x2000) == 0)
                key->localScale.x = this->keyFrames[this->keyFrames.size() - 1]->localScale.x;
            if ((lo & 0x4000) == 0)
                key->localScale.y = this->keyFrames[this->keyFrames.size() - 1]->localScale.y;
            if ((lo & 0x8000) == 0)
                key->localScale.z = this->keyFrames[this->keyFrames.size() - 1]->localScale.z;
            key->channels = lo | flags;
            ArrayAdd(key, this->keyFrames);
            return;
        }

        KeyFrame *existing = this->keyFrames[i];
        if (existing->timestamp == time) {
            // Same time: fold the new key into the existing one (additive / multiplicative).
            ulonglong lo = existing->channels;
            {
                float v = key->rotation.x;
                if ((lo & 0x40) != 0)
                    v = v + existing->rotation.x;
                existing->rotation.x = v;
            }
            {
                float v = key->rotation.y;
                if ((lo & 0x80) != 0)
                    v = v + existing->rotation.y;
                existing->rotation.y = v;
            }
            {
                float v = key->rotation.z;
                if ((lo & 0x100) != 0)
                    v = v + existing->rotation.z;
                existing->rotation.z = v;
            }
            {
                float v = key->translation.x;
                if ((lo & 0x1) != 0)
                    v = v + existing->translation.x;
                existing->translation.x = v;
            }
            {
                float v = key->translation.y;
                if ((lo & 0x2) != 0)
                    v = v + existing->translation.y;
                existing->translation.y = v;
            }
            {
                float v = key->translation.z;
                if ((lo & 0x4) != 0)
                    v = v + existing->translation.z;
                existing->translation.z = v;
            }
            {
                float v = key->scale.x;
                if ((lo & 0x8) != 0)
                    v = v * existing->scale.x;
                existing->scale.x = v;
            }
            {
                float v = key->scale.y;
                if ((lo & 0x10) != 0)
                    v = v * existing->scale.y;
                existing->scale.y = v;
            }
            {
                float v = key->scale.z;
                if ((lo & 0x20) != 0)
                    v = v * existing->scale.z;
                existing->scale.z = v;
            }
            existing->alpha = key->alpha;
            {
                float v = key->localRotation.x;
                if ((lo & 0x10000) != 0)
                    v = v + existing->localRotation.x;
                existing->localRotation.x = v;
            }
            {
                float v = key->localRotation.y;
                if ((lo & 0x20000) != 0)
                    v = v + existing->localRotation.y;
                existing->localRotation.y = v;
            }
            {
                float v = key->localRotation.z;
                if ((lo & 0x40000) != 0)
                    v = v + existing->localRotation.z;
                existing->localRotation.z = v;
            }
            {
                float v = key->localTranslation.x;
                if ((lo & 0x400) != 0)
                    v = v + existing->localTranslation.x;
                existing->localTranslation.x = v;
            }
            {
                float v = key->localTranslation.y;
                if ((lo & 0x800) != 0)
                    v = v + existing->localTranslation.y;
                existing->localTranslation.y = v;
            }
            {
                float v = key->localTranslation.z;
                if ((lo & 0x1000) != 0)
                    v = v + existing->localTranslation.z;
                existing->localTranslation.z = v;
            }
            {
                float v = key->localScale.x;
                if ((lo & 0x2000) != 0)
                    v = v * existing->localScale.x;
                existing->localScale.x = v;
            }
            {
                float v = key->localScale.y;
                if ((lo & 0x4000) != 0)
                    v = v * existing->localScale.y;
                existing->localScale.y = v;
            }
            {
                float v = key->localScale.z;
                if ((lo & 0x8000) != 0)
                    v = v * existing->localScale.z;
                existing->localScale.z = v;
            }
            existing->channels = lo | flags;
            key->channels |= flags;
            UpdateKeyFrames(key, i);
            delete key;
            return;
        }

        if (i == 0) {
            InsertKeyFrame(key, 0);
            UpdateKeyFrames(key, 0);
            return;
        }

        // In between: interpolate the neighbours, then overwrite the channel being set.
        KeyFrame *before = this->keyFrames[i - 1];
        float t = (float) (key->timestamp - before->timestamp) / (float) (existing->timestamp - before->timestamp);
        key->rotation = AEMath::VectorLerp(before->rotation, existing->rotation, t);
        key->translation = AEMath::VectorLerp(this->keyFrames[i - 1]->translation, this->keyFrames[i]->translation, t);
        key->scale = AEMath::VectorLerp(this->keyFrames[i - 1]->scale, this->keyFrames[i]->scale, t);
        key->alpha = this->keyFrames[i - 1]->alpha + t * (this->keyFrames[i]->alpha - this->keyFrames[i - 1]->alpha);
        key->localRotation = AEMath::VectorLerp(this->keyFrames[i - 1]->localRotation, this->keyFrames[i]->localRotation, t);
        key->localTranslation = AEMath::VectorLerp(this->keyFrames[i - 1]->localTranslation,
                                                   this->keyFrames[i]->localTranslation, t);
        key->localScale = AEMath::VectorLerp(this->keyFrames[i - 1]->localScale, this->keyFrames[i]->localScale, t);
        key->channels |= this->keyFrames[i - 1]->channels;
        switch (flags) {
            case 0x1:
                key->translation.x = values[0];
                break;
            case 0x2:
                key->translation.y = values[0];
                break;
            case 0x4:
                key->translation.z = values[0];
                break;
            case 0x7:
                key->translation.x = values[0];
                key->translation.y = values[1];
                key->translation.z = values[2];
                break;
            case 0x8:
                key->scale.x = values[0];
                break;
            case 0x10:
                key->scale.y = values[0];
                break;
            case 0x20:
                key->scale.z = values[0];
                break;
            case 0x38:
                key->scale.x = values[0];
                key->scale.y = values[1];
                key->scale.z = values[2];
                break;
            case 0x40:
                key->rotation.x = values[0];
                break;
            case 0x80:
                key->rotation.y = values[0];
                break;
            case 0x100:
                key->rotation.z = values[0];
                break;
            case 0x1c0:
                key->rotation.x = values[0];
                key->rotation.y = values[1];
                key->rotation.z = values[2];
                break;
            case 0x200:
                key->alpha = values[0];
                break;
            case 0x400:
                key->localTranslation.x = -values[0];
                break;
            case 0x800:
                key->localTranslation.y = values[0];
                break;
            case 0x1000:
                key->localTranslation.z = values[0];
                break;
            case 0x1c00:
                key->localTranslation.x = -values[0];
                key->localTranslation.y = values[1];
                key->localTranslation.z = values[2];
                break;
            case 0x2000:
                key->localScale.x = values[0];
                break;
            case 0x4000:
                key->localScale.y = -values[0];
                break;
            case 0x8000:
                key->localScale.z = values[0];
                break;
            case 0xe000:
                key->localScale.x = values[0];
                key->localScale.y = -values[1];
                key->localScale.z = values[2];
                break;
            case 0x10000:
                key->localRotation.x = values[0];
                break;
            case 0x20000:
                key->localRotation.y = -values[0];
                break;
            case 0x40000:
                key->localRotation.z = values[0];
                break;
            case 0x70000:
                key->localRotation.x = values[0];
                key->localRotation.y = -values[1];
                key->localRotation.z = values[2];
                break;
            default:
                break;
        }
        InsertKeyFrame(key, i);
        UpdateKeyFrames(key, i);
    }


    int Transform::InCameraVF(AEMath::Matrix *matrix, Camera *camera) {
        if (camera == 0 || this->vfcEnabled == false ||
            (this->meshes.size() == 1 &&
             this->meshes[0]->vertexCount == 0)) {
            return 1;
        }

        const AEMath::Vector &bc = this->boundingSphere.center;
        AEMath::Vector center = {0.0f, 0.0f, 0.0f};
        AEMath::Vector extent = {this->boundingSphere.radius, this->boundingSphere.radius, this->boundingSphere.radius};
        if (matrix != 0) {
            center = AEMath::MatrixTransformVector(*matrix * this->worldMatrix, bc);
            extent = AEMath::MatrixRotateVector(*matrix * this->worldMatrix, extent);
        } else {
            center = AEMath::MatrixTransformVector(this->worldMatrix, bc);
            extent = AEMath::MatrixRotateVector(this->worldMatrix, extent);
        }

        float ax = extent.x > 0.0f ? extent.x : -extent.x;
        float ay = extent.y > 0.0f ? extent.y : -extent.y;
        float az = extent.z > 0.0f ? extent.z : -extent.z;
        float m = ax > ay ? ax : ay;
        m = m > az ? m : az;

        return CameraIsSphereinViewFrustum(center, this->boundingSphere.radius2 * m, 0, camera);
    }
}
