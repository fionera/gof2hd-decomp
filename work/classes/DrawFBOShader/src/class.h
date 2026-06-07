#ifndef WORK_CLASSES_DRAWFBOSHADER_SRC_CLASS_H
#define WORK_CLASSES_DRAWFBOSHADER_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;

void operator delete(void *ptr) noexcept;

namespace AbyssEngine {

namespace AEMath {
struct Matrix {
    uint32_t m[15];
};
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

struct DrawFBOShader : ShaderBaseStruct {
    DrawFBOShader();
    ~DrawFBOShader() override;

    void Init(Engine *engine);
    void SetInActive();
    void RenderEffect(FBOContainer *fbo, Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine);

    static int32_t ShaderIndex;
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
