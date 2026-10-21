#ifndef GOF2_BUMPSHADER_H
#define GOF2_BUMPSHADER_H
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
    class BumpShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int aPositionLoc;
        int aTexCoordLoc;
        int aNormalLoc;
        int uModelViewProjectionMatrixLoc;
        int uModelMatrixLoc;
        int uLightdirmodel0Loc;
        int uLightdirmodel1Loc;
        int uEyeposmodelLoc;
        int sTextureLoc;
        int uTextureCubemapLoc;
        int uGlColorLoc;
        int uAmbientColor0Loc;
        int uAmbientColor1Loc;
        int uDiffuseColor0Loc;
        int uDiffuseColor1Loc;
        int uSpecularColor0Loc;
        int uSpecularColor1Loc;
        int uSpecularPowerLoc;
        int uRimColorLoc;
        int uTexBiasDiffuseLoc;
        int uTexBiasNormalLoc;
        int uFogColorLoc;
        int uFogMaxDistLoc;
        int uFogMinDistLoc;
        int uEnableFogLoc;

        BumpShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
}

#endif
