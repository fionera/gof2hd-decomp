#ifndef GOF2_TRANSFORM_H
#define GOF2_TRANSFORM_H
#include "engine/core/Array.h"
#include "engine/math/BSphere.h"
#include "../core/AEString.h"
#include "Quaternion.h"
#include "engine/core/KeyFrame.h"

#include "engine/math/AEMath.h"
#include "engine/render/Camera.h"

#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"

namespace AbyssEngine {
    class Camera;
    class KeyFrame;
}

using uint = uint32_t;
using longlong = int64_t;
using ulonglong = uint64_t;

namespace AbyssEngine {
    class Mesh;

    class Transform;

    enum AnimationMode {
        AnimationMode_0 = 0
    };

    int CameraIsSphereinViewFrustum(const AEMath::Vector &center, float radius, Camera *camera);

    class Transform {
    public:
        AEMath::Matrix worldMatrix;
        Array<Mesh *> meshes;
        int id;
        Array<Transform *> children;

        int flags;

        AEMath::Matrix rotationMatrix;
        AEMath::Matrix localMatrix;
        AEMath::Vector boundingCenter;
        float boundingRadius;
        float boundingRadius2;
        int animationStart;
        bool visible;
        bool animating;
        float animationSpeed;
        int color;
        longlong animationLength;
        longlong rangeStart;
        longlong rangeEnd;
        longlong currentTime;
        int currentKeyFrameIndex;
        Array<KeyFrame *> keyFrames;
        Quaternion rotation;
        AEMath::Vector translation;
        AEMath::Vector scale;
        Quaternion localRotation;
        AEMath::Vector localTranslation;
        AEMath::Vector localScale;
        float alpha;
        bool vfcEnabled;
        bool keyFramesShared;

        AEMath::BSphere &bounds() { return *(AEMath::BSphere *) &boundingCenter; }

        Transform();

        Transform(Transform *other);

        ~Transform();

        void SetCurrentAnimationTime(longlong value);

        void SetAnimationRangeInKeyFrames(int first, int last);

        void SetVisible(bool value);

        void SetVFCFlag(bool value);

        void PauseAnimationWithKeyFrame(int index);

        void PauseAnimationWithTimeStamp(longlong time);

        void SetAnimationSpeed(float value);

        void CollectAnimationData();

        void InitAnimationRangeInTime();

        int IsRunning();

        void UpdateKeyFrames(KeyFrame *keyFrame, int index);

        void SetAnimationLength(longlong value);

        void SetAnimationRangeInTime(longlong start, longlong end);

        void InsertKeyFrame(KeyFrame *keyFrame, int index);

        void InsertKeyFrame(unsigned int channel, float value);

        void InsertKeyFrame(const float *values, longlong flags, int time);

        void InsertKeyFrame_old(const float *values, longlong flags, int time);

        void Update(longlong time, bool updateBounds);

        void InternUpdate(longlong time, bool updateBounds);

        int DebugOut(int value);

        void SetAnimationState(AnimationMode, void *); // lint: void_ptr method parameter baked into mangled symbol

        void AddKeyFrame(const AEMath::Vector &a, const AEMath::Vector &b,
                         const AEMath::Vector &c, const AEMath::Vector &d,
                         const AEMath::Vector &e, const AEMath::Vector &f,
                         int time);

        int InCameraVF(AEMath::Matrix *matrix, Camera *camera);
    };

#ifdef GOF2_MATCH
    static_assert(__builtin_offsetof(Transform, meshes) == 0x3c, "Transform::meshes");
    static_assert(__builtin_offsetof(Transform, id) == 0x48, "Transform::id");
    static_assert(__builtin_offsetof(Transform, children) == 0x4c, "Transform::children");
    static_assert(__builtin_offsetof(Transform, flags) == 0x58, "Transform::flags");
    static_assert(__builtin_offsetof(Transform, rotationMatrix) == 0x5c, "Transform::rotationMatrix");
    static_assert(__builtin_offsetof(Transform, localMatrix) == 0x98, "Transform::localMatrix");
    static_assert(__builtin_offsetof(Transform, boundingCenter) == 0xd4, "Transform::boundingCenter");
    static_assert(__builtin_offsetof(Transform, animationLength) == 0xf8, "Transform::animationLength");
    static_assert(__builtin_offsetof(Transform, rangeStart) == 0x100, "Transform::rangeStart");
    static_assert(__builtin_offsetof(Transform, currentKeyFrameIndex) == 0x118, "Transform::currentKeyFrameIndex");
    static_assert(__builtin_offsetof(Transform, keyFrames) == 0x11c, "Transform::keyFrames");
    static_assert(__builtin_offsetof(Transform, rotation) == 0x128, "Transform::rotation");
    static_assert(__builtin_offsetof(Transform, alpha) == 0x178, "Transform::alpha");
    static_assert(sizeof(Transform) == 0x17e, "Transform size");
#endif
}
#endif
