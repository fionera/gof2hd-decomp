#ifndef GOF2_POSTBWSHADER_H
#define GOF2_POSTBWSHADER_H
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
    class PostBWShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int aPositionLoc;
        int uWorldMatrixLoc;
        int aTexCoordLoc;
        int sTextureBaseLoc;

        // Trailing state not yet identified; the original allocates sizeof == 0x34 (operator new in Engine::ShaderInit).
        unsigned char unknownTail[4];

        PostBWShader();

        ~PostBWShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void RenderEffect(FBOContainer *fbo, Engine *engine) override;
    };

#if __SIZEOF_POINTER__ == 4
    static_assert(sizeof(PostBWShader) == 0x34, "PostBWShader size (Engine::ShaderInit operator new)");
#endif
}

#endif
