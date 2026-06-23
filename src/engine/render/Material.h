#ifndef GOF2_MATERIAL_H
#define GOF2_MATERIAL_H
#include <cstdint>

#include "engine/core/Array.h"
#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"

#include "engine/math/AEMath.h"


namespace AbyssEngine {
    class Mesh;

    class Material {
    public:
        int textures[8];

        union {
            int materialMode;
            int blendMode;
        };

        void *addData;
        uint32_t addDataSize;
        Array<AEMath::Matrix> *arr_2c;
        Array<AEMath::Matrix> *arr_38;
        Array<Mesh *> *meshes;
        Array<uint32_t> *arr_50;
        Array<AEMath::Matrix> *arr_5c;
        AEMath::Vector ambientColor;

        Material();

        Material(Material *other);

        ~Material();
    };
}

namespace AbyssEngine {
    class Material;
}
using ::AbyssEngine::Material;

extern "C" float Material_defaultVectorX;
#endif
