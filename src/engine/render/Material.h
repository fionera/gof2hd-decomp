#ifndef GOF2_MATERIAL_H
#define GOF2_MATERIAL_H
#include <cstdint>
#include "mathtypes.h"   // AbyssEngine::AEMath::{Vector,Matrix} (used by value)

#include "engine/core/Array.h"

namespace AbyssEngine {
    class Mesh; // pointer-only

    class Material {
    public:
        using Matrix = AEMath::Matrix;
        using Vector = AEMath::Vector;

        int textures[8]; // +0x00  texture handles (-1 = unset)
        union {
            // +0x20  render/material mode (4 bytes)
            int materialMode; //   primary name (set to 0xe to cloak)
            int blendMode; //   legacy alias kept for existing TUs
        };

        void *addData; // +0x24  scratch buffer (operator new[])
        uint32_t addDataSize; // +0x28
        Array<Matrix> *arr_2c; // +0x2c
        Array<Matrix> *arr_38; // +0x38
        Array<Mesh *> *meshes; // +0x44
        Array<uint32_t> *arr_50; // +0x50
        Array<Matrix> *arr_5c; // +0x5c
        Vector ambientColor; // +0x68  (3 floats: R, G, B)

        Material();

        Material(Material *other);

        ~Material();
    };
} // namespace AbyssEngine

using ::AbyssEngine::Material;

extern "C" float Material_defaultVectorX;
#endif
