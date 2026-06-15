#ifndef GOF2_MATERIAL_H
#define GOF2_MATERIAL_H
#include <cstdint>
#include "gof2/math.h"   // AbyssEngine::AEMath::{Vector,Matrix} (used by value)

// Array<T> container alias (== std::vector<T>).
#include <vector>
template <class T> using Array = std::vector<T>;

namespace AbyssEngine {

class Mesh; // pointer-only

// A render material: a set of texture handles plus per-instance transform/colour state
// (0x74 bytes in the shipped binary).
class Material {
public:
    using Matrix = AEMath::Matrix;
    using Vector = AEMath::Vector;

    int textures[8];              // +0x00  texture handles (-1 = unset)
    int blendMode;                // +0x20
    void *addData;                // +0x24  scratch buffer (operator new[])
    uint32_t addDataSize;         // +0x28
    Array<Matrix> *arr_2c;        // +0x2c
    Array<Matrix> *arr_38;        // +0x38
    Array<Mesh *> *meshes;        // +0x44
    Array<uint32_t> *arr_50;      // +0x50
    Array<Matrix> *arr_5c;        // +0x5c
    Vector ambientColor;          // +0x68  (3 floats: R, G, B)

    Material();
    Material(Material *other);
    ~Material();
};

} // namespace AbyssEngine

// Default zero Vector value loaded by the default constructor.
extern "C" float Material_defaultVectorX;
#endif
