#ifndef GOF2_SHADERBASESTRUCT_H
#define GOF2_SHADERBASESTRUCT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
void *operator new(__SIZE_TYPE__ size);
void *operator new[](__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }

extern "C" uint32_t __stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" void *malloc(uint32_t size);
extern "C" void free(void *ptr);
extern "C" uint32_t ShaderBaseStruct_vtable[];
extern "C" uint32_t ShaderBaseStruct_count;

extern "C" uint32_t glCreateShader(uint32_t type);
extern "C" void glShaderSource(uint32_t shader, uint32_t count, const char **strings, const int *lengths);
extern "C" void glCompileShader(uint32_t shader);
extern "C" void glGetShaderiv(uint32_t shader, uint32_t pname, int *params);
extern "C" void glGetShaderInfoLog(uint32_t shader, uint32_t maxLength, int *length, void *infoLog);
extern "C" void glDeleteShader(uint32_t shader);
extern "C" void glUseProgram(uint32_t program);
extern "C" void glDeleteProgram(uint32_t program);
extern "C" uint32_t glCreateProgram();
extern "C" void glAttachShader(uint32_t program, uint32_t shader);
extern "C" void glLinkProgram(uint32_t program);
extern "C" void glGetProgramiv(uint32_t program, uint32_t pname, int *params);
extern "C" void glGetProgramInfoLog(uint32_t program, uint32_t maxLength, int *length, void *infoLog);
extern "C" void AELabelObject(uint32_t type, uint32_t object, const char *name);
extern "C" uint32_t ShaderBaseStruct_ES2LoadShader(uint32_t receiver, uint32_t type, const char *source);

namespace AbyssEngine {



struct AEFile {
    static uint32_t OpenRead(const char *path, uint32_t *handle);
    static uint32_t GetFileSize(uint32_t handle);
    static void Read(uint32_t size, void *data, uint32_t handle);
    static void Close(uint32_t handle);
};



static inline uint32_t &shader_vtable(ShaderBaseStruct *self)
{
    return *(uint32_t *)((char *)self + 0x0);
}

static inline uint32_t &shader_program(ShaderBaseStruct *self)
{
    return *(uint32_t *)((char *)self + 0x4);
}

static inline uint16_t &shader_flags(ShaderBaseStruct *self)
{
    return *(uint16_t *)((char *)self + 0x8);
}

static inline uint8_t &shader_dirty(ShaderBaseStruct *self)
{
    return *(uint8_t *)((char *)self + 0x9);
}

static inline String *shader_name(ShaderBaseStruct *self)
{
    return (String *)((char *)self + 0xc);
}

static inline const String *shader_name(const ShaderBaseStruct *self)
{
    return (const String *)((const char *)self + 0xc);
}

static inline const char *&shader_vertex_path(ShaderBaseStruct *self)
{
    return *(const char **)((char *)self + 0x18);
}

static inline const char *&shader_fragment_path(ShaderBaseStruct *self)
{
    return *(const char **)((char *)self + 0x1c);
}

static inline uint64_t &shader_paths(ShaderBaseStruct *self)
{
    return *(uint64_t *)((char *)self + 0x18);
}

} // namespace AbyssEngine

struct ShaderBaseStruct {
    int field_0x4;                      // +0x4
    volatile uint16_t field_0x8;        // +0x8
};
#endif
