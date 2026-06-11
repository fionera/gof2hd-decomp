#ifndef GOF2_TEXTUREALPHATESTSHADER_H
#define GOF2_TEXTUREALPHATESTSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Engine/Mesh/ShaderBaseStruct/TextureAlphaTestShader are top-level classes (declared
// globally in fwd.h), so model them at global scope to stay consistent with the rest
// of the codebase. (placement operator new comes from <new> via common.h.)

void operator delete(void *ptr) noexcept;

using Vector = AbyssEngine::AEMath::Vector;

struct Engine;
struct Mesh;

struct ShaderBaseStruct {
    ShaderBaseStruct();
    ~ShaderBaseStruct();
};

class TextureAlphaTestShader : public ShaderBaseStruct  {
public:
    TextureAlphaTestShader();
    ~TextureAlphaTestShader();
    void Init(Engine *);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void ConnectShaderComponents(int program, int index);
    void UseShader(bool);
};




static inline String &string_at(void *self, unsigned int offset)
{
    return *(String *)((char *)self + offset);
}

extern "C" {
extern uint8_t g_TextureAlphaTestShader_useAlphaProgram;
extern uint8_t g_TextureAlphaTestShader_programIndex;
extern char g_TextureAlphaTestShader_vtable[];
extern int g_TextureAlphaTestShader_staticSource;
extern int g_TextureAlphaTestShader_staticDest;
extern uint32_t __stack_chk_guard;

int glGetUniformLocation(int program, const char *name);
int glGetAttribLocation(int program, const char *name);
void glUseProgram(int program);
void glDisableVertexAttribArray(int index);
void glEnableVertexAttribArray(int index);
void glUniform1f(int location, float value);
void glUniform1i(int location, int value);
void glUniform3f(int location, float x, float y, float z);
void glUniform3fv(int location, int count, const float *value);
void glUniform4fv(int location, int count, const float *value);
void glUniformMatrix4fv(int location, int count, uint8_t transpose, const float *value);
void glVertexAttribPointer(int index, int size, unsigned int type, uint8_t normalized,
                           int stride, const void *pointer);
void glBindBuffer(unsigned int target, unsigned int buffer);

float *Vector_cast_to_float(Vector *self);
int ShaderBaseStruct_ES2LoadProgram(ShaderBaseStruct *self, const char *vertexSource,
                                    const char *fragmentSource);
void *ShaderBaseStruct_dtor(ShaderBaseStruct *self) noexcept;
void operator_delete(void *ptr) noexcept;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;
}

#endif
