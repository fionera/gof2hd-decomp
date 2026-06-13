#ifndef GOF2_BUMPSHADERV3_H
#define GOF2_BUMPSHADERV3_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

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
// Named layout recovered from the per-method offset accesses. Trailing comments are the
// original 32-bit field offsets, kept for cross-reference; the live layout is natural 64-bit.
//
// ShaderBaseStruct is modelled as an empty base here, so the base-region slots this class
// touches (vtable / dirty flag / name String) are declared inline below in offset order.
class BumpShaderV3 : public ShaderBaseStruct  {
public:
    // ---- base-region slots (ShaderBaseStruct storage this class accesses) ----
    void *vtable;                 // +0x00  vtable pointer (installed in the ctor)
    int programId;                // +0x04  GL shader program handle (glCreateProgram)
    uint8_t field_0x8;            // +0x08
    uint8_t dirty;                // +0x09  uniforms-need-upload flag (cleared after upload)
    uint8_t field_0xa;            // +0x0a
    uint8_t field_0xb;            // +0x0b
    String name;                  // +0x0c  "BumpShaderV3"
    int field_0x18;               // +0x18
    int field_0x1c;               // +0x1c

    // ---- attribute locations (a0..a4) ----
    int attrib_position;          // +0x20  a0  vertex position
    int attrib_texCoord;          // +0x24  a1  texture coordinate
    int attrib_normal;            // +0x28  a2  normal
    int attrib_tangent;           // +0x2c  a3  tangent
    int attrib_bitangent;         // +0x30  a4  bitangent

    // ---- uniform locations (u0..u19) ----
    int uniform_mvpMatrix;        // +0x34  u0  model-view-projection matrix (mat4)
    int uniform_normalMatrix;     // +0x38  u1  normal matrix (mat3)
    int field_0x3c;               // +0x3c  u2
    int field_0x40;               // +0x40  u3
    int field_0x44;               // +0x44  u4
    int uniform_texture0;         // +0x48  u5  sampler unit 0
    int uniform_texture1;         // +0x4c  u6  sampler unit 1
    int uniform_texture2;         // +0x50  u7  sampler unit 2
    int field_0x54;               // +0x54  u8
    int field_0x58;               // +0x58  u9
    int field_0x5c;               // +0x5c  u12
    int field_0x60;               // +0x60  u10
    int field_0x64;               // +0x64  u13
    int field_0x68;               // +0x68  u11
    int field_0x6c;               // +0x6c  u14
    int field_0x70;               // +0x70  u15
    int field_0x74;               // +0x74  u16
    int field_0x78;               // +0x78  u17
    int field_0x7c;               // +0x7c  u18
    int field_0x80;               // +0x80  u19

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    BumpShaderV3();

    static int ShaderIndex;
};

} // namespace AbyssEngine

#endif
