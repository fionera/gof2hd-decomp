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
    int textures[8];         // +0x00  texture handles (-1 = unset)
    int f_20;                // +0x20
    void *f_24;              // +0x24  scratch buffer (operator new[])
    int f_28;                // +0x28
    Array<Matrix> *arr_2c;   // +0x2c  Array<Matrix>
    Array<Matrix> *arr_38;   // +0x38  Array<Matrix>
    Array<Mesh *> *meshes;   // +0x44  Array<Mesh*>
    Array<uint32_t> *arr_50; // +0x50  Array<uint32_t>
    Array<Matrix> *arr_5c;   // +0x5c  Array<Matrix>
    AEMath::Vector vec_68;   // +0x68  (3 floats)

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

// Default zero Vector value loaded by the default constructor.
extern "C" float Material_defaultVectorX;
#endif
