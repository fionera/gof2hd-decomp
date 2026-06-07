#ifndef GOF2_DECOMP_ENGINE_CLASS_H
#define GOF2_DECOMP_ENGINE_CLASS_H

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;

namespace AbyssEngine {

struct Engine {
    ~Engine();
};
struct ApplicationManager;
struct FileInterfaceAndroid;
struct FBOContainer;
struct Mesh;
struct ShaderBaseStruct;

struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    String();
    String(const char *text, bool copy);
    ~String();

    String &operator=(const String &other);
    char *GetAEChar() const;
};

namespace AEMath {

struct Vector {
    float x;
    float y;
    float z;

    Vector &operator=(const Vector &other);
};

struct Matrix {
    float m[15];
};

void MatrixGetGL(const Matrix &matrix, float *out);

} // namespace AEMath

} // namespace AbyssEngine

using Engine = AbyssEngine::Engine;
using ApplicationManager = AbyssEngine::ApplicationManager;
using FBOContainer = AbyssEngine::FBOContainer;
using Mesh = AbyssEngine::Mesh;
using ShaderBaseStruct = AbyssEngine::ShaderBaseStruct;
using String = AbyssEngine::String;
using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

template <class T>
struct Array {
    uint32_t size;
    T *data;
    uint32_t size2;

    Array();
    ~Array();
};

void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;
inline void *operator new(uint32_t, void *ptr) noexcept { return ptr; }

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" void *__aeabi_memcpy(void *dest, const void *src, unsigned long n);
extern "C" unsigned int __aeabi_uidiv(unsigned int num, unsigned int den);

#endif
