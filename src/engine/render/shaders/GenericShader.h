#ifndef GOF2_GENERICSHADER_H
#define GOF2_GENERICSHADER_H
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
    class GenericShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int aPositionLoc;
        int aTexCoordLoc;
        int aNormalLoc;
        int aTangentLoc;
        int aBitangentLoc;

        int uModelViewProjectionMatrixLoc;
        int uModelMatrixLoc;
        int uLightdirmodelLoc;
        int uEyeposmodelLoc;
        int sTexture0Loc;
        int sTexture1Loc;
        int sTexture2Loc;
        int uGlColorLoc;
        int uAmbientColorLoc;
        int uDiffuseColorLoc;
        int uSpecularColorLoc;
        int uSpecularPowerLoc;

        GenericShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
}

#endif
