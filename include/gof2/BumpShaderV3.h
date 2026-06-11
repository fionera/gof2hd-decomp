#ifndef GOF2_BUMPSHADERV3_H
#define GOF2_BUMPSHADERV3_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" char _ZTVN11AbyssEngine12BumpShaderV3E[];

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







// AbyssEngine::BumpShaderV3 — GLES2 bump-mapping shader (derives from ShaderBaseStruct).
// Members accessed via field_i32(this, 0xNN) helpers above; backing storage holds the
// shader-program handle, attribute/uniform locations and the dirty flag.
class BumpShaderV3 : public ShaderBaseStruct  {
public:
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    BumpShaderV3();

    // raw field storage (offsets referenced through the field_i32/field_u8 helpers)
    char field_storage[0x84];
};

} // namespace AbyssEngine

#endif
