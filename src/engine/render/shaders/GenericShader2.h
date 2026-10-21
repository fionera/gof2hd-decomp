#ifndef GOF2_GENERICSHADER2_H
#define GOF2_GENERICSHADER2_H
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
    class GenericShader2 : public ShaderBaseStruct {
    public:
        int aPositionLoc;
        int aTexCoordLoc;
        int aNormalLoc;
        int aTangentLoc;
        int aBitangentLoc;
        int uModelViewProjectionMatrixLoc;
        int uModelMatrixLoc;
        int uLightdirmodelLoc;
        int uEyeposmodelLoc;
        int sTextureLoc;
        int uGlColorLoc;
        int uAmbientColorLoc;
        int uDiffuseColorLoc;
        int uSpecularColorLoc;

        static int ShaderIndex;

        GenericShader2();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
}

#endif
