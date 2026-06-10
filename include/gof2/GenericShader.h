#ifndef GOF2_GENERICSHADER_H
#define GOF2_GENERICSHADER_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- AbyssEngine::GenericShader (ES2 program wrapper).
// Holds the GL program handle plus the attribute/uniform locations queried at Init.

struct Engine;   // global engine object (opaque here; only raw GL data is read)
struct Mesh;     // global mesh object (opaque here; only raw GL data is read)

using Vector = AbyssEngine::AEMath::Vector;

extern "C" {
extern void *__stack_chk_guard;
extern char GenericShader_vtable[];
extern int GenericShader_typeInfoSource;
extern int GenericShader_typeInfoDest;
extern const char GenericShader_name[];

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

float *Vector_cast_to_float(Vector *self);

void operator_delete(void *ptr) noexcept;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;
}

namespace AbyssEngine {

struct GenericShader {
    void*    field_0x0;     // +0x00 vtable
    int32_t  field_0x4;     // +0x04 GL program handle
    uint8_t  field_0x9;     // +0x09 lighting-uniforms-dirty flag
    String   field_0xc;     // +0x0c shader name
    // attribute locations (a0..a4)
    int32_t  field_0x20;    // +0x20 a0
    int32_t  field_0x24;    // +0x24 a1
    int32_t  field_0x28;    // +0x28 a2
    int32_t  field_0x2c;    // +0x2c a3
    int32_t  field_0x30;    // +0x30 a4
    // uniform locations (u0..u11)
    int32_t  field_0x34;    // +0x34 u0
    int32_t  field_0x38;    // +0x38 u1
    int32_t  field_0x3c;    // +0x3c u2
    int32_t  field_0x40;    // +0x40 u3
    int32_t  field_0x44;    // +0x44 u4
    int32_t  field_0x48;    // +0x48 u5
    int32_t  field_0x4c;    // +0x4c u6
    int32_t  field_0x50;    // +0x50 u7
    int32_t  field_0x54;    // +0x54 u8
    int32_t  field_0x58;    // +0x58 u9
    int32_t  field_0x5c;    // +0x5c u10
    int32_t  field_0x60;    // +0x60 u11

    GenericShader();
    ~GenericShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

// Note: fwd.h forward-declares a distinct global `::GenericShader`; the real class
// lives in namespace AbyssEngine, so we do not alias it into the global scope.

#endif
