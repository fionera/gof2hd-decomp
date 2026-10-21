#ifndef GOF2_BUMPRIMCUBESHADER_NEW_H
#define GOF2_BUMPRIMCUBESHADER_NEW_H
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
    class BumpRimCubeShader_new : public ShaderBaseStruct {
    public:
        int aPositionLoc;
        int aTexCoordLoc;
        int aNormalLoc;
        int aTangentLoc;
        int aBitangentLoc;
        int uModelViewProjectionMatrixLoc;
        int uModelMatrixLoc;
        int uModelMatrixFullLoc;
        int uLightdirmodel0Loc;
        int uLightdirmodel1Loc;
        int uEyeposmodelLoc;
        int sTexture0Loc;
        int sTexture1Loc;
        int uTextureCubemapLoc;
        int uGlColorLoc;
        int uAmbientColorLoc;
        int uDiffuseColor0Loc;
        int uDiffuseColor1Loc;
        int uSpecularColor0Loc;
        int uSpecularColor1Loc;
        int uSpecularPowerLoc;
        int uRimColorLoc;
        int uTexBiasDiffuseLoc;
        int uTexBiasNormalLoc;
        int uIsGlowMatLoc;
        int uFogColorLoc;
        int uFogMaxDistLoc;
        int uFogMinDistLoc;
        int uEnableFogLoc;
        int uLodDistLoc;

        static int ShaderIndex;

        BumpRimCubeShader_new();

        void Init(Engine *);

        void UpdateMeshData(Mesh *mesh, Engine *engine);

        void SetInActive();
    };
}
#endif
