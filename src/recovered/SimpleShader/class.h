#ifndef WORK_CLASSES_SIMPLESHADER_SRC_CLASS_H
#define WORK_CLASSES_SIMPLESHADER_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {

struct Engine;
struct Mesh;
struct ShaderBaseStruct;

struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    String(const char *text, bool copy) noexcept;
    ~String() noexcept;
    String &operator=(const String &other) noexcept;
};

struct SimpleShader {
    SimpleShader();
    ~SimpleShader();

    void SetInActive();
    void Init(Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

using AbyssEngine::Engine;
using AbyssEngine::Mesh;
using AbyssEngine::ShaderBaseStruct;
using AbyssEngine::SimpleShader;
using AbyssEngine::String;

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;

extern "C" void ShaderBaseStruct_ctor(ShaderBaseStruct *self);
extern "C" void *ShaderBaseStruct_dtor(ShaderBaseStruct *self);
extern "C" uint32_t ShaderBaseStruct_ES2LoadProgram(ShaderBaseStruct *self, const char *vertex, const char *fragment);
extern "C" void String_ctor_char(String *self, const char *text, bool copy);
extern "C" void String_assign(String *self, const String *other);
extern "C" void String_dtor(String *self);

extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" void glUseProgram(uint32_t program);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniform4fv(int location, int count, const float *value);
extern "C" void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glEnableVertexAttribArray(uint32_t index);
extern "C" void glDisableVertexAttribArray(uint32_t index);
extern "C" void glVertexAttribPointer(uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);
extern "C" void glBindBuffer(uint32_t target, uint32_t buffer);

extern "C" void *SimpleShader_vtable[];
extern "C" int32_t ShaderBaseStruct_shaderIndexIntern;
extern "C" int32_t SimpleShader_ShaderIndex;

static inline char *bytes(void *p) { return (char *)p; }
static inline uint8_t &field_u8(void *p, uint32_t off) { return *(uint8_t *)(bytes(p) + off); }
static inline int32_t &field_i32(void *p, uint32_t off) { return *(int32_t *)(bytes(p) + off); }
static inline uint32_t &field_u32(void *p, uint32_t off) { return *(uint32_t *)(bytes(p) + off); }
static inline void *&field_ptr(void *p, uint32_t off) { return *(void **)(bytes(p) + off); }

#endif
