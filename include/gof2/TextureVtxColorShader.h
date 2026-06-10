#ifndef GOF2_TEXTUREVTXCOLORSHADER_H
#define GOF2_TEXTUREVTXCOLORSHADER_H
#include "gof2/common.h"
// AbyssEngine::TextureVtxColorShader (Android libgof2hdaa.so, armv7 Thumb).
// Real named struct recovered from the byte-offset field map.
//
// The shader keeps two GL programs: a normal program (index 0) and a fog
// program (index 1). Every uniform / attribute location is therefore stored
// as a two-element array indexed by the active program. In the original
// disassembly these pairs were reached via `(char*)this + index*4 + 0xNN`,
// i.e. an int[2] at an 8-byte stride starting at offset 0x24.

#include <new>

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" char _ZTVN11AbyssEngine21TextureVtxColorShaderE[];

extern "C" void glUseProgram(uint32_t program);
extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" void glUniform1f(int location, float value);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniform3f(int location, float x, float y, float z);
extern "C" void glUniform3fv(int location, int count, const float *value);
extern "C" void glUniform4fv(int location, int count, const float *value);
extern "C" void glUniformMatrix4fv(int location, int count, uint8_t transpose, const float *value);
extern "C" void glEnableVertexAttribArray(uint32_t index);
extern "C" void glDisableVertexAttribArray(uint32_t index);
extern "C" void glBindBuffer(uint32_t target, uint32_t buffer);
extern "C" void glVertexAttribPointer(
    uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);

namespace AbyssEngine {

struct Mesh;  // defined in gof2/Mesh.h within namespace AbyssEngine

struct Engine {
    static bool fogEnabled;
    // Engine fields referenced by the shader, accessed at byte offsets in the
    // original code. Only the offsets actually read here are named.
    char         field_0x00[0xd0];
    float        glColor[4];          // +0xd0
    char         field_0xe0[0x24];
    float        worldMatrix[16];     // +0x104
    char         field_0x144[0x80];
    float        uvMatrix[16];        // +0x1c4
    char         field_0x204[0x148];
    float        eyePosModelX;        // +0x34c
    float        eyePosModelY;        // +0x350
    float        eyePosModelZ;        // +0x354
    char         field_0x358[0x98];
    float        eyePosModel[3];      // +0x3f0
    char         field_0x3fc_pad[0x0];
};

struct ShaderBaseStruct {
    static int shaderIndexIntern;

    ShaderBaseStruct();
    ~ShaderBaseStruct();

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
};

struct TextureVtxColorShader {
    void    *vtable;                  // +0x00
    int      program;                 // +0x04
    uint8_t  field_0x08;              // +0x08
    uint8_t  needsUniformUpdate;      // +0x09
    uint16_t field_0x0a;              // +0x0a
    String   shaderName;              // +0x0c
    int      fogProgram;              // +0x20
    // Uniform / attribute locations, indexed by program (0 = normal, 1 = fog).
    int      loc_a_position[2];       // +0x24
    int      loc_a_texCoord[2];       // +0x2c
    int      loc_a_VertexColor[2];    // +0x34
    int      loc_u_WorldMatrix[2];    // +0x3c
    int      loc_glColor[2];          // +0x44
    int      loc_s_texture[2];        // +0x4c
    int      loc_u_UVAnimation[2];    // +0x54
    int      loc_u_UvMatrix[2];       // +0x5c
    int      loc_u_fogColor[2];       // +0x64
    int      loc_u_fogMaxDist[2];     // +0x6c
    int      loc_u_fogMinDist[2];     // +0x74
    int      loc_u_EnableFog[2];      // +0x7c
    int      loc_u_eyeposmodel[2];    // +0x84
    int      loc_u_DarkenValue[2];    // +0x8c

    static int ShaderIndex;

    void Init(Engine *engine);
    void SetInActive();
    void ConnectShaderComponents(uint32_t program, int index);
    void UseShader(bool);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
