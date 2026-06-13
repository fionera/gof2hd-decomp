#ifndef GOF2_POSTBWSHADER_H
#define GOF2_POSTBWSHADER_H
#include "gof2/common.h"
#include <new>
// Galaxy on Fire 2 - AbyssEngine::PostBWShader (post-process black & white shader).
// Derives from ShaderBaseStruct. Field names use the deterministic field_0xNN convention.
namespace AbyssEngine {

struct FBOContainer;
struct Mesh;

// AbyssEngine::Engine - minimal interface used by the post-process shader.
struct Engine {
    int GetDisplayWidth();
    int GetDisplayHeight();
    void SetWorldViewMatrix(const AEMath::Matrix &matrix);
    void DrawQuad(int x, int y, int width, int height);
};

struct FBOContainer {
    void Activate();
};

// AbyssEngine::ShaderBaseStruct - common base for the GLES2 shaders.
struct ShaderBaseStruct {
    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4
    volatile uint16_t field_0x8;        // +0x8

    static int shaderIndexIntern;

    ShaderBaseStruct();
    virtual ~ShaderBaseStruct();
    virtual void Init(Engine *engine);
    virtual void SetActive();
    virtual void SetInActive();
    virtual void UpdateMeshData(Mesh *mesh, Engine *engine);
    virtual void RenderEffect(FBOContainer *fbo, Engine *engine);
    uint32_t ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
};

// AbyssEngine::PostBWShader
class PostBWShader : public ShaderBaseStruct  {
public:
    uint8_t field_0x9;                  // +0x9 dirty flag
    String field_0xc;                   // +0xc shader name
    uint32_t field_0x4_program;         // GL program handle (stored at base +0x4)
    int field_0x20;                     // +0x20 attribute a_Position
    int field_0x24;                     // +0x24 uniform u_MVPMatrix
    int field_0x28;                     // +0x28 attribute a_TexCoord
    int field_0x2c;                     // +0x2c uniform s_Texture

    PostBWShader();
    ~PostBWShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void RenderEffect(FBOContainer *fbo, Engine *engine);
};

} // namespace AbyssEngine

extern "C" {
extern void *PostBWShader_vtable[];
extern uint32_t PostBWShader_ctor_copy_src;
extern uint32_t PostBWShader_ctor_copy_dst;

void glActiveTexture(uint32_t texture);
void glBindBuffer(uint32_t target, uint32_t buffer);
void glBindFramebuffer(uint32_t target, uint32_t framebuffer);
void glBlendFunc(uint32_t sfactor, uint32_t dfactor);
void glClear(uint32_t mask);
void glDepthMask(uint8_t flag);
void glDisable(uint32_t cap);
void glDisableVertexAttribArray(uint32_t index);
void glEnable(uint32_t cap);
void glEnableVertexAttribArray(uint32_t index);
int glGetAttribLocation(uint32_t program, const char *name);
int glGetUniformLocation(uint32_t program, const char *name);
void glUniform1i(int location, int value);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
void glUseProgram(uint32_t program);
void glVertexAttribPointer(
    uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);
void glViewport(int x, int y, int width, int height);
}

#endif
