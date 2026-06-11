#ifndef GOF2_GLOWSHADER_H
#define GOF2_GLOWSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

extern "C" void glUseProgram(uint32_t program);
extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniform1f(int location, float value);
extern "C" void glUniform3f(int location, float x, float y, float z);
extern "C" void glUniform3fv(int location, int count, const float *value);
extern "C" void glUniform4fv(int location, int count, const float *value);
extern "C" void glUniformMatrix3fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glEnableVertexAttribArray(uint32_t index);
extern "C" void glDisableVertexAttribArray(uint32_t index);
extern "C" void glBindBuffer(uint32_t target, uint32_t buffer);
extern "C" void glActiveTexture(uint32_t texture);
extern "C" void glBindTexture(uint32_t target, uint32_t texture);
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
    int LoadBindShader(const char *vertexPath, const char *fragmentPath);
};



// Per-class backing storage accessor helpers. The GlowShader fields live at fixed byte
// offsets (program handle at 0x4, name String at 0xc, attribute/uniform locations 0x20..0x30)
// and are read/written through these helpers.




// AbyssEngine::GlowShader — GLES2 glow shader (derives from ShaderBaseStruct).
class GlowShader {
public:
    static int ShaderIndex;

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);

    // raw field storage (offsets referenced through the field_i32/field_u8/field_ptr helpers)
    char field_storage[0x34];
};

} // namespace AbyssEngine

extern "C" char _ZTVN11AbyssEngine10GlowShaderE[];

#endif
