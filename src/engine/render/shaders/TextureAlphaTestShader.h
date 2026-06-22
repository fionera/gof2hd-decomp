#ifndef GOF2_TEXTUREALPHATESTSHADER_H
#define GOF2_TEXTUREALPHATESTSHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

namespace AbyssEngine {
    class Engine;
}

using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class Mesh;

    class TextureAlphaTestShader : public ShaderBaseStruct {
    public:
        int alphaProgram; // GL program handle (alpha-test variant, index 1)

        int aPositionLoc[2]; // a_Position
        int aTexCoordLoc[2]; // a_TexCoord
        int uMVPMatrixLoc[2]; // u_MVPMatrix
        int uColorLoc[2]; // u_Color
        int uTextureLoc[2]; // u_Texture
        int uLightPosLoc[2]; // u_LightPos
        int uAmbientLoc[2]; // u_Ambient
        int uDiffuseLoc[2]; // u_Diffuse
        int uSamplerLoc[2]; // u_Sampler
        int uFogColorLoc[2]; // u_FogColor

        // Snapshot of ShaderBaseStruct::shaderIndexIntern taken at construction.
        static int ShaderIndex;

        TextureAlphaTestShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void ConnectShaderComponents(unsigned int program, int index);

        void UseShader(bool useExtra) override;
    };
} // namespace AbyssEngine

#endif
