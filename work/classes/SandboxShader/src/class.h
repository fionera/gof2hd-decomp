#ifndef WORK_CLASSES_SANDBOXSHADER_SRC_CLASS_H
#define WORK_CLASSES_SANDBOXSHADER_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;
inline void *operator new(uint32_t, void *ptr) noexcept { return ptr; }

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" char _ZTVN11AbyssEngine13SandboxShaderE[];

extern "C" void glUseProgram(uint32_t program);
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

struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    String() noexcept;
    String(const char *text, bool copy) noexcept;
    ~String() noexcept;

    String &operator=(const String &other) noexcept;
};

struct ShaderBaseStruct {
    static int shaderIndexIntern;

    ShaderBaseStruct();
    ~ShaderBaseStruct();

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
};

struct SandboxShader {
    static int ShaderIndex;

    SandboxShader();
    ~SandboxShader();

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

static inline int &field_i32(void *self, uint32_t offset)
{
    return *(int *)((char *)self + offset);
}

static inline uint8_t &field_u8(void *self, uint32_t offset)
{
    return *(uint8_t *)((char *)self + offset);
}

static inline float &field_f32(void *self, uint32_t offset)
{
    return *(float *)((char *)self + offset);
}

static inline void *field_ptr(void *self, uint32_t offset)
{
    return *(void **)((char *)self + offset);
}

} // namespace AbyssEngine

// Backing C symbols used by the ctor.
extern "C" {
extern char _ZTVN11AbyssEngine13SandboxShaderE[];

// ShaderBaseStruct ctor.
void *_ZN11AbyssEngine16ShaderBaseStructC2Ev(AbyssEngine::ShaderBaseStruct *self);
// String(char const*, bool).
void _ZN11AbyssEngine6StringC2EPKcb(AbyssEngine::String *self, const char *text, bool copy);
// String::operator=(String const&).
AbyssEngine::String *_ZN11AbyssEngine6StringaSERKS0_(
    AbyssEngine::String *self, const AbyssEngine::String *other);
// ~String().
void _ZN11AbyssEngine6StringD2Ev(AbyssEngine::String *self);

// Shader-registry globals patched by the ctor (one global's value copied into another).
extern int SandboxShader_registerSrc;
extern int *SandboxShader_registerDst;
}

namespace AbyssEngine {
inline String::String(const char *text, bool copy) noexcept
{
    _ZN11AbyssEngine6StringC2EPKcb(this, text, copy);
}
inline String::~String() noexcept { _ZN11AbyssEngine6StringD2Ev(this); }
inline String &String::operator=(const String &other) noexcept
{
    return *_ZN11AbyssEngine6StringaSERKS0_(this, &other);
}
} // namespace AbyssEngine

#endif
