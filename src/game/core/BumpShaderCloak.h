#ifndef GOF2_BUMPSHADERCLOAK_H
#define GOF2_BUMPSHADERCLOAK_H
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
    class BumpShaderCloak : public ShaderBaseStruct {
    public:
        int aPosition;
        int aTexCoord;
        int aNormal;
        int aTangent;
        int aBitangent;

        int uModelViewProjectionMatrix;
        int uModelMatrix;
        int uLightDirModel[2];
        int uEyePosModel;
        int sTexture[3];
        int field_0x54;
        int uGlColor;
        int uAmbientColor[2];
        int uDiffuseColor[2];
        int uSpecularColor[2];
        int uSpecularPower;
        int uRimColor;
        int field_0x7c;
        int uAnimValue;
        int uCloakRate;
        int field_0x88;
        int uTexelSize;
        int sTextureBase;
        int field_0x94;

        static int ShaderIndex;


        BumpShaderCloak();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };

#if __SIZEOF_POINTER__ == 4
    static_assert(sizeof(BumpShaderCloak) == 0x98, "BumpShaderCloak size (Engine::ShaderInit operator new)");
#endif
}

#endif
