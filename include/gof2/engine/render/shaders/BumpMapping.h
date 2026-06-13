#ifndef GOF2_BUMPMAPPING_H
#define GOF2_BUMPMAPPING_H
#include "gof2/common.h"
#include <new>
// struct derived from offset-access field map (deterministic field_0xNN naming)

extern "C" char _ZTVN11AbyssEngine11BumpMappingE[];

extern "C" void glUseProgram(uint32_t program);
extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" void glUniform1f(int location, float value);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniform3f(int location, float x, float y, float z);
extern "C" void glUniform3fv(int location, int count, const float *value);
extern "C" void glUniform4fv(int location, int count, const float *value);
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

    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4 GL program handle
    volatile uint16_t field_0x8;        // +0x8 flags

    ShaderBaseStruct();
    ~ShaderBaseStruct();

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
};

class BumpMapping : public ShaderBaseStruct  {
public:
    uint8_t field_0x9;                  // +0x9 dirty flag
    String field_0xc;                   // +0xc shader name
    int field_0x20;                     // +0x20 attrib a_position
    int field_0x24;                     // +0x24 attrib a_normal
    int field_0x28;                     // +0x28 attrib a_tangent
    int field_0x2c;                     // +0x2c attrib a_binormal
    int field_0x30;                     // +0x30 attrib a_texCoord
    int field_0x34;                     // +0x34 uniform u_mvpMatrix
    int field_0x38;                     // +0x38 uniform u_lightDir
    int field_0x3c;                     // +0x3c uniform u_texture
    int field_0x40;                     // +0x40 uniform u_normalMap

    static int ShaderIndex;

    BumpMapping();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

// cross-class field accessors (Engine/Mesh are not in this batch; opaque here)

} // namespace AbyssEngine

#endif
