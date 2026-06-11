#ifndef GOF2_SPECCUBEALPHAMAPPING_H
#define GOF2_SPECCUBEALPHAMAPPING_H
#include "gof2/common.h"
// AbyssEngine::SpecCubeAlphaMapping — GLES2 specular-cube + alpha-mapping shader.
// Derives from ShaderBaseStruct; holds one GL program plus its attrib/uniform locations.

extern "C" {
extern void *__stack_chk_guard;
extern char SpecCubeAlphaMapping_vtable[];
extern const char g_SCAM_name[];      // resource-name string literal
extern void **g_SCAM_globalSlotSrc;
extern void **g_SCAM_globalSlotDst;

int glGetAttribLocation(int program, const char *name);
int glGetUniformLocation(int program, const char *name);
void glUseProgram(int program);
void glUniform1i(int location, int value);
void glUniform1f(int location, float value);
void glUniform3f(int location, float x, float y, float z);
void glUniform4f(int location, float x, float y, float z, float w);
void glUniform3fv(int location, int count, const float *value);
void glUniform4fv(int location, int count, const float *value);
void glUniformMatrix3fv(int location, int count, uint8_t transpose, const float *value);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const float *value);
void glEnableVertexAttribArray(int index);
void glDisableVertexAttribArray(int index);
void glVertexAttribPointer(int index, int size, unsigned int type, uint8_t normalized,
                           int stride, const void *pointer);
void glBindBuffer(unsigned int target, unsigned int buffer);

void ShaderBaseStruct_ctor(void *self);
void *ShaderBaseStruct_dtor(void *self) noexcept;
int ShaderBaseStruct_ES2LoadProgram(void *self, const char *vertexSource, const char *fragmentSource);
int ShaderBaseStruct_LoadBindShader(void *self, const char *vertexPath, const char *fragmentPath);

void operator_delete(void *ptr) noexcept;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;
}

namespace AbyssEngine {

struct Engine;
struct Mesh;

struct SpecCubeAlphaMapping {
    void* field_0x0;        // +0x0  vtable
    int program;          // +0x4  GL program
    uint8_t field_0x8;      // +0x8
    uint8_t uniformsDirty;      // +0x9  per-frame uniform dirty flag
    uint8_t field_0xa;      // +0xa
    uint8_t field_0xb;      // +0xb
    String name;       // +0xc  resource name
    int attrA0;         // +0x20 attrib a0 (position)
    int attrA1;         // +0x24 attrib a1 (normal)
    int attrA2;         // +0x28 attrib a2 (texcoord)
    int uniU0;         // +0x2c uniform u0 (mvp)
    int uniU1;         // +0x30 uniform u1 (normal matrix)
    int uniU2;         // +0x34 uniform u2
    int uniU3;         // +0x38 uniform u3
    int uniU5;         // +0x3c uniform u5
    int uniU4;         // +0x40 uniform u4
    int uniU6;         // +0x44 uniform u6
    int uniU7;         // +0x48 uniform u7
    int uniU8;         // +0x4c uniform u8
    int uniU9;         // +0x50 uniform u9
    int uniU10;         // +0x54 uniform u10
    int uniU11;         // +0x58 uniform u11

    SpecCubeAlphaMapping();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
