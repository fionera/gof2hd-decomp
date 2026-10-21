#ifndef GOF2_BLURSHADER_H
#define GOF2_BLURSHADER_H
#include "engine/core/Array.h"
#include "../../core/AEString.h"
#include "engine/render/ShaderBaseStruct.h"

#include "engine/math/Vector.h"

namespace AbyssEngine {
    class FBOContainer;
    class Mesh;
}

namespace AbyssEngine {
    class Engine;
}

using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class BlurShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int field_0x20;
        unsigned int blurProgram;
        int aPosition;
        int uMvpMatrix;
        int aTexCoord;
        int sTexture;
        int uTexelSize;
        int uBlurAmount;
        int uStrength;
        int uCenter;
        unsigned int positionAttrib;
        int field_0x4c;
        unsigned int texCoordAttrib;
        int field_0x54;
        float strength;
        float blurScale;
        int field_0x60;

        BlurShader();

        ~BlurShader() override;

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void RenderEffect(FBOContainer *fbo, FBOContainer *&target, Engine *engine,
                          float amount, Vector vector);

        void RenderEffect(FBOContainer *fbo, Engine *engine, float amount, Vector vector);
    };
}

#endif
