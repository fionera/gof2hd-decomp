#ifndef GOF2_TEXTURELIGHTSHADER_H
#define GOF2_TEXTURELIGHTSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field naming)
#include <new>

namespace AbyssEngine {

struct Engine;
struct Mesh;

// AbyssEngine::TextureLightShader — GLES2 lit-texture shader (derives from ShaderBaseStruct).
// Layout mirrors the recovered field offsets: vtable slot at 0x0, program handle at 0x4,
// per-frame dirty byte at 0x9, name String at 0xc, attribute/uniform locations 0x20..0x6c.
class TextureLightShader {
public:
    int   f_0;                 // 0x00 vtable slot
    int   f_4;                 // 0x04 shader program handle
    uint8_t f_8;               // 0x08
    uint8_t f_9;               // 0x09 per-frame dirty flag
    uint8_t f_a;               // 0x0a
    uint8_t f_b;               // 0x0b
    char  name_storage[0x14];  // 0x0c name String (12 bytes) + padding to 0x20
    int   f_20;                // 0x20 attribute/uniform locations follow
    int   f_24;
    int   f_28;
    int   f_2c;
    int   f_30;
    int   f_34;
    int   f_38;
    int   f_3c;
    int   f_40;
    int   f_44;
    int   f_48;
    int   f_4c;
    int   f_50;
    int   f_54;
    int   f_58;
    int   f_5c;
    int   f_60;
    int   f_64;
    int   f_68;
    int   f_6c;

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    TextureLightShader();
};

} // namespace AbyssEngine

extern "C" {
extern char TextureLightShader_vtable[];
extern int TextureLightShader_typeInfoSource;
extern int TextureLightShader_typeInfoDest;
extern const char TextureLightShader_name[];

int glGetAttribLocation(int program, const char *name);
int glGetUniformLocation(int program, const char *name);
extern "C" void glUseProgram(uint32_t program);
void glUniform1i(int location, int value);
void glUniform1f(int location, float value);
void glUniform3f(int location, float x, float y, float z);
void glUniform4f(int location, float x, float y, float z, float w);
void glUniform3fv(int location, int count, const float *value);
void glUniform4fv(int location, int count, const float *value);
void glUniformMatrix3fv(int location, int count, uint8_t transpose, const float *value);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const float *value);
void glEnableVertexAttribArray(int index);
void glDisableVertexAttribArray(int index);
void glVertexAttribPointer(int index, int size, unsigned int type, uint8_t normalized,
                           int stride, const void *pointer);
void glBindBuffer(unsigned int target, unsigned int buffer);

void ShaderBaseStruct_ctor(void *self);
void *ShaderBaseStruct_dtor(void *self) noexcept;
int ShaderBaseStruct_ES2LoadProgram(void *self, const char *vertexSource, const char *fragmentSource);

float *Vector_cast_to_float(Vector *self);

void operator_delete(void *ptr) noexcept;
}

#endif
