#ifndef GOF2_PULSESHADER_H
#define GOF2_PULSESHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// AbyssEngine::PulseShader -- GLES2 pulse shader (derives from ShaderBaseStruct).
// Layout (confirmed from Ghidra Init/ctor):
//   +0x00 vtable   +0x04 GL program   +0x09 byte "dirty" flag
//   +0x0c String name (12 bytes)
//   +0x20..0x30 attribute locations a0..a4
//   +0x34..0x58 uniform locations u0..u9 (note 0x58 carries u4)

#include "gof2/Mesh.h"   // full Mesh layout (top-level ::Mesh)

struct Engine;            // top-level, matches fwd.h (layout not modelled in this batch)

namespace AbyssEngine {

// AbyssEngine::PulseShader.
struct PulseShader {
    void *field_0x0;                    // +0x0  vtable
    int field_0x4;                      // +0x4  GL program handle
    uint8_t pad_0x8;                    // +0x8  (padding/unused)
    uint8_t field_0x9;                  // +0x9  "needs uniform update" flag
    uint8_t pad_0xa[2];                 // +0xa
    String field_0xc;                   // +0xc  shader name (12 bytes -> ends 0x18)
    uint8_t pad_0x18[8];                // +0x18
    int field_0x20;                     // +0x20 attrib a0
    int field_0x24;                     // +0x24 attrib a1
    int field_0x28;                     // +0x28 attrib a2
    int field_0x2c;                     // +0x2c attrib a3
    int field_0x30;                     // +0x30 attrib a4
    int field_0x34;                     // +0x34 uniform u0
    int field_0x38;                     // +0x38 uniform u1
    int field_0x3c;                     // +0x3c uniform u2
    int field_0x40;                     // +0x40 uniform u3
    int field_0x44;                     // +0x44 uniform u5
    int field_0x48;                     // +0x48 uniform u6
    int field_0x4c;                     // +0x4c uniform u7
    int field_0x50;                     // +0x50 uniform u8
    int field_0x54;                     // +0x54 uniform u9
    int field_0x58;                     // +0x58 uniform u4

    PulseShader();
    void Init(::Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, ::Engine *engine);
};

} // namespace AbyssEngine

// Byte-offset accessors into the render Engine object (its full layout is not modelled
// in this batch -- the matrices / light vectors it carries are read by fixed offset).

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

void operator_delete(void *ptr) noexcept;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;
}

#endif
