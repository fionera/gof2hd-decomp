#ifndef GOF2_BUMPSHADERPARTICLE_H
#define GOF2_BUMPSHADERPARTICLE_H
#include "gof2/common.h"
#include <new>
// Galaxy on Fire 2 -- AbyssEngine::BumpShaderParticle (Android libgof2hdaa.so, armv7 Thumb).
// GLES2 bump-mapping shader for particle meshes. Derives from ShaderBaseStruct: program handle
// at 0x4, dirty flag byte at 0x9, name String at 0xc, attribute/uniform locations 0x20..0x68.
// All members are named typed fields declared in offset order (natural 64-bit layout).

extern "C" char _ZTVN11AbyssEngine18BumpShaderParticleE[];

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

// AbyssEngine::ShaderBaseStruct base layout used by BumpShaderParticle.
struct ShaderBaseStruct {
    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4 GL program handle
    volatile uint16_t field_0x8;        // +0x8

    static int shaderIndexIntern;

    ShaderBaseStruct();
    ~ShaderBaseStruct();

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
};

// AbyssEngine::BumpShaderParticle - derives from ShaderBaseStruct (shared shader storage layout).
class BumpShaderParticle : public ShaderBaseStruct {
public:
    uint8_t field_0x9;                  // +0x9 dirty/init flag
    String name;                        // +0xc shader name ("BumpShaderParticle")
    int attribA0;                       // +0x20 attribute "a0" (position)
    int attribA1;                       // +0x24 attribute "a1" (texcoord)
    int attribA2;                       // +0x28 attribute "a2"
    int attribA3;                       // +0x2c attribute "a3"
    int attribA4;                       // +0x30 attribute "a4"
    int uniformU0;                      // +0x34 uniform "u0"
    int uniformU1;                      // +0x38 uniform "u1" (modelview matrix)
    int uniformU2;                      // +0x3c uniform "u2" (normal matrix)
    int uniformU3;                      // +0x40 uniform "u3" (light dir / pos)
    int uniformU4;                      // +0x44 uniform "u4"
    int uniformU5;                      // +0x48 uniform "u5" (sampler, indexed base)
    int uniformU6;                      // +0x4c uniform "u6" (sampler)
    int uniformU7;                      // +0x50 uniform "u7"
    int uniformU8;                      // +0x54 uniform "u8"
    int uniformU9;                      // +0x58 uniform "u9"
    int uniformU10;                     // +0x5c uniform "u10"
    int uniformU11;                     // +0x60 uniform "u11"
    int uniformU12;                     // +0x64 uniform "u12" (global param A)
    int uniformU13;                     // +0x68 uniform "u13" (global param B)

    static int ShaderIndex;

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    BumpShaderParticle();
};

} // namespace AbyssEngine

#endif
