#ifndef GOF2_CUBENORMALMAPPING_H
#define GOF2_CUBENORMALMAPPING_H
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
    class CubeNormalMapping : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int aPositionLoc;
        int aNormalLoc;
        int aTexCoordLoc;
        int aTangentLoc;
        int aBitangentLoc;
        int uWorldMatrixLoc;
        int uModelMatrixLoc;
        int uEyeposmodelLoc;
        int uLightposmodelLoc;
        int uTextureCubemapLoc;
        int uTextureBaseLoc;
        int uNormalTextureLoc;
        int uEnvValueLoc;
        int uAmbientColorLoc;
        int uDiffuseColorLoc;
        int uGlColorLoc;
        int uSpecularColorLoc;
        int uSpecularPowerLoc;

        CubeNormalMapping();

        void Init(Engine *) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void SetInActive() override;
    };
}
#endif
