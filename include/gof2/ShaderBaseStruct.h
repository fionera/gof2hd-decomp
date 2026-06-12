#ifndef GOF2_SHADERBASESTRUCT_H
#define GOF2_SHADERBASESTRUCT_H
#include "gof2/common.h"
#include "gof2/String.h"

// AbyssEngine::ShaderBaseStruct — the GLSL shader base that every concrete shader
// (BloomShader, BumpShader*, GenericShader*, TextureShader, ...) derives from. It owns the
// GL program handle and drives ES2 shader compile/link plus uniform/attrib location binding.
//
// Each concrete shader is compiled in its own translation unit that declares its OWN local
// copy of this base layout (using the field_0xNN convention). To stay link-compatible with
// those TUs the layout here MUST remain byte-for-byte identical: vtable(+0x0), program(+0x4),
// flags(+0x8), dirty(+0x9), name(+0xc), vertexPath(+0x18), fragmentPath(+0x1c).

extern "C" void *malloc(__SIZE_TYPE__ size);
extern "C" void free(void *ptr);

// AbyssEngine::ShaderBaseStruct vtable (the real C++ vtable symbol).
extern "C" void *_ZTVN11AbyssEngine16ShaderBaseStructE[];

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

namespace AbyssEngine {

struct AEFile {
    static uint32_t OpenRead(const char *path, uint32_t *handle);
    static uint32_t GetFileSize(uint32_t handle);
    static void Read(uint32_t size, void *data, uint32_t handle);
    static void Close(uint32_t handle);
};

class ShaderBaseStruct {
public:
    void *vtable;                   // +0x0  vtable
    int program;                    // +0x4  GL program handle (-1 when unset)
    volatile uint16_t flags;        // +0x8  flags
    uint8_t dirty;                  // +0x9  dirty flag
    String name;                    // +0xc  shader name
    const char *vertexPath;         // +0x18 vertex shader path
    const char *fragmentPath;       // +0x1c fragment shader path

    // Running count of all shaders ever constructed; concrete shaders snapshot it in their
    // ctor as their per-shader index. Module-static engine counter (`shaderIndexIntern`).
    static int shaderIndexIntern;

    ShaderBaseStruct();
    virtual ~ShaderBaseStruct();

    String GetShaderName();
    void UseShader();
    void DeleteShader();
    void Update();
    uint32_t ES2LoadProgram(const char *vertexSource, const char *fragmentSource);
    uint32_t ES2LoadShader(uint32_t type, const char *source);
    uint32_t LoadBindShader(const char *vertexPath, const char *fragmentPath);
};

// field accessors (named-field based, no byte-offset arithmetic)
static inline void *&shader_vtable(ShaderBaseStruct *self)           { return self->vtable; }
static inline int &shader_program(ShaderBaseStruct *self)            { return self->program; }
static inline volatile uint16_t &shader_flags(ShaderBaseStruct *self){ return self->flags; }
static inline uint8_t &shader_dirty(ShaderBaseStruct *self)          { return self->dirty; }
static inline String *shader_name(ShaderBaseStruct *self)            { return &self->name; }
static inline const String *shader_name(const ShaderBaseStruct *self){ return &self->name; }
static inline const char *&shader_vertex_path(ShaderBaseStruct *self){ return self->vertexPath; }
static inline const char *&shader_fragment_path(ShaderBaseStruct *self){ return self->fragmentPath; }

} // namespace AbyssEngine

#endif
