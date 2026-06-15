#ifndef GOF2_TRANSFORM_H
#define GOF2_TRANSFORM_H
#include "gof2/common.h"
#include "gof2/engine/math/AEMath.h"   // AEMath::Matrix / Vector + math free-functions (VectorLerp, MatrixTransformVector, ...)

using uint = uint32_t;
using longlong = int64_t;
using ulonglong = uint64_t;

namespace AbyssEngine {

class Camera;
class KeyFrame;
class Mesh;
class Transform;

namespace AEMath {

// Bounding sphere as laid out inside a Transform (center / radius / squared-radius).
// Distinct from the engine-wide BSphere.h variant, whose field naming does not match
// this node's in-place bounds storage.
struct BSphere {
    Vector center;
    float radius;
    float radius2;

    BSphere &operator=(const BSphere &other);
    void Merge(const BSphere &other);
};

} // namespace AEMath

enum AnimationMode {
    AnimationMode_0 = 0
};

// Frustum test used by Transform::InCameraVF (resolves to the engine's camera code).
int CameraIsSphereinViewFrustum(const AEMath::Vector &center, float radius, Camera *camera);

class Transform {
public:
    AEMath::Matrix worldMatrix;             // final world matrix; InCameraVF reads it as *(Matrix*)this
    int color;                              // packed RGBA (per-channel alpha)
    uint32_t field_0x4c;
    uint32_t field_0x50;
    uint32_t field_0x54;
    int field_0x58;                         // render mode/flag (ctor sets 2)
    AEMath::Matrix rotationMatrix;          // rotation part (Quaternion::Convert)
    AEMath::Matrix localMatrix;             // composed local transform
    AEMath::Vector boundingCenter;
    float boundingRadius;
    float field_0xe4;
    int animationStart;
    bool visible;
    bool animating;
    float animationSpeed;
    longlong animationLength;
    longlong rangeStart;
    longlong rangeEnd;
    longlong currentTime;
    int currentKeyFrameIndex;
    // Per the binary (Transform ctor/dtor construct/destruct these in place), the mesh / child /
    // keyframe lists are EMBEDDED Array<T> objects (count+data+cap inline), not pointers: the ctor
    // does Array<Mesh*>::Array(&this->meshes) and the dtor ~Array<Mesh*>(&this->meshes). meshes are
    // at +0x3c, children at +0x4c, keyFrames at +0x11c in the 32-bit layout.
    Transform* parent;                      // +0x34 scene-graph parent
    int id;                                 // +0x48
    Array<Mesh*> meshes;
    Array<Transform*> children;
    Array<KeyFrame*> keyFrames;
    Quaternion rotation;                    // current pose rotation
    AEMath::Vector translation;             // current pose translation
    AEMath::Vector scale;                   // current pose scale
    Quaternion localRotation;               // secondary (local) rotation
    AEMath::Vector localTranslation;        // secondary translation
    AEMath::Vector localScale;              // secondary scale
    float alpha;                            // animated alpha value [0..1]
    bool vfcEnabled;
    bool keyFramesShared;                   // when true, keyFrames are not owned

    // boundingCenter/boundingRadius/field_0xe4 are contiguous and laid out exactly
    // like a BSphere; this exposes them as one so Update can Merge child bounds.
    AEMath::BSphere &bounds() { return *(AEMath::BSphere *)&boundingCenter; }

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
    int  IsRunning();
    void UpdateKeyFrames(KeyFrame *keyFrame, int index);
    void SetAnimationLength(longlong value);
    void SetAnimationRangeInTime(longlong start, longlong end);
    void InsertKeyFrame(KeyFrame *keyFrame, int index);
    void InsertKeyFrame(unsigned int channel, float value);
    void InsertKeyFrame(const float *values, longlong flags, int time);
    void InsertKeyFrame_old(const float *values, longlong flags, int time);
    void Update(longlong time, bool updateBounds);
    // 32-bit-ABI wrappers around Update(): the 64-bit animation time arrives split
    // into two 32-bit register halves at the C call boundary. Update32 takes the
    // time already in r2:r3 (low half = `elapsed`); UpdatePtr assembles it from
    // explicit (lo, hi) words read out of a sibling transform's animationLength.
    void Update32(uint32_t high, longlong elapsed, uint32_t updateBounds);
    void UpdatePtr(uint32_t lo, uint32_t hi, int updateBounds);
    // Toggle the transform (and its exhaust meshes) on/off — used when a station
    // module or fixed-object exhaust is shown or hidden.
    void SetActive();
    void SetInactive();
    void setExhaustVisible(bool visible);
    void InternUpdate(longlong time, bool updateBounds);
    void SetAnimationState(AnimationMode, void *);
    void AddKeyFrame(const AEMath::Vector &a, const AEMath::Vector &b,
                     const AEMath::Vector &c, const AEMath::Vector &d,
                     const AEMath::Vector &e, const AEMath::Vector &f,
                     int time);
    int  InCameraVF(AEMath::Matrix *matrix, Camera *camera);
};

} // namespace AbyssEngine
#endif
