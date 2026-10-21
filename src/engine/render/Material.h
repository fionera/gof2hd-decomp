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

        int blendMode;

        unsigned char *addData;
        int addDataSize;
        Array<AEMath::Matrix> arr_2c;
        Array<AEMath::Matrix> arr_38;
        Array<Mesh *> meshes;
        Array<uint32_t> arr_50;
        Array<AEMath::Matrix> arr_5c;
        AEMath::Vector ambientColor;

        // The constructors/destructor are inline: the original emits them weak (C2/D2 only) in
        // PaintCanvas.cpp, next to their callers.
        Material() : blendMode(0) {
            this->ambientColor.x = -10.0f;
            this->ambientColor.y = 0;
            this->ambientColor.z = 0;

            for (int i = 0; i != 8; ++i)
                this->textures[i] = -1;
            this->textures[0] = 0;

            this->addData = 0;
            this->addDataSize = 0;
        }

        Material(Material *other) {
            this->ambientColor.x = 0;
            this->ambientColor.y = 0;
            this->ambientColor.z = 0;

            if (other != 0) {
                for (int i = 0; i != 8; ++i)
                    this->textures[i] = other->textures[i];
                this->blendMode = other->blendMode;
                this->addDataSize = other->addDataSize;
                if (this->addDataSize != 0)
                    this->addData = new unsigned char[this->addDataSize];
                else
                    this->addData = 0;
                this->ambientColor = other->ambientColor;
            } else {
                this->blendMode = 0;
                AEMath::Vector defaultColor = {-10.0f, 0, 0};
                this->ambientColor = defaultColor;
                for (int i = 0; i != 8; ++i)
                    this->textures[i] = -1;
            }
        }

        ~Material() {
        }
    };

#if __SIZEOF_POINTER__ == 4
    static_assert(sizeof(Array<AEMath::Matrix>) == 0x0c, "Array layout (Material)");
    static_assert(sizeof(Material) == 0x74, "Material size");
    static_assert(__builtin_offsetof(Material, textures) == 0x00, "Material::textures");
    static_assert(__builtin_offsetof(Material, blendMode) == 0x20, "Material::blendMode");
    static_assert(__builtin_offsetof(Material, addData) == 0x24, "Material::addData");
    static_assert(__builtin_offsetof(Material, addDataSize) == 0x28, "Material::addDataSize");
    static_assert(__builtin_offsetof(Material, arr_2c) == 0x2c, "Material::arr_2c");
    static_assert(__builtin_offsetof(Material, arr_38) == 0x38, "Material::arr_38");
    static_assert(__builtin_offsetof(Material, meshes) == 0x44, "Material::meshes");
    static_assert(__builtin_offsetof(Material, arr_50) == 0x50, "Material::arr_50");
    static_assert(__builtin_offsetof(Material, arr_5c) == 0x5c, "Material::arr_5c");
    static_assert(__builtin_offsetof(Material, ambientColor) == 0x68, "Material::ambientColor");
#endif
}

namespace AbyssEngine {
    class Material;
}

using ::AbyssEngine::Material;

#endif
