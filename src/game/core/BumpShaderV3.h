#ifndef GOF2_BUMPSHADERV3_H
#define GOF2_BUMPSHADERV3_H
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

    class BumpShaderV3 : public ShaderBaseStruct {
    public:
        // ---- vertex attribute locations ----
        int aPosition;
        int aTexCoord;
        int aNormal;
        int aTangent;
        int aBitangent;

        // ---- uniform locations ----
        int uModelViewProjectionMatrix;
        int uModelMatrix;
        int uLightDirModel0;
        int uLightDirModel1;
        int uEyePosModel;
        int uTexDiffuse;
        int uTexNormal;
        int uTexSpecular;
        int uGlColor;
        int uAmbientColor0;
        int uDiffuseColor0;
        int uAmbientColor1;
        int uSpecularColor0;
        int uDiffuseColor1;
        int uSpecularColor1;
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
} // namespace AbyssEngine

#endif
