#ifndef GOF2_BUMPSHADERREFRACT_H
#define GOF2_BUMPSHADERREFRACT_H
#include "gof2/common.h"
// Galaxy on Fire 2 - AbyssEngine::BumpShaderRefract (GLES2 refraction bump shader).
// Derives from ShaderBaseStruct. Field names use the deterministic field_0xNN convention.

extern "C" char _ZTVN11AbyssEngine17BumpShaderRefractE[];

namespace AbyssEngine {

struct Engine;
struct Mesh;

// AbyssEngine::ShaderBaseStruct base layout used by BumpShaderRefract.
struct ShaderBaseStruct {
    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4 GL program handle
    volatile uint16_t field_0x8;        // +0x8

    static int shaderIndexIntern;

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
};

// AbyssEngine::BumpShaderRefract
class BumpShaderRefract : public ShaderBaseStruct  {
public:
    uint8_t field_0x9;                  // +0x9 dirty flag
    String field_0xc;                   // +0xc shader name
    int field_0x20;                     // +0x20 attribute a0
    int field_0x24;                     // +0x24 attribute a1
    int field_0x28;                     // +0x28 uniform u0
    int field_0x2c;                     // +0x2c uniform u1 (world matrix)
    int field_0x30;                     // +0x30 uniform u2 (sampler base, indexed)
    int field_0x34;                     // +0x34 uniform u3
    int field_0x38;                     // +0x38 uniform u4 (texel size)
    int field_0x3c;                     // +0x3c uniform u8 (refract sampler)
    int field_0x40;                     // +0x40 uniform u5
    int field_0x44;                     // +0x44 uniform u6
    int field_0x48;                     // +0x48 uniform u7

    static int ShaderIndex;

    BumpShaderRefract();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine


extern "C" {
void glActiveTexture(uint32_t texture);
void glUseProgram(uint32_t program);
int glGetUniformLocation(uint32_t program, const char *name);
int glGetAttribLocation(uint32_t program, const char *name);
void glUniform1f(int location, float value);
void glUniform1i(int location, int value);
void glUniform2f(int location, float x, float y);
void glUniform3f(int location, float x, float y, float z);
void glUniform3fv(int location, int count, const float *value);
void glUniform4fv(int location, int count, const float *value);
void glUniformMatrix3fv(int location, int count, uint8_t transpose, const void *value);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
void glEnableVertexAttribArray(uint32_t index);
void glDisableVertexAttribArray(uint32_t index);
void glBindBuffer(uint32_t target, uint32_t buffer);
void glVertexAttribPointer(
    uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);

void operator_delete(void *ptr) noexcept;
}

#endif
