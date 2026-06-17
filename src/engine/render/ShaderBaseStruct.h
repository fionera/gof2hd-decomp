#ifndef GOF2_SHADERBASESTRUCT_H
#define GOF2_SHADERBASESTRUCT_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/core/String.h"

// AbyssEngine::ShaderBaseStruct — the GLSL shader base that every concrete shader (BloomShader,
// BumpShader*, GenericShader*, TextureShader, ...) derives from. It owns the GL program handle and
// drives ES2 shader compile/link plus uniform/attribute location binding.


namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;

namespace AbyssEngine {

class Mesh;
class FBOContainer;

class ShaderBaseStruct {
public:
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

    // Dispatched shader vtable (uniform across all concrete shaders):
    //   +0x08 Init   +0x0c UpdateMeshData   +0x10 SetInActive
    //   +0x14/0x18 RenderEffect(source)         (direct post-process draw)
    //   +0x1c/0x20 RenderEffect(source,target)  (post-process draw with FBO swap)
    //   +0x24 DeleteShader   +0x28 UseShader
    virtual void Init(Engine *engine) = 0;
    virtual void UpdateMeshData(Mesh *mesh, Engine *engine) = 0;
    virtual void SetInActive() = 0;
    // Post-process draw slots; base impls are inert (concrete post-process shaders override them).
    virtual void RenderEffect(FBOContainer *source, ::Engine *engine);
    virtual void RenderEffect(FBOContainer *source, FBOContainer **target, ::Engine *engine);
    virtual void DeleteShader();
    virtual void UseShader(bool useExtra);

    String GetShaderName();
    void Update();
    uint32_t ES2LoadProgram(const char *vertexSource, const char *fragmentSource);
    uint32_t ES2LoadShader(uint32_t type, const char *source);
    uint32_t LoadBindShader(const char *vertexPath, const char *fragmentPath);
};

} // namespace AbyssEngine

#endif
