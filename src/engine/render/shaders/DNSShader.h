#ifndef GOF2_DNSSHADER_H
#define GOF2_DNSSHADER_H
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
    class DNSShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int aPositionLoc;
        int aTexCoordLoc;
        int aNormalLoc;
        int aTangentLoc;
        int aBitangentLoc;
        int uMvpMatrixLoc;
        int uModelMatrixLoc;
        int uModelMatrixFullLoc;
        int uLightDirModelLoc;
        int uEyePosModelLoc;
        int uGlColorLoc;
        int uAmbientColorLoc;
        int uDiffuseColorLoc;
        int uSpecularColorLoc;
        int uSpecularPowerLoc;
        int uTexBiasDiffuseLoc;
        int uTexBiasNormalLoc;

        DNSShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
}

#endif
