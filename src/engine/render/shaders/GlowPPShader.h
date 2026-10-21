#ifndef GOF2_GLOWPPSHADER_H
#define GOF2_GLOWPPSHADER_H
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
    class GlowPPShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        unsigned int copyProgram;
        int copyAttribPosition;
        int copyUniformWorldView;
        int copyAttribTexCoord;
        int copyUniformTexture;
        uint32_t field_0x34;
        uint32_t field_0x38;

        FBOContainer *copyTarget;

        unsigned int blurXProgram;
        int blurXAttribPosition;
        int blurXUniformWorldView;
        int blurXAttribTexCoord;
        int blurXUniformTexture;
        int blurXUniformSampleSize;

        FBOContainer *blurXTarget;

        unsigned int blurYProgram;
        int blurYAttribPosition;
        int blurYUniformWorldView;
        int blurYAttribTexCoord;
        int blurYUniformTexture;
        int blurYUniformSampleSize;

        FBOContainer *blurYTarget;

        unsigned int combineProgram;
        int combineAttribPosition;
        int combineUniformWorldView;
        int combineAttribTexCoord;
        int combineUniformTexture;
        int combineUniformTexture2;

        int meshAttribPosition;
        uint32_t field_0x94;
        int meshAttribTexCoord;
        uint32_t field_0x9c;

        FBOContainer *backgroundTarget;

        // Trailing state not yet identified; the original allocates sizeof == 0xa8 (operator new in Engine::ShaderInit).
        unsigned char unknownTail[4];

        GlowPPShader();

        ~GlowPPShader();

        void Init(Engine *engine) override;

        void InternalInit(Engine *engine);

        void RenderEffect(FBOContainer *source, FBOContainer *&target, Engine *engine) override;

        void RenderEffect(FBOContainer *source, Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };

#if __SIZEOF_POINTER__ == 4
    static_assert(sizeof(GlowPPShader) == 0xa8, "GlowPPShader size (Engine::ShaderInit operator new)");
#endif
}
#endif
