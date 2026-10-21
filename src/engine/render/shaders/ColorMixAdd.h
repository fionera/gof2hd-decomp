#ifndef GOF2_COLORMIXADD_H
#define GOF2_COLORMIXADD_H
#include "engine/core/Array.h"
#include "../../core/AEString.h"
#include "engine/render/ShaderBaseStruct.h"

namespace AbyssEngine {
    class Mesh;
}

namespace AbyssEngine {
    class Engine;
}

using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class ColorMixAdd : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int aPositionLoc;
        int aTexCoordLoc;
        int uWorldMatrixLoc;
        int uGlColorLoc;
        int sTextureLoc;
        int uUvMatrixLoc;
        int uDarkenValueLoc;
        int uUVAnimationLoc;

        ColorMixAdd();

        void Init(Engine *engine) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void SetInActive() override;
    };
}

#endif
