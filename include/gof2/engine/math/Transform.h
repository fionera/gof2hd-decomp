#ifndef GOF2_TRANSFORM_H
#define GOF2_TRANSFORM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>
using uint = uint32_t;
using longlong = int64_t;
using ulonglong = uint64_t;

namespace AbyssEngine {

struct Camera;
struct KeyFrame;
struct Mesh;
struct Transform;

namespace AEMath {

struct BSphere {
    Vector center;
    float radius;
    float radius2;

    BSphere &operator=(const BSphere &other);
    void Merge(const BSphere &other);
};

Vector VectorLerp(const Vector &from, const Vector &to, float t);
Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);
Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);
Matrix MatrixSetScaling(Matrix &matrix, float x, float y, float z);
Matrix MatrixSetTranslation(Matrix &matrix, float x, float y, float z);
Matrix operator*(const Matrix &lhs, const Matrix &rhs);

} // namespace AEMath

struct KeyFrame {
    KeyFrame();
};

struct Mesh {
    Mesh(Mesh *other);
};

enum AnimationMode {
    AnimationMode_0 = 0
};

int CameraIsSphereinViewFrustum(const AEMath::Vector &center, float radius, Camera *camera);

} // namespace AbyssEngine

extern "C" float __aeabi_l2f(longlong value);
extern "C" int *g_transform_insert_counter;
extern "C" bool g_transform_matrix_flag;

namespace AbyssEngine {

class Transform {
public:
    // Final world matrix of the node; InCameraVF reads it as *(Matrix*)this.
    AEMath::Matrix worldMatrix;             // +0x00
    int color;                              // +0x48  packed RGBA alpha (per-channel)
    uint32_t field_0x4c;                    // +0x4c
    uint32_t field_0x50;                    // +0x50
    uint32_t field_0x54;                    // +0x54
    int field_0x58;                         // +0x58  render mode/flag (ctor sets 2)
    AEMath::Matrix rotationMatrix;          // +0x5c  rotation part (Quaternion::Convert)
    AEMath::Matrix localMatrix;             // +0x98  composed local transform
    AEMath::Vector boundingCenter;          // +0xd4
    float boundingRadius;                   // +0xe0
    float field_0xe4;                   // +0xe4
    int animationStart;                     // +0xe8
    bool visible;                    // +0xec
    bool animating;                    // +0xed
    float animationSpeed;                   // +0xf0
    longlong animationLength;                // +0xf8
    longlong rangeStart;               // +0x100
    longlong rangeEnd;               // +0x108
    longlong currentTime;               // +0x110
    int currentKeyFrameIndex;               // +0x118
    Array<Mesh*>* meshes;              // +0x3c (size() is the former meshCount)
    Array<Transform*>* children;       // +0x4c (size() is the former childCount)
    Array<KeyFrame*>* keyFrames;       // +0x11c (size() is the former keyFrameCount)
    Quaternion rotation;                    // +0x128 current pose rotation
    AEMath::Vector translation;             // +0x138 current pose translation
    AEMath::Vector scale;                   // +0x144 current pose scale
    Quaternion localRotation;               // +0x150 secondary (local) rotation
    AEMath::Vector localTranslation;        // +0x160 secondary translation
    AEMath::Vector localScale;              // +0x16c secondary scale
    float alpha;                            // +0x178 animated alpha value [0..1]
    bool vfcEnabled;                   // +0x17c
    bool keyFramesShared;                   // +0x17d when true, keyFrames are not owned

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
