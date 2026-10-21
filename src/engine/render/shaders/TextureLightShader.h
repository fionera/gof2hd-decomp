#ifndef GOF2_TEXTURELIGHTSHADER_H
#define GOF2_TEXTURELIGHTSHADER_H
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
    class TextureLightShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int aPositionLoc;
        int aTexCoordLoc;
        int aNormalLoc;
        int uWorldMatrixLoc;
        int uFullModelMatrixLoc;
        int uModelMatrixLoc;
        int uLightDirection0Loc;
        int uLightDirection1Loc;
        int uGlColorLoc;
        int sTextureLoc;
        int uAmbientColor0Loc;
        int uAmbientColor1Loc;
        int uDiffuseColor0Loc;
        int uDiffuseColor1Loc;
        int uSpecularColor0Loc;
        int uSpecularColor1Loc;
        int uSpecularPowerLoc;
        int uEyePosModelLoc;
        int uUvMatrixLoc;
        int uUVAnimationLoc;

        TextureLightShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
}

#endif
