#ifndef GOF2_SPECCUBEMAPPING_H
#define GOF2_SPECCUBEMAPPING_H
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
    class SpecCubeMapping : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int aPositionLoc;
        int aNormalLoc;
        int aTexCoordLoc;
        int uWorldMatrixLoc;
        int uModelMatrixLoc;
        int uEyeposmodelLoc;

        int uLightposmodelLoc;

        int uTextureCubemapLoc;
        int uTextureBaseLoc;

        int uEnvValueLoc;

        int uAmbientColorLoc;

        int uDiffuseColorLoc;

        int uSpecularColorLoc;

        int uSpecularPowerLoc;

        int uGlColorLoc;

        SpecCubeMapping();

        void Init(Engine *) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void SetInActive() override;
    };
}
#endif
