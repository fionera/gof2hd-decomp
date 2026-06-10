#ifndef WORK_CLASSES_PAINTCANVAS_SRC_CLASS_H
#define WORK_CLASSES_PAINTCANVAS_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned int size_t;

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }

extern "C" void *memcpy(void *, const void *, size_t);
extern "C" void *memset(void *, int, size_t);
extern "C" int strcmp(const char *, const char *);
extern "C" int strlen(const char *);

namespace AbyssEngine {

struct Engine;
struct PaintCanvas {
    PaintCanvas(Engine *engine);
    ~PaintCanvas();
};

struct Mesh;
struct Material;
struct Resource;
struct ResourceTexture;
struct ResourceMesh;
struct ImageFont;
struct Image2D;
struct Transform;
struct Camera;
struct SpriteSystem;

struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    String();
    String(const char *text, bool copy);
    String(const uint16_t *text, bool copy);
    String(const String &other, bool copy);
    ~String();

    String &operator=(const String &other);
    uint16_t *operator[](int32_t index);
    int32_t Compare(const char *text) const;
    int32_t Compare(const String &other) const;
    String SubString(uint32_t begin, uint32_t end) const;
};

template <class T>
struct Array {
    uint32_t size;
    T *data;
    uint32_t size2;

    Array();
    ~Array();
};

template <class T>
void ArrayAdd(T item, Array<T> &array);

template <class T>
void ArrayRemoveAt(uint32_t index, Array<T> &array);

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
    operator float *();
    operator const float *() const;
};

struct BSphere {
    Vector center;
    float radius;
    float radius2;
};

Matrix MatrixIdentity(Matrix &matrix);
Matrix MatrixSetTranslation(Matrix &matrix, float x, float y, float z);
Matrix MatrixSetScaling(Matrix &matrix, float x, float y, float z);
Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z);
Matrix MatrixGetInverse(const Matrix &matrix);
void MatrixMultiply(Matrix &lhs, const Matrix &rhs);
void MatrixGetGL(const Matrix &matrix, float *out);
Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);
Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);
float VectorDot(const Vector &lhs, const Vector &rhs);
Vector VectorCross(const Vector &lhs, const Vector &rhs);
float VectorLength(const Vector &value);
float Sqrtf(float value);
float Sinf(float value);
float Cosf(float value);
float ACosf(float value);
float ATanf(float value);
float Absf(float value);

} // namespace AEMath

} // namespace AbyssEngine

#endif
