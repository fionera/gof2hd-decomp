#ifndef GOF2_BUMPSHADERPARTICLE_H
#define GOF2_BUMPSHADERPARTICLE_H
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
    class BumpShaderParticle : public ShaderBaseStruct {
    public:
        int aPositionLoc;
        int aTexCoordLoc;
        int aNormalLoc;
        int aTangentLoc;
        int aBitangentLoc;
        int aVertexColorLoc;
        int uModelViewProjectionMatrixLoc;
        int uModelMatrixLoc;
        int uLightdirmodelLoc;
        int uEyeposmodelLoc;
        int sTexture0Loc;
        int sTexture1Loc;
        int uGlColorLoc;
        int uAmbientColorLoc;
        int uDiffuseColorLoc;
        int uSpecularColorLoc;
        int uSpecularPowerLoc;
        int uTexBiasDiffuseLoc;
        int uTexBiasNormalLoc;

        static int ShaderIndex;

        BumpShaderParticle();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
}

#endif
