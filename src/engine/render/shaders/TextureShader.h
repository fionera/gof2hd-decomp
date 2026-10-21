#ifndef GOF2_TEXTURESHADER_H
#define GOF2_TEXTURESHADER_H
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
    class TextureShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        uint32_t programExt;
        int aPositionLoc[2];
        int aTexCoordLoc[2];
        int uWorldMatrixLoc[2];
        int uGlColorLoc[2];
        int sTextureLoc[2];
        int uUvMatrixLoc[2];
        int uUVAnimationLoc[2];
        int uFogColorLoc[2];
        int uFogMaxDistLoc[2];
        int uFogMinDistLoc[2];
        int uEnableFogLoc[2];
        int uEyePosModelLoc[2];
        int uDarkenValueLoc[2];

        TextureShader();

        void Init(Engine *engine) override;

        void UseShader(bool flag) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void ConnectShaderComponents(uint32_t program, int slot);
    };
}

#endif
