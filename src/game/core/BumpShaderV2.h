#ifndef GOF2_BUMPSHADERV2_H
#define GOF2_BUMPSHADERV2_H
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
    class BumpShaderV2 : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int aPositionLoc;
        int aTexCoordLoc;
        int aNormalLoc;
        int aTangentLoc;
        int aBitangentLoc;
        int field_0x34;
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

        BumpShaderV2();

        void Init(Engine *engine) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void SetInActive() override;
    };
}

#endif
