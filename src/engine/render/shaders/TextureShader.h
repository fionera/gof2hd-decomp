#ifndef GOF2_TEXTURESHADER_H
#define GOF2_TEXTURESHADER_H
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

    class TextureShader : public ShaderBaseStruct {
    public:
        uint32_t programExt; // extended GL program handle (slot 1)
        int positionAttrib[2]; // position
        int texcoordAttrib[2]; // texcoord
        int mvpUniform[2]; // mvp
        int colorUniform[2]; // color
        int textureUniform[2]; // texture
        int worldViewUniform[2]; // worldView
        int textureModeUniform[2]; // textureMode
        int lightUniform[2]; // light
        int fogNearUniform[2]; // fogNear
        int fogFarUniform[2]; // fogFar
        int activeTextureUniform[2]; // activeTexture
        int fogColorUniform[2]; // fogColor
        int alphaUniform[2]; // alpha

        TextureShader();

        void Init(Engine *engine) override;

        void UseShader(bool flag) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void ConnectShaderComponents(uint32_t program, int slot);
    };
} // namespace AbyssEngine

#endif
