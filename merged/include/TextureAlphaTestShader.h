#ifndef WORK_CLASSES_TEXTUREALPHATESTSHADER_SRC_CLASS_H
#define WORK_CLASSES_TEXTUREALPHATESTSHADER_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept
{
    return ptr;
}

void operator delete(void *ptr) noexcept;

namespace AbyssEngine {

struct Engine;
struct Mesh;

struct ShaderBaseStruct {
    ShaderBaseStruct();
    ~ShaderBaseStruct();
};

struct String {
    uint32_t text;
    uint32_t field_04;
    uint32_t size;

    String();
    String(const char *text, bool copy);
    ~String();
    String &operator=(const String &other);
};

namespace AEMath {
struct Vector;
}

struct TextureAlphaTestShader {
    TextureAlphaTestShader();
    ~TextureAlphaTestShader();

    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void Init(Engine *engine);
    void SetInActive();
    void UseShader(bool active);
    void ConnectShaderComponents(int program, int index);
};

} // namespace AbyssEngine

using Engine = AbyssEngine::Engine;
using Mesh = AbyssEngine::Mesh;
using ShaderBaseStruct = AbyssEngine::ShaderBaseStruct;
using String = AbyssEngine::String;
using TextureAlphaTestShader = AbyssEngine::TextureAlphaTestShader;
using Vector = AbyssEngine::AEMath::Vector;

static inline int &i32(void *self, unsigned int offset)
{
    return *(int *)((char *)self + offset);
}

static inline uint8_t &u8(void *self, unsigned int offset)
{
    return *(uint8_t *)((char *)self + offset);
}

static inline float &f32(void *self, unsigned int offset)
{
    return *(float *)((char *)self + offset);
}

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
