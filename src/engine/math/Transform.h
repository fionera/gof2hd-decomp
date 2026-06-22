#ifndef GOF2_TRANSFORM_H
#define GOF2_TRANSFORM_H
#include "engine/core/Array.h"
#include "../core/AEString.h"
#include "fieldaccess.h"

#include "engine/math/AEMath.h"

using uint = uint32_t;
using longlong = int64_t;
using ulonglong = uint64_t;

namespace AbyssEngine {
    namespace AEMath {
        struct BSphere {
            Vector center;
            float radius;
            float radius2;

            BSphere &operator=(const BSphere &other);

            void Merge(const BSphere &other);
        };
    }

    enum AnimationMode {
        AnimationMode_0 = 0
    };

    int CameraIsSphereinViewFrustum(const AEMath::Vector &center, float radius, Camera *camera);

    class Transform {
    public:
        AEMath::Matrix worldMatrix;
        int color;
        uint32_t field_0x4c;
        uint32_t field_0x50;
        uint32_t field_0x54;

        union {
            int renderMode;
            int field_0x58;
        };

        AEMath::Matrix rotationMatrix;
        AEMath::Matrix localMatrix;
        AEMath::Vector boundingCenter;
        float boundingRadius;
        float boundingRadius2;
        int animationStart;
        bool visible;
        bool animating;
        float animationSpeed;
        longlong animationLength;
        longlong rangeStart;
        longlong rangeEnd;
        longlong currentTime;
        int currentKeyFrameIndex;

        Transform *parent;
        int id;
        Array<Mesh *> meshes;
        Array<Transform *> children;
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

        void SetAnimationState(AnimationMode, void *);

        void AddKeyFrame(const AEMath::Vector &a, const AEMath::Vector &b,
                         const AEMath::Vector &c, const AEMath::Vector &d,
                         const AEMath::Vector &e, const AEMath::Vector &f,
                         int time);

        int InCameraVF(AEMath::Matrix *matrix, Camera *camera);
    };
}
#endif
