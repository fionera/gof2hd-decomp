#ifndef GOF2_BUMPSHADERV2_H
#define GOF2_BUMPSHADERV2_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// AbyssEngine::BumpShaderV2 — GLES2 bump-mapping shader program. Derives from
// ShaderBaseStruct; caches attribute/uniform locations after Init and uploads the
// per-mesh / per-frame matrices and lighting block in UpdateMeshData.

namespace AbyssEngine {

struct Engine;
struct Mesh;

namespace AEMath {
struct Vector;
}

} // namespace AbyssEngine

// Engine, Mesh are forward-declared globally in fwd.h; Vector is aliased in common.h.

extern "C" {
extern void *__stack_chk_guard;
extern char BumpShaderV2_vtable[];
extern int BumpShaderV2_typeInfoSource;
extern int BumpShaderV2_typeInfoDest;
extern const char BumpShaderV2_name[];

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

void String_ctor_char(AbyssEngine::String *self, const char *text, bool copy);
AbyssEngine::String *String_assign(AbyssEngine::String *self, const AbyssEngine::String *other);
void String_dtor(AbyssEngine::String *self);

float *Vector_cast_to_float(Vector *self);

void operator_delete(void *ptr) noexcept;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;
}

namespace AbyssEngine {

// Foreign-pointer byte accessors (Engine/Mesh layouts are owned by other classes).
static inline int &i32(void *self, uint32_t offset) { return *(int *)((char *)self + offset); }
static inline uint8_t &u8(void *self, uint32_t offset) { return *(uint8_t *)((char *)self + offset); }
static inline float &f32(void *self, uint32_t offset) { return *(float *)((char *)self + offset); }

// Layout (after the ShaderBaseStruct base; offsets are absolute from the object base):
//   0x00 vtable
//   0x04 int   GL program handle
//   0x09 u8    lighting-dirty flag
//   0x0c String name
//   0x20..0x30 int  attribute locations (position/normal/texcoord/tangent/binormal)
//   0x38..0x5c int  uniform locations (mvp/normal/lightDir/eyePos/tex0/tex1/ambient/diffuse/specular/emissive)
// Note: on the host build AbyssEngine::String is std::u16string-backed (not the game's
// 12-byte layout), so the absolute byte offsets are not reproduced here. Fields are accessed
// by name; the only adjacency assumption (the texture-sampler pair) is handled with an array
// helper member access in the .cpp.
struct BumpShaderV2 {
    void*    field_0x0;                 // +0x0  vtable
    int      field_0x4;                 // +0x4  GL program handle
    uint8_t  field_0x9;                 // +0x9  lighting-dirty flag
    String   field_0xc;                 // +0xc  name
    int      field_0x20;                // +0x20 a_position
    int      field_0x24;                // +0x24 a_normal
    int      field_0x28;                // +0x28 a_texCoord
    int      field_0x2c;                // +0x2c a_tangent
    int      field_0x30;                // +0x30 a_binormal
    int      field_0x38;                // +0x38 u_mvpMatrix
    int      field_0x3c;                // +0x3c u_normalMatrix
    int      field_0x40;                // +0x40 u_lightDir
    int      field_0x44;                // +0x44 u_eyePos
    int      field_0x48;                // +0x48 u_texture0
    int      field_0x4c;                // +0x4c u_texture1
    int      field_0x50;                // +0x50 u_ambient
    int      field_0x54;                // +0x54 u_diffuse
    int      field_0x58;                // +0x58 u_specular
    int      field_0x5c;                // +0x5c u_emissive

    BumpShaderV2();
    void Init(Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine

#endif
