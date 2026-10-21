#ifndef GOF2_BUMPSHADERV3_H
#define GOF2_BUMPSHADERV3_H
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
    class BumpShaderV3 : public ShaderBaseStruct {
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
        int uGlColor;
        int uAmbientColor[2];
        int uDiffuseColor[2];
        int uSpecularColor[2];
        int uSpecularPower;
        int uRimColor;
        int uTexBiasDiffuse;
        int uTexBiasNormal;
        int uIsGlowMat;

        BumpShaderV3();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        static int ShaderIndex;
    };
}

#endif
