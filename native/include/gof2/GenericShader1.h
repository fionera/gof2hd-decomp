#ifndef GOF2_GENERICSHADER1_H
#define GOF2_GENERICSHADER1_H
#include "gof2/common.h"
#include <new>
// AbyssEngine::GenericShader1 (Android libgof2hdaa.so, armv7 Thumb).
// Real named struct recovered from the byte-offset field map.

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" char _ZTVN11AbyssEngine14GenericShader1E[];

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

struct Mesh;  // defined in gof2/Mesh.h at global scope

namespace AbyssEngine {

struct Engine {
    // Only the byte offsets actually read by this shader are named.
    char         field_0x00[0xd0];
    float        glColor[4];          // +0xd0  (u_m5 source)
    char         field_0xe0[0x24];
    float        worldMatrix[16];     // +0x104 (u_m0, mat4)
    char         field_0x144[0xc0];
    float        normalMatrix[9];     // +0x204 (u_m1, mat3)
    char         field_0x228[0x70];
    float        lightDir2[4];        // +0x298 (u_m7 source)
    float        lightColor[4];       // +0x2a8 (u_m6 source)
    float        lightDir[4];         // +0x2b8 (u_m8 source)
    char         field_0x2c8[0x68];
    float        eyePosWorldX;        // +0x330 (u_m2 source)
    float        eyePosWorldY;        // +0x334
    float        eyePosWorldZ;        // +0x338
    char         field_0x33c[0x10];
    float        eyePosModelX;        // +0x34c (u_m3 source)
    float        eyePosModelY;        // +0x350
    float        eyePosModelZ;        // +0x354
};

struct ShaderBaseStruct {
    static int shaderIndexIntern;

    ShaderBaseStruct();
    ~ShaderBaseStruct();

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
};

struct GenericShader1 {
    void    *vtable;                  // +0x00
    int      program;                 // +0x04
    uint8_t  field_0x08;              // +0x08
    uint8_t  needsUniformUpdate;      // +0x09
    uint16_t field_0x0a;              // +0x0a
    String   field_0xc;               // +0x0c  (shader name; kept name per fieldmap)
    int      loc_a_position;          // +0x20
    int      loc_a_normal;            // +0x24
    int      loc_a_tangent;           // +0x28
    int      loc_a_binormal;          // +0x2c
    int      loc_a_texCoord;          // +0x30
    int      loc_u_m0;                // +0x34
    int      loc_u_m1;                // +0x38
    int      loc_u_m2;                // +0x3c
    int      loc_u_m3;                // +0x40
    int      loc_u_m4;                // +0x44
    int      loc_u_m5;                // +0x48
    int      loc_u_m6;                // +0x4c
    int      loc_u_m7;                // +0x50
    int      loc_u_m8;                // +0x54

    GenericShader1();

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(::Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
