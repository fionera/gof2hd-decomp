#ifndef GOF2_SIMPLEREFRACTIONSHADER_H
#define GOF2_SIMPLEREFRACTIONSHADER_H
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
    class SimpleRefractionShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int aPositionLoc;
        int aTexCoordLoc;
        int aNormalLoc;
        int aTangentLoc;
        int aBitangentLoc;
        int uModelViewProjectionMatrixLoc;
        int sTexture0Loc;
        int sTexture1Loc;
        int uTexBiasLoc;
        int uGlColorLoc;
        int uEyeposmodelLoc;
        int uCloakRateLoc;
        int uTexelSizeLoc;
        int sTextureBaseLoc;

        SimpleRefractionShader();

        void Init(::Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, ::Engine *engine) override;
    };
}

#endif
