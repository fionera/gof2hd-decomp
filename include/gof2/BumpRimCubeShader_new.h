#ifndef GOF2_BUMPRIMCUBESHADER_NEW_H
#define GOF2_BUMPRIMCUBESHADER_NEW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

extern "C" char _ZTVN11AbyssEngine21BumpRimCubeShader_newE[];

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
    int LoadBindShader(const char *vertexPath, const char *fragmentPath);
};







// BumpRimCubeShader_new — derives from ShaderBaseStruct (vtable at +0x0).
// Int fields are GL uniform/attribute locations (contiguous block 0x20..0x94).
class BumpRimCubeShader_new {
public:
    void *field_0x0;            // +0x0  vtable ptr
    int program;              // +0x4  program
    uint8_t pad_0x8;            // +0x8
    uint8_t needsUpdate;          // +0x9  needsUpdate flag
    uint16_t pad_0xa;
    String name;           // +0xc  shader name
    int pad_0x1c;
    int attrib0;  int attrib1;  int attrib2;  int attrib3;  int attrib4; // attribs a0..a4
    int uniform0;  int uniform1;  int uniform2;  int uniform3;  int uniform4; // u0..u4
    int uniform5;  int uniform6;  int uniform7;  int uniform8;  int uniform9; // u5..u9
    int uniform10;  int uniform11;  int uniform12;  int uniform13;  int uniform14; // u10..u14
    int uniform15;  int uniform16;  int uniform17;  int uniform18;  int uniform19; // u15..u19
    int uniform20;  int uniform21;  int uniform22;  int uniform23;  int uniform24; // u20..u24

    static int ShaderIndex;

    BumpRimCubeShader_new();
    void Init(Engine *);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine
#endif
