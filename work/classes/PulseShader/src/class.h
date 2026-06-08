#ifndef WORK_CLASSES_PulseShader_SRC_CLASS_H
#define WORK_CLASSES_PulseShader_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace AbyssEngine {

struct Engine;
struct Mesh;

struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    String(const char *text, bool copy);
    ~String();
    void operator=(const String &other);
};

struct PulseShader {
    // @portable-fields
    unsigned char _pad_0[4];
    int f_4; // 0x4
    unsigned char _pad_8[1];
    uint8_t f_9; // 0x9
    unsigned char _pad_a[22];
    int f_20; // 0x20
    int f_24; // 0x24
    int f_28; // 0x28
    int f_2c; // 0x2c
    int f_30; // 0x30
    int f_34; // 0x34
    int f_38; // 0x38
    int f_3c; // 0x3c
    int f_40; // 0x40
    int f_44; // 0x44
    int f_48; // 0x48
    int f_4c; // 0x4c
    int f_50; // 0x50
    int f_54; // 0x54
    int f_58; // 0x58
    uint8_t f_5c; // 0x5c
    unsigned char _pad_5d[3];
    int f_60; // 0x60
    unsigned char _pad_64[4];
    int f_68; // 0x68
    int f_6c; // 0x6c
    int f_70; // 0x70
    int f_74; // 0x74
    unsigned char _pad_78[696];
    int f_330; // 0x330
    int f_334; // 0x334
    int f_338; // 0x338
    unsigned char _pad_33c[16];
    int f_34c; // 0x34c
    int f_350; // 0x350
    int f_354; // 0x354

    PulseShader();
    ~PulseShader();

    void SetInActive();
    void Init(Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

namespace AEMath {
struct Vector;
}

} // namespace AbyssEngine

using Engine = AbyssEngine::Engine;
using Mesh = AbyssEngine::Mesh;
using String = AbyssEngine::String;
using PulseShader = AbyssEngine::PulseShader;
using Vector = AbyssEngine::AEMath::Vector;

static_assert(sizeof(String) == 0xc, "String layout");

static inline int &i32(void *self, uint32_t offset)
{
    return *(int *)((char *)self + offset);
}

static inline uint8_t &u8(void *self, uint32_t offset)
{
    return *(uint8_t *)((char *)self + offset);
}

static inline float &f32(void *self, uint32_t offset)
{
    return *(float *)((char *)self + offset);
}

extern "C" {
extern void *__stack_chk_guard;
extern char PulseShader_vtable[];
extern int PulseShader_typeInfoSource;
extern int PulseShader_typeInfoDest;
extern const char PulseShader_name[];

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
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;
}

namespace AbyssEngine {
inline String::String(const char *text, bool copy) { String_ctor_char(this, text, copy); }
inline String::~String() { String_dtor(this); }
inline void String::operator=(const String &other) { String_assign(this, &other); }
} // namespace AbyssEngine

#endif
