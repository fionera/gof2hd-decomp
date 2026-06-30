#ifndef GOF2_SHADERBASESTRUCT_H
#define GOF2_SHADERBASESTRUCT_H
#include "Engine.h"
#include "FBOContainer.h"
#include "../core/AEString.h"
#include "Mesh.h"
#include "engine/math/Vector.h"

#include "engine/math/AEMath.h"

namespace AbyssEngine {
    class Engine;
    class FBOContainer;
    class Mesh;
}

namespace AbyssEngine {
    class ShaderBaseStruct {
    public:
        int program;
        // Ghidra ground truth: the 16-bit `flags` is at 0x8 and `dirty` (materialDirty) is its HIGH
        // byte at 0x9. Our decomp had split `dirty` to a separate field at 0xa (the wrong byte, in the
        // padding). Model `dirty` as the real 0x9 byte; the 16-bit `flags` view is a reinterpret over
        // flagsLow (its sole writer sets 0x100 = dirty:=1).
        uint8_t flagsLow;
        uint8_t dirty;
        String name;
        const char *vertexPath;
        const char *fragmentPath;

        static int shaderIndexIntern;

        ShaderBaseStruct();

        virtual ~ShaderBaseStruct();

        virtual void Init(Engine *engine) = 0;

        virtual void UpdateMeshData(Mesh *mesh, Engine *engine) = 0;

        virtual void SetInActive() = 0;

        // vtable order (from original _ZTV ABS32): 2-arg, 4-arg, 3-arg-ref, 5-arg, DeleteShader, UseShader.
        // The 4-arg and 5-arg RenderEffect are VIRTUAL (slots 6 & 8), interleaved — were non-virtual in ours.
        virtual void RenderEffect(FBOContainer *source, Engine *engine);

        virtual FBOContainer *RenderEffect(FBOContainer *source, Engine *engine, float strength, AEMath::Vector tint);

        virtual void RenderEffect(FBOContainer *source, FBOContainer *&target, AbyssEngine::Engine *engine);

        virtual FBOContainer *RenderEffect(FBOContainer *source, FBOContainer *&target, Engine *engine, float strength,
                                           AEMath::Vector tint);

        virtual void DeleteShader();

        virtual void UseShader(bool useExtra);

        String GetShaderName();

        void Update();

        uint32_t ES2LoadProgram(const char *vertexSource, const char *fragmentSource);

        uint32_t ES2LoadShader(uint32_t type, const char *source);

        uint32_t LoadBindShader(const char *vertexPath, const char *fragmentPath);
    };
}

#endif
