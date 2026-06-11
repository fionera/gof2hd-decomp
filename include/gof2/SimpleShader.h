#ifndef GOF2_SIMPLESHADER_H
#define GOF2_SIMPLESHADER_H
#include "gof2/common.h"
// Galaxy on Fire 2 - AbyssEngine::SimpleShader (GLES2 solid-color shader).
// Derives from ShaderBaseStruct. Field names use the deterministic field_0xNN convention.
namespace AbyssEngine {

struct Engine;
struct Mesh;

// AbyssEngine::ShaderBaseStruct base layout used by SimpleShader.
struct ShaderBaseStruct {
    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4 GL program handle
    volatile uint16_t field_0x8;        // +0x8
};

// AbyssEngine::SimpleShader
class SimpleShader : public ShaderBaseStruct  {
public:
    uint8_t field_0x9;                  // +0x9 color-dirty flag
    String field_0xc;                   // +0xc shader name
    int field_0x20;                     // +0x20 attribute a_position
    int field_0x24;                     // +0x24 uniform u_WorldMatrix
    int field_0x28;                     // +0x28 uniform u_color

    SimpleShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

extern "C" {
void ShaderBaseStruct_ctor(void *self);
void *ShaderBaseStruct_dtor(void *self);
uint32_t ShaderBaseStruct_ES2LoadProgram(void *self, const char *vertex, const char *fragment);

int glGetAttribLocation(uint32_t program, const char *name);
int glGetUniformLocation(uint32_t program, const char *name);
void glUseProgram(uint32_t program);
void glUniform1i(int location, int value);
void glUniform4fv(int location, int count, const float *value);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
void glEnableVertexAttribArray(uint32_t index);
void glDisableVertexAttribArray(uint32_t index);
void glVertexAttribPointer(uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);
void glBindBuffer(uint32_t target, uint32_t buffer);

extern void *SimpleShader_vtable[];
extern int32_t ShaderBaseStruct_shaderIndexIntern;
extern int32_t SimpleShader_ShaderIndex;

void operator_delete(void *ptr) noexcept;
}

#endif
