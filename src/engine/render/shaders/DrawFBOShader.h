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
        // precede aPositionLoc (modeled as opaque pad; exact fields unknown).
        uint8_t _pad_fbo_state[40];
        int aPositionLoc;
        int uWorldMatrixLoc;
        int aTexCoordLoc;
        int sTextureBaseLoc;

        // Trailing state not yet identified; the original allocates sizeof == 0x60 (operator new in Engine::ShaderInit).
        unsigned char unknownTail[8];

        DrawFBOShader();

        ~DrawFBOShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void RenderEffect(FBOContainer *fbo, Engine *engine) override;
    };

#if __SIZEOF_POINTER__ == 4
    static_assert(sizeof(DrawFBOShader) == 0x60, "DrawFBOShader size (Engine::ShaderInit operator new)");
#endif
}

#endif
