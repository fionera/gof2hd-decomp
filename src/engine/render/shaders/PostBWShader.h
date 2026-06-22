#ifndef GOF2_POSTBWSHADER_H
#define GOF2_POSTBWSHADER_H
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
    class FBOContainer;

    class PostBWShader : public ShaderBaseStruct {
    public:
        int aPosition; // attribute a_Position
        int uMvpMatrix; // uniform u_MVPMatrix
        int aTexCoord; // attribute a_TexCoord
        int sTexture; // uniform s_Texture

        PostBWShader();

        ~PostBWShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void RenderEffect(FBOContainer *fbo, Engine *engine) override;
    };
} // namespace AbyssEngine

#endif
