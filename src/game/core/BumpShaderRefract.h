#ifndef GOF2_BUMPSHADERREFRACT_H
#define GOF2_BUMPSHADERREFRACT_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "engine/render/ShaderBaseStruct.h"

namespace AbyssEngine {
    class Mesh;
}

namespace AbyssEngine {
    class Engine;
}

using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class BumpShaderRefract : public ShaderBaseStruct {
    public:
        int aPositionLoc;
        int aTexCoordLoc;
        int aVertexColorLoc;
        int uModelViewProjectionMatrixLoc;
        int sTexture0Loc;
        int sTexture1Loc;
        int uTexelSizeLoc;
        int sTextureBaseLoc;
        int uUvMatrixLoc;
        int uUVAnimationLoc;
        int uGlColorLoc;

        static int ShaderIndex;

        BumpShaderRefract();

        void Init(::Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, ::Engine *engine) override;
    };
}

#endif
