#ifndef WORK_CLASSES_Material_SRC_CLASS_H
#define WORK_CLASSES_Material_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace AbyssEngine {

struct Mesh;

namespace AEMath {
struct Vector;
struct Matrix;
}

// AbyssEngine::Array<T> - 12-byte container header (length, data, capacity).
template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t cap;
};

// Material layout (0x74 bytes):
//   +0x00  uint32_t textures[8]   (filled with 0xffffffff by default ctor)
//   +0x20  int      field_20
//   +0x24  void*    field_24
//   +0x28  uint32_t field_28
//   +0x2c  Array<Matrix>
//   +0x38  Array<Matrix>
//   +0x44  Array<Mesh*>
//   +0x50  Array<uint32_t>
//   +0x5c  Array<Matrix>
//   +0x68  Vector (3 floats)
struct Material {
    Material();
    Material(Material *other);
    ~Material();
};

} // namespace AbyssEngine

using Material = AbyssEngine::Material;
using Mesh = AbyssEngine::Mesh;
using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

static inline int &i32(void *self, uint32_t offset)
{
    return *(int *)((char *)self + offset);
}
static inline uint32_t &u32(void *self, uint32_t offset)
{
    return *(uint32_t *)((char *)self + offset);
}
static inline float &f32(void *self, uint32_t offset)
{
    return *(float *)((char *)self + offset);
}
static inline void *&pptr(void *self, uint32_t offset)
{
    return *(void **)((char *)self + offset);
}

extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

// Array<T> constructors/destructors (zero-init the 12-byte header).
void Array_Matrix_ctor(void *self);
void Array_Matrix_dtor(void *self);
void Array_MeshPtr_ctor(void *self);
void Array_MeshPtr_dtor(void *self);
void Array_uint_ctor(void *self);
void Array_uint_dtor(void *self);

void *operator_new_array(uint32_t size);

// AEMath::Vector::operator=(const Vector&)
void Vector_assign(Vector *self, const Vector *other);

// Default zero Vector value loaded by the default constructor.
extern float Material_defaultVectorX;
}

#endif
