#ifndef GOF2_BLURSHADER_H
#define GOF2_BLURSHADER_H
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

    class BlurShader : public ShaderBaseStruct {
    public:
        int aPosition; // a_position
        int uMvpMatrix; // u_mvpMatrix
        int aTexCoord; // a_texCoord
        int sTexture; // s_texture
        int uTexelSize; // u_texelSize
        int uBlurAmount; // u_blurAmount
        int uStrength; // u_strength
        int uCenter; // u_center
        unsigned int positionAttrib; // vertex attrib slot
        unsigned int texCoordAttrib; // texcoord attrib slot
        float strength;
        float blurScale;

        BlurShader();

        ~BlurShader() override;

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void RenderEffect(FBOContainer *fbo, FBOContainer *&target, Engine *engine,
                          float amount, Vector vector);

        void RenderEffect(FBOContainer *fbo, Engine *engine, float amount, Vector vector);
    };
} // namespace AbyssEngine

#endif
