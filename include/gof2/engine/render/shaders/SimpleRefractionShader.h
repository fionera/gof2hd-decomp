#ifndef GOF2_SIMPLEREFRACTIONSHADER_H
#define GOF2_SIMPLEREFRACTIONSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

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

// AbyssEngine::SimpleRefractionShader — GLES2 refraction shader (derives from ShaderBaseStruct).
// Fields accessed through the field_i32/field_u8 helpers; storage covers offsets 0x00..0x54.
class SimpleRefractionShader : public ShaderBaseStruct  {
public:
    static int ShaderIndex;

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);

    // raw field storage (offsets referenced through the field_i32/field_u8 helpers)
    char field_storage[0x58];
    String shaderName;                // shader name ("SimpleRefractionShader")
};

} // namespace AbyssEngine

extern "C" char _ZTVN11AbyssEngine22SimpleRefractionShaderE[];

#endif
