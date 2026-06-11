#ifndef GOF2_DRAWFBOSHADER_H
#define GOF2_DRAWFBOSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
void operator delete(void *ptr) noexcept;

namespace AbyssEngine {

namespace AEMath {

} // namespace AEMath

struct Engine {
    int32_t GetDisplayWidth();
    int32_t GetDisplayHeight();
    void SetWorldViewMatrix(const AEMath::Matrix &matrix);
    bool IsPostEffectActivated();
    void ActivateRender2FracFBO();
    void DeactivateRender2FracFBO();
    void DrawQuad(int x, int y, int width, int height);
};

struct FBOContainer {
    void Activate();
};

struct Mesh;

struct ShaderBaseStruct {
    ShaderBaseStruct();
    virtual ~ShaderBaseStruct();

    uint32_t ES2LoadProgram(const char *vertexShader, const char *fragmentShader);

    static int32_t shaderIndexIntern;
};

// AbyssEngine::DrawFBOShader — GLES2 full-screen FBO blit shader (derives from ShaderBaseStruct).
// Fields (program handle at 0x4, dirty byte at 0x9, attribute/uniform locations 0x48..0x54)
// are accessed through raw byte casts; storage covers offsets 0x00..0x54.
class DrawFBOShader : public ShaderBaseStruct  {
public:
    static int32_t ShaderIndex;

    DrawFBOShader();
    ~DrawFBOShader();

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void RenderEffect(FBOContainer *fbo, Engine *engine);

    char field_storage[0x58];
};

} // namespace AbyssEngine

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" void glUseProgram(uint32_t program);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glEnableVertexAttribArray(uint32_t index);
extern "C" void glDisableVertexAttribArray(uint32_t index);
extern "C" void glVertexAttribPointer(uint32_t index, int size, uint32_t type,
                                      uint8_t normalized, int stride, const void *pointer);
extern "C" void glBindBuffer(uint32_t target, uint32_t buffer);
extern "C" void glBindFramebuffer(uint32_t target, uint32_t framebuffer);
extern "C" void glViewport(int x, int y, int width, int height);
extern "C" void glDisable(uint32_t cap);
extern "C" void glEnable(uint32_t cap);
extern "C" void glDepthMask(uint8_t flag);
extern "C" void glActiveTexture(uint32_t texture);
extern "C" void glClear(uint32_t mask);

#endif
