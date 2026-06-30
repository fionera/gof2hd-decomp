#ifndef GOF2_DRAWFBOSHADER_H
#define GOF2_DRAWFBOSHADER_H
#include "engine/core/Array.h"
#include "../../core/AEString.h"
#include "engine/render/ShaderBaseStruct.h"

namespace AbyssEngine {
    class FBOContainer;
    class Mesh;
}

namespace AbyssEngine {
    class Engine;
}

using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class DrawFBOShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        // ASM (Init/UpdateMeshData, r4=this): locs read @0x48+ but ours @0x20 (+40). 40 bytes of FBO state
        // precede positionLoc (modeled as opaque pad; exact fields unknown).
        uint8_t _pad_fbo_state[40];
        int positionLoc;
        int worldViewMatrixLoc;
        int texCoordLoc;
        int textureLoc;

        DrawFBOShader();

        ~DrawFBOShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void RenderEffect(FBOContainer *fbo, Engine *engine) override;
    };
}

#endif
