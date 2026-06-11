#ifndef GOF2_MATERIAL_H
#define GOF2_MATERIAL_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
namespace AbyssEngine {

struct Mesh;

namespace AEMath {
struct Vector;
struct Matrix;
}

// AbyssEngine::Array<T> - 12-byte container header (length, data, capacity).


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
class Material {
public:
    // @portable-fields
    int f_0; // 0x0
    unsigned char _pad_4[28];
    int f_20; // 0x20
    int f_24; // 0x24
    int f_28; // 0x28
    unsigned char _pad_2c[60];
    int f_68; // 0x68
    int f_6c; // 0x6c
    int f_70; // 0x70

    Material();
    Material(Material *other);
    ~Material();
};

} // namespace AbyssEngine

using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

static inline void *&pptr(void *self, uint32_t offset)
{
    return *(void **)((char *)self + offset);
}

extern "C" {

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
