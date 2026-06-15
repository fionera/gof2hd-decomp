#ifndef GOF2_SHADERBASESTRUCT_H
#define GOF2_SHADERBASESTRUCT_H
#include "gof2/common.h"
#include "gof2/game/core/String.h"

// AbyssEngine::ShaderBaseStruct — the GLSL shader base that every concrete shader (BloomShader,
// BumpShader*, GenericShader*, TextureShader, ...) derives from. It owns the GL program handle and
// drives ES2 shader compile/link plus uniform/attribute location binding.

// ShaderBaseStruct's C++ vtable symbol (platform-supplied at the engine ABI level).
extern "C" void *_ZTVN11AbyssEngine16ShaderBaseStructE[];

namespace AbyssEngine {

class ShaderBaseStruct {
public:
    void *vtable;
    int program;                // GL program handle (-1 when unset)
    volatile uint16_t flags;
    uint8_t dirty;
    String name;
    const char *vertexPath;
    const char *fragmentPath;

    // Running count of all shaders ever constructed; each concrete shader snapshots it in its
    // constructor as its per-shader index.
    static int shaderIndexIntern;

    ShaderBaseStruct();
    virtual ~ShaderBaseStruct();

    String GetShaderName();
    void UseShader();
    void DeleteShader();
    void Update();
    uint32_t ES2LoadProgram(const char *vertexSource, const char *fragmentSource);
    uint32_t ES2LoadShader(uint32_t type, const char *source);
    uint32_t LoadBindShader(const char *vertexPath, const char *fragmentPath);
};

} // namespace AbyssEngine

#endif
