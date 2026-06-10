#ifndef GOF2_POSTBWSHADER_H
#define GOF2_POSTBWSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

struct FBOContainer;
struct Mesh;

namespace AEMath {

} // namespace AEMath



struct Engine {
    int GetDisplayWidth();
    int GetDisplayHeight();
    void SetWorldViewMatrix(const AEMath::Matrix &matrix);
    void DrawQuad(int x, int y, int width, int height);
};

struct FBOContainer {
    void Activate();
};

struct ShaderBaseStruct {
    ShaderBaseStruct();
#ifdef POSTBW_NONVIRTUAL_DTOR
    ~ShaderBaseStruct();
#else
    virtual ~ShaderBaseStruct();
    virtual void Init(Engine *engine);
    virtual void SetActive();
    virtual void SetInActive();
    virtual void UpdateMeshData(Mesh *mesh, Engine *engine);
    virtual void RenderEffect(FBOContainer *fbo, Engine *engine);
    uint32_t ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
};



} // namespace AbyssEngine

extern "C" void __stack_chk_fail(...);

inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }

extern "C" void glActiveTexture(uint32_t texture);
extern "C" void glBindBuffer(uint32_t target, uint32_t buffer);
extern "C" void glBindFramebuffer(uint32_t target, uint32_t framebuffer);
extern "C" void glBlendFunc(uint32_t sfactor, uint32_t dfactor);
extern "C" void glClear(uint32_t mask);
extern "C" void glDepthMask(uint8_t flag);
extern "C" void glDisable(uint32_t cap);
extern "C" void glDisableVertexAttribArray(uint32_t index);
extern "C" void glEnable(uint32_t cap);
extern "C" void glEnableVertexAttribArray(uint32_t index);
extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glUseProgram(uint32_t program);
extern "C" void glVertexAttribPointer(
    uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);
extern "C" void glViewport(int x, int y, int width, int height);

static inline char *bytes(void *p) { return (char *)p; }
static inline const char *bytes(const void *p) { return (const char *)p; }

static inline uint8_t &field_u8(void *p, uint32_t off) { return *(uint8_t *)(bytes(p) + off); }
static inline int32_t &field_i32(void *p, uint32_t off) { return *(int32_t *)(bytes(p) + off); }
static inline uint32_t &field_u32(void *p, uint32_t off) { return *(uint32_t *)(bytes(p) + off); }
static inline float &field_f32(void *p, uint32_t off) { return *(float *)(bytes(p) + off); }
static inline void *&field_ptr(void *p, uint32_t off) { return *(void **)(bytes(p) + off); }

struct PostBWShader { void* _opaque; };  // no offset accesses observed
#endif
