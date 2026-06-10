#ifndef GOF2_VERTEXCOLORALPHATEXTURESHADER_H
#define GOF2_VERTEXCOLORALPHATEXTURESHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

void operator delete(void *ptr) noexcept;

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" char _ZTVN11AbyssEngine29VertexColorAlphaTextureShaderE[];

extern "C" void glUseProgram(uint32_t program);
extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" void glUniform1f(int location, float value);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniform3f(int location, float x, float y, float z);
extern "C" void glUniform3fv(int location, int count, const float *value);
extern "C" void glUniform4fv(int location, int count, const float *value);
extern "C" void glUniformMatrix3fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glEnableVertexAttribArray(uint32_t index);
extern "C" void glDisableVertexAttribArray(uint32_t index);
extern "C" void glBindBuffer(uint32_t target, uint32_t buffer);
extern "C" void glVertexAttribPointer(
    uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);

namespace AbyssEngine {

struct Engine;
struct Mesh;

struct ShaderBaseStruct {
    static int shaderIndexIntern;

    ShaderBaseStruct();
    ~ShaderBaseStruct();

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
};

extern "C" void String_ctor_char(String *self, const char *text, bool copy);
extern "C" void String_assign(String *self, const String *other);
extern "C" void String_dtor(String *self);

// AbyssEngine::VertexColorAlphaTextureShader — GLES2 shader with per-vertex color +
// alpha (derives from ShaderBaseStruct). Holds the program handle, six vertex
// attribute locations and nine uniform locations.
struct VertexColorAlphaTextureShader {
    void *field_0x0;     // vtable
    int field_0x4;       // program handle
    uint8_t field_0x8;
    uint8_t field_0x9;   // dirty flag
    uint8_t field_0xa;
    uint8_t field_0xb;
    String field_0xc;    // name
    int field_0x20;      // attrib a1
    int field_0x24;      // attrib a2
    int field_0x28;      // attrib a3
    int field_0x2c;      // attrib a4
    int field_0x30;      // attrib a5
    int field_0x34;      // attrib a0
    int field_0x38;      // uniform u0
    int field_0x3c;      // uniform u1
    int field_0x40;      // uniform u2
    int field_0x44;      // uniform u3
    int field_0x48;      // uniform u4
    int field_0x4c;      // uniform u5
    int field_0x50;      // uniform u6
    int field_0x54;      // uniform u7
    int field_0x58;      // uniform u8

    void Init(Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
    VertexColorAlphaTextureShader();
};

} // namespace AbyssEngine

#endif
