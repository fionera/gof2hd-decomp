#ifndef GOF2_BUMPSHADER_H
#define GOF2_BUMPSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

namespace AbyssEngine {

struct Engine;
struct Mesh;

// Per-class backing storage accessor helpers. The BumpShader fields live at fixed byte
// offsets (program handle at 0x4, dirty flag byte at 0x9, name String at 0xc, attribute/
// uniform locations 0x20..0x80) and are read/written through these helpers.



// AbyssEngine::BumpShader — GLES2 bump-mapping shader (derives from ShaderBaseStruct).
struct BumpShader {
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    BumpShader();

    // raw field storage (offsets referenced through the i32/u8/f32 helpers above)
    char field_storage[0x84];
};

} // namespace AbyssEngine

extern "C" {
extern char BumpShader_vtable[];
extern int BumpShader_typeInfoSource;
extern int BumpShader_typeInfoDest;
extern const char BumpShader_name[];

int glGetAttribLocation(int program, const char *name);
int glGetUniformLocation(int program, const char *name);
void glUseProgram(int program);
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

void String_ctor_char(String *self, const char *text, bool copy);
String *String_assign(String *self, const String *other);
void String_dtor(String *self);

float *Vector_cast_to_float(Vector *self);

void operator_delete(void *ptr) noexcept;
}

#endif
