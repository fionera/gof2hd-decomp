#ifndef GOF2_TRANSFORM_H
#define GOF2_TRANSFORM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>
using uint = uint32_t;
using longlong = int64_t;
using ulonglong = uint64_t;



template <class T> void ArrayAdd(T value, Array<T> &array);
template <class T> void ArraySet(Array<T> *src, Array<T> *dst);
template <class T> void ArrayReleaseClasses(Array<T> *array);

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
extern "C" void __stack_chk_fail();
extern "C" int *g_transform_insert_counter;
extern "C" bool g_transform_matrix_flag;

namespace AbyssEngine {

struct Transform {
    uint8_t field_0x0;                  // +0x0
    uint16_t field_0x2;                 // +0x2
    uint32_t field_0x4;                 // +0x4
    uint32_t field_0x8;                 // +0x8
    uint32_t field_0xc;                 // +0xc
    uint32_t field_0x10;                // +0x10
    uint32_t field_0x14;                // +0x14
    uint32_t field_0x18;                // +0x18
    uint16_t field_0x28;                // +0x28
    uint16_t field_0x2a;                // +0x2a
    uint32_t field_0x2c;                // +0x2c
    uint32_t field_0x30;                // +0x30
    Transform* field_0x34;              // +0x34
    uint meshCount;                    // +0x3c
    char** meshes;                  // +0x40
    uint childCount;                    // +0x4c
    Transform** children;             // +0x50
    uint8_t field_0x5c;                 // +0x5c
    uint32_t field_0x60;                // +0x60
    uint32_t field_0x64;                // +0x64
    uint32_t field_0x68;                // +0x68
    uint32_t field_0x6c;                // +0x6c
    uint32_t field_0x70;                // +0x70
    uint32_t field_0x74;                // +0x74
    uint32_t field_0x78;                // +0x78
    uint32_t field_0x80;                // +0x80
    unsigned char field_0x84;           // +0x84
    uint8_t field_0x85;                 // +0x85
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
    int keyFrameCount;                    // +0x11c (keyframe array length)
    KeyFrame** keyFrames;             // +0x120 (keyframe array data)
    bool vfcEnabled;                   // +0x17c

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
    void InsertKeyFrame(const float *values, longlong flags, int time);
    void InsertKeyFrame_old(const float *values, longlong flags, int time);
    void Update(longlong time, bool updateBounds);
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
