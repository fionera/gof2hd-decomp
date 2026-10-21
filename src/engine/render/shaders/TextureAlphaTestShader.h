#ifndef GOF2_TEXTUREALPHATESTSHADER_H
#define GOF2_TEXTUREALPHATESTSHADER_H
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
    class TextureAlphaTestShader : public ShaderBaseStruct {
    public:
        int fogProgram;

        int aPositionLoc[2];
        int aTexCoordLoc[2];
        int uWorldMatrixLoc[2];
        int uGlColorLoc[2];
        int sTextureLoc[2];
        int uFogColorLoc[2];
        int uFogMaxDistLoc[2];
        int uFogMinDistLoc[2];
        int uEnableFogLoc[2];
        int uEyePosModelLoc[2];

        static int ShaderIndex;

        TextureAlphaTestShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void ConnectShaderComponents(unsigned int program, int index);

        void UseShader(bool useExtra) override;
    };
}

#endif
