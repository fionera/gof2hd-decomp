#ifndef GOF2_BUMPRIMCUBESHADER_H
#define GOF2_BUMPRIMCUBESHADER_H
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
    class BumpRimCubeShader : public ShaderBaseStruct {
    public:
        int aPosition;
        int aTexCoord;
        int aNormal;
        int aTangent;
        int aBitangent;
        int uModelViewProjectionMatrix;
        int uModelMatrix;
        int uModelMatrixFull;
        int uLightDirModel[2];
        int uEyePosModel;
        int sTexture[2];
        int uTextureCubemap;
        int uGlColor;
        int uAmbientColor[2];
        int uDiffuseColor[2];
        int uSpecularColor[2];
        int uSpecularPower;
        int uRimColor;
        int uTexBiasDiffuse;
        int uTexBiasNormal;
        int uIsGlowMat;
        int uFogColor;
        int uFogMaxDist;
        int uFogMinDist;
        int uEnableFog;

        static int ShaderIndex;

        BumpRimCubeShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
}

#endif
