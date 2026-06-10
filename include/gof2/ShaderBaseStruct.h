#ifndef GOF2_SHADERBASESTRUCT_H
#define GOF2_SHADERBASESTRUCT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)

extern "C" uint32_t __stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" void *malloc(__SIZE_TYPE__ size);
extern "C" void free(void *ptr);
extern "C" void *ShaderBaseStruct_vtable[];
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

// AbyssEngine::String::GetAEChar() const -- freshly allocated UTF-8 C-string.
extern "C" void *String_GetAEChar(const AbyssEngine::String *self);

namespace AbyssEngine {

struct AEFile {
    static uint32_t OpenRead(const char *path, uint32_t *handle);
    static uint32_t GetFileSize(uint32_t handle);
    static void Read(uint32_t size, void *data, uint32_t handle);
    static void Close(uint32_t handle);
};

struct ShaderBaseStruct {
    void *field_0x0;                    // +0x0  vtable
    int field_0x4;                      // +0x4  GL program handle (-1 when unset)
    volatile uint16_t field_0x8;        // +0x8  flags
    uint8_t field_0x9;                  // +0x9  dirty flag
    String field_0xc;                   // +0xc  shader name
    const char *field_0x18;             // +0x18 vertex path
    const char *field_0x1c;             // +0x1c fragment path

    String GetShaderName();
    virtual ~ShaderBaseStruct();
    void UseShader();
    void DeleteShader();
    void Update();
    uint32_t ES2LoadProgram(const char *vertexSource, const char *fragmentSource);
    uint32_t ES2LoadShader(uint32_t type, const char *source);
    uint32_t LoadBindShader(const char *vertexPath, const char *fragmentPath);
};

// field accessors (named-field based, no byte-offset arithmetic)
static inline void *&shader_vtable(ShaderBaseStruct *self)           { return self->field_0x0; }
static inline int &shader_program(ShaderBaseStruct *self)            { return self->field_0x4; }
static inline volatile uint16_t &shader_flags(ShaderBaseStruct *self){ return self->field_0x8; }
static inline uint8_t &shader_dirty(ShaderBaseStruct *self)          { return self->field_0x9; }
static inline String *shader_name(ShaderBaseStruct *self)            { return &self->field_0xc; }
static inline const String *shader_name(const ShaderBaseStruct *self){ return &self->field_0xc; }
static inline const char *&shader_vertex_path(ShaderBaseStruct *self){ return self->field_0x18; }
static inline const char *&shader_fragment_path(ShaderBaseStruct *self){ return self->field_0x1c; }

} // namespace AbyssEngine

#endif
