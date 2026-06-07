#ifndef WORK_CLASSES_TRANSFORM_SRC_CLASS_H
#define WORK_CLASSES_TRANSFORM_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;

using uint = uint32_t;
using longlong = int64_t;
using ulonglong = uint64_t;

void *operator new(unsigned int size);
inline void *operator new(unsigned int, void *ptr) { return ptr; }
void operator delete(void *ptr) noexcept;

template <class T>
struct Array {
    uint32_t size;
    T *data;
    uint32_t size2;

    Array();
    ~Array();
};

template <class T> void ArrayAdd(T value, Array<T> &array);
template <class T> void ArraySet(Array<T> *src, Array<T> *dst);
template <class T> void ArrayReleaseClasses(Array<T> *array);

namespace AbyssEngine {

struct Camera;
struct KeyFrame;
struct Mesh;
struct Transform;

namespace AEMath {

struct Vector {
    float x;
    float y;
    float z;

    Vector &operator=(const Vector &other);
};

struct Matrix {
    float m[15];

    Matrix();
    Matrix &operator=(const Matrix &other);
};

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

struct Quaternion {
    float x;
    float y;
    float z;
    float w;

    Quaternion();
    Quaternion(float x, float y, float z);
    ~Quaternion();

    void Convert(AEMath::Matrix &matrix);
    static Quaternion *Lerp(Quaternion *out, const Quaternion *from, const Quaternion *to, float t);
};

struct KeyFrame {
    KeyFrame();
};

struct Mesh {
    Mesh(Mesh *other);
};

enum AnimationMode {
    AnimationMode_0 = 0
};

struct Transform {
    Transform();
    Transform(Transform *other);
    ~Transform();

    void SetAnimationSpeed(float value);
    void SetCurrentAnimationTime(longlong value);
    void SetVisible(bool value);
    void SetAnimationState(AnimationMode mode, void *data);
    void SetAnimationLength(longlong value);
    void Update(longlong time, bool updateBounds);
    void PauseAnimationWithTimeStamp(longlong time);
    int IsRunning();
    void PauseAnimationWithKeyFrame(int index);
    void InsertKeyFrame(KeyFrame *keyFrame, int index);
    void SetVFCFlag(bool value);
    void SetAnimationRangeInKeyFrames(int first, int last);
    void InitAnimationRangeInTime();
    void AddKeyFrame(const AEMath::Vector &a, const AEMath::Vector &b,
                     const AEMath::Vector &c, const AEMath::Vector &d,
                     const AEMath::Vector &e, const AEMath::Vector &f,
                     int time);
    void CollectAnimationData();
    void SetAnimationRangeInTime(longlong start, longlong end);
    int InCameraVF(AEMath::Matrix *matrix, Camera *camera);
    void InsertKeyFrame(const float *values, longlong flags, int time);
    void UpdateKeyFrames(KeyFrame *keyFrame, int index);
    void InternUpdate(longlong time, bool updateBounds);
    void InsertKeyFrame_old(const float *values, longlong flags, int time);
};

int CameraIsSphereinViewFrustum(const AEMath::Vector &center, float radius, Camera *camera);

} // namespace AbyssEngine

extern "C" float __aeabi_l2f(longlong value);
extern "C" void __stack_chk_fail();
extern "C" int *g_transform_insert_counter;
extern "C" bool g_transform_matrix_flag;

#endif
