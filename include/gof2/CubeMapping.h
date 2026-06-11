#ifndef GOF2_CUBEMAPPING_H
#define GOF2_CUBEMAPPING_H
#include "gof2/common.h"
// AbyssEngine::CubeMapping — GLES2 cube-mapping shader.
// Derives from ShaderBaseStruct; holds one GL program plus its attrib/uniform locations.

extern "C" {
extern void *__stack_chk_guard;
extern char CubeMapping_vtable[];
extern int CubeMapping_typeInfoSource;
extern int CubeMapping_typeInfoDest;
extern const char CubeMapping_name[];

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

void operator_delete(void *ptr) noexcept;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;
}

namespace AbyssEngine {

struct Engine;
struct Mesh;

class CubeMapping {
public:
    void* field_0x0;        // +0x0  vtable
    int program;          // +0x4  GL program
    uint8_t field_0x8;      // +0x8
    uint8_t uniformsDirty;      // +0x9  per-frame uniform dirty flag
    uint8_t field_0xa;      // +0xa
    uint8_t field_0xb;      // +0xb
    String name;       // +0xc  resource name
    int field_0x18;         // +0x18
    int field_0x1c;         // +0x1c
    int aPosition;         // +0x20 attrib a0 (position)
    int aNormal;         // +0x24 attrib a1 (normal)
    int aTexCoord;         // +0x28 attrib a2 (texcoord)
    int uMvp;         // +0x2c uniform u0 (mvp)
    int uNormalMatrix;         // +0x30 uniform u1 (normal matrix)
    int uniform2;         // +0x34 uniform u2
    int uniform3;         // +0x38 uniform u3
    int uniform5;         // +0x3c uniform u5
    int uniform4;         // +0x40 uniform u4
    int uniform6;         // +0x44 uniform u6
    int uniform7;         // +0x48 uniform u7
    int uniform8;         // +0x4c uniform u8
    int uniform9;         // +0x50 uniform u9
    int uniform10;         // +0x54 uniform u10
    int uniform11;         // +0x58 uniform u11

    CubeMapping();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
