#ifndef GOF2_MASKSHADER_H
#define GOF2_MASKSHADER_H
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
    class MaskShader : public ShaderBaseStruct {
    public:
        int aPositionLoc;
        int aTexCoord0Loc;
        int aTexCoord1Loc;
        int uWorldMatrixLoc;
        int sTexture0Loc;
        int sTexture1Loc;
        int uGlColorLoc;

        static int ShaderIndex;

        MaskShader();

        void Init(::Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, ::Engine *engine) override;
    };
}

#endif
