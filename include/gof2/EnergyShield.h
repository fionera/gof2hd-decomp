#ifndef GOF2_ENERGYSHIELD_H
#define GOF2_ENERGYSHIELD_H
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







// EnergyShield — refraction shield shader. Derives from ShaderBaseStruct
// (vtable ptr at +0x0). Int fields are GL uniform/attribute locations.
class EnergyShield {
public:
    void *vtable;            // +0x0  vtable ptr
    int program;              // +0x4  program
    uint8_t pad_0x8;            // +0x8
    uint8_t needsUniformUpdate;          // +0x9  needsRefractInit flag
    uint16_t pad_0xa;
    String shaderName;           // +0xc  shader name ("EnergyShield")
    // GL locations (block accessed dynamically as &uTex0 + i*4 for tex units)
    int aPosition;             // +0x20 a_position
    int aTexCoord;             // +0x24 a_texCoord
    int uM0;             // +0x28 u_m0
    int uM1;             // +0x2c u_m1
    int uM2;             // +0x30 u_m2
    int uM3;             // +0x34 u_m3
    int uTex0;             // +0x38 u_tex0
    int uTex1;             // +0x3c u_tex1
    int pad_0x40;
    int uM4;             // +0x44 u_m4
    int uM5;             // +0x48 u_m5
    int pad_0x4c;
    int uM7;             // +0x50 u_m7
    int uRefract;             // +0x54 u_refract
    int uM6;             // +0x58 u_m6
    int uM8;             // +0x5c u_m8

    static int ShaderIndex;

    void Init(Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine

extern "C" char _ZTVN11AbyssEngine12EnergyShieldE[];
#endif
