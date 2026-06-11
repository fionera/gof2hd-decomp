#ifndef GOF2_GENERICSHADER1_H
#define GOF2_GENERICSHADER1_H
#include "gof2/common.h"
// AbyssEngine::GenericShader1 (Android libgof2hdaa.so, armv7 Thumb).
// GLES2 normal-mapped lighting shader. Derives from ShaderBaseStruct.
// Field names use the deterministic field_0xNN convention.
namespace AbyssEngine {

struct Engine;
struct Mesh;

// AbyssEngine::ShaderBaseStruct base layout used by GenericShader1.
struct ShaderBaseStruct {
    void    *field_0x0;                 // +0x0 vtable
    int      field_0x4;                 // +0x4 GL program handle
    volatile uint16_t field_0x8;        // +0x8
};

// AbyssEngine::GenericShader1
class GenericShader1 : public ShaderBaseStruct  {
public:
    uint8_t  field_0x9;                 // +0x9 uniform-dirty flag
    String   field_0xc;                 // +0xc shader name
    int      field_0x20;                // +0x20 attribute a_position
    int      field_0x24;                // +0x24 attribute a_normal
    int      field_0x28;                // +0x28 attribute a_tangent
    int      field_0x2c;                // +0x2c attribute a_binormal
    int      field_0x30;                // +0x30 attribute a_texCoord
    int      field_0x34;                // +0x34 uniform u_m0
    int      field_0x38;                // +0x38 uniform u_m1
    int      field_0x3c;                // +0x3c uniform u_m2
    int      field_0x40;                // +0x40 uniform u_m3
    int      field_0x44;                // +0x44 uniform u_m4
    int      field_0x48;                // +0x48 uniform u_m5
    int      field_0x4c;                // +0x4c uniform u_m6
    int      field_0x50;                // +0x50 uniform u_m7
    int      field_0x54;                // +0x54 uniform u_m8

    GenericShader1();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" char _ZTVN11AbyssEngine14GenericShader1E[];

extern "C" {
void ShaderBaseStruct_ctor(void *self);
void *ShaderBaseStruct_dtor(void *self);
uint32_t ShaderBaseStruct_ES2LoadProgram(void *self, const char *vertex, const char *fragment);

int glGetAttribLocation(uint32_t program, const char *name);
int glGetUniformLocation(uint32_t program, const char *name);
void glUseProgram(uint32_t program);
void glUniform1i(int location, int value);
void glUniform3f(int location, float x, float y, float z);
void glUniform4fv(int location, int count, const float *value);
void glUniformMatrix3fv(int location, int count, uint8_t transpose, const void *value);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
void glEnableVertexAttribArray(uint32_t index);
void glDisableVertexAttribArray(uint32_t index);
void glVertexAttribPointer(uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);

extern int32_t ShaderBaseStruct_shaderIndexIntern;
extern int32_t GenericShader1_ShaderIndex;
extern void *GenericShader1_vtable[];

void operator_delete(void *ptr) noexcept;
}

#endif
