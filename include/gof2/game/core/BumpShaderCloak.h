#ifndef GOF2_BUMPSHADERCLOAK_H
#define GOF2_BUMPSHADERCLOAK_H
#include "gof2/common.h"
// Full named layout (recovered from the binary). Trailing comments are the original
// 32-bit field offsets, kept for cross-reference; the live layout is natural 64-bit.
#include <new>

extern "C" char _ZTVN11AbyssEngine15BumpShaderCloakE[];

extern "C" void glUseProgram(uint32_t program);
extern "C" void glActiveTexture(uint32_t texture);
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

    ShaderBaseStruct();
    ~ShaderBaseStruct();

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
};

// AbyssEngine::BumpShaderCloak — GLES2 refraction/cloak shader (derives from ShaderBaseStruct).
// The ctor placement-news a ShaderBaseStruct into `this` and installs the BumpShaderCloak
// vtable manually, so the base-region fields (vtable / program handle / name String) are
// modelled here as named members rather than via inheritance.
//
// Fields are GL handles: programId is the linked shader program, attrib_* hold vertex
// attribute locations and uniform_* hold uniform locations (-1 when the GLSL compiler
// optimised the corresponding name away).
class BumpShaderCloak {
public:
    void *vtable;                        // +0x00  installed in ctor
    int programId;                       // +0x04  glLinkProgram handle
    unsigned char field_0x8;             // +0x08
    unsigned char dirty;                 // +0x09  re-upload material uniforms this frame
    unsigned char field_0xa;             // +0x0a
    unsigned char field_0xb;             // +0x0b
    String name;                         // +0x0c  "BumpShaderCloak"

    int attrib_a0;                       // +0x20  vertex position
    int attrib_a1;                       // +0x24  texcoord
    int attrib_a2;                       // +0x28  normal
    int attrib_a3;                       // +0x2c  tangent
    int attrib_a4;                       // +0x30  binormal

    int uniform_u0;                      // +0x34  mvp matrix (mat4)
    int uniform_u1;                      // +0x38  normal matrix (mat3)
    int uniform_u2;                      // +0x3c  light/ambient color
    int uniform_u3;                      // +0x40  fallback diffuse color
    int uniform_u4;                      // +0x44
    int uniform_u5;                      // +0x48  texture sampler 0
    int uniform_u6;                      // +0x4c  texture sampler 1
    int uniform_u7;                      // +0x50  texture sampler 6
    int uniform_u8;                      // +0x58
    int uniform_u9;                      // +0x5c
    int uniform_u12;                     // +0x60
    int uniform_u10;                     // +0x64
    int uniform_u13;                     // +0x68
    int uniform_u11;                     // +0x6c
    int uniform_u14;                     // +0x70
    int uniform_u15;                     // +0x74
    int uniform_u16;                     // +0x78
    int uniform_u18;                     // +0x80  mesh-derived param @mesh+0x1c
    int uniform_u19;                     // +0x84  mesh-derived param @mesh+0x20
    int uniform_u17;                     // +0x8c  viewport inverse-size (vec2)
    int uniform_u20;                     // +0x90  refraction sampler 7

    static int ShaderIndex;

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    BumpShaderCloak();
};

} // namespace AbyssEngine

#endif
