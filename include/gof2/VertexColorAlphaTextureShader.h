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

// String_ctor_char / String_assign / String_dtor are declared in gof2/String.h

// AbyssEngine::VertexColorAlphaTextureShader — GLES2 shader with per-vertex color +
// alpha (derives from ShaderBaseStruct). Holds the program handle, six vertex
// attribute locations and nine uniform locations.
struct VertexColorAlphaTextureShader {
    void *vtable;     // vtable
    int program;       // program handle
    uint8_t field_0x8;
    uint8_t dirty;   // dirty flag
    uint8_t field_0xa;
    uint8_t field_0xb;
    String name;    // name
    int attrib1;      // attrib a1
    int attrib2;      // attrib a2
    int attrib3;      // attrib a3
    int attrib4;      // attrib a4
    int attrib5;      // attrib a5
    int attrib0;      // attrib a0
    int uniform0;      // uniform u0
    int uniform1;      // uniform u1
    int uniform2;      // uniform u2
    int uniform3;      // uniform u3
    int uniform4;      // uniform u4
    int uniform5;      // uniform u5
    int uniform6;      // uniform u6
    int uniform7;      // uniform u7
    int uniform8;      // uniform u8

    void Init(Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
    VertexColorAlphaTextureShader();
};

} // namespace AbyssEngine

#endif
