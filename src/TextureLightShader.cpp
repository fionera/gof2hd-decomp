#include "gof2/TextureLightShader.h"
#include "gof2/Mesh.h"




// ---- _TextureLightShader_8df1c.cpp ----
extern "C" void _ZN11AbyssEngine18TextureLightShaderD0Ev(TextureLightShader *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}

// ---- Init_8db68.cpp ----
namespace AbyssEngine {

void TextureLightShader::Init(Engine *)
{
    int program = ShaderBaseStruct_ES2LoadProgram(this, "TextureLightShader.vsh", "TextureLightShader.fsh");
    this->f_4 = program;

    int (*uniformFn)(int, const char *) = glGetUniformLocation;
    this->f_68 = uniformFn(program, "u0");
    this->f_6c = uniformFn(this->f_4, "u1");

    int (*attribFn)(int, const char *) = glGetAttribLocation;
    this->f_20 = attribFn(this->f_4, "a0");
    this->f_24 = attribFn(this->f_4, "a1");

    this->f_28 = uniformFn(this->f_4, "u2");
    this->f_2c = uniformFn(this->f_4, "u3");
    this->f_30 = uniformFn(this->f_4, "u4");
    this->f_34 = uniformFn(this->f_4, "u5");
    this->f_38 = uniformFn(this->f_4, "u6");
    this->f_3c = uniformFn(this->f_4, "u7");
    this->f_64 = uniformFn(this->f_4, "u8");
    this->f_44 = uniformFn(this->f_4, "u9");
    this->f_40 = uniformFn(this->f_4, "u10");
    this->f_48 = uniformFn(this->f_4, "u11");
    this->f_50 = uniformFn(this->f_4, "u12");
    this->f_58 = uniformFn(this->f_4, "u13");
    this->f_60 = uniformFn(this->f_4, "u14");
    this->f_4c = uniformFn(this->f_4, "u15");
    this->f_54 = uniformFn(this->f_4, "u16");
    this->f_5c = uniformFn(this->f_4, "u17");

    glUseProgram(this->f_4);
    return glUniform1i(this->f_44, 0);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8dd14.cpp ----
namespace AbyssEngine {

// Helpers to view the Engine/Mesh objects at raw byte offsets.
static inline const float *ef(Engine *e, unsigned off)
{
    return (const float *)((char *)e + off);
}
static inline int ei(Engine *e, unsigned off)
{
    return *(int *)((char *)e + off);
}
static inline float eff(Engine *e, unsigned off)
{
    return *(float *)((char *)e + off);
}

// TextureLightShader::UpdateMeshData(Mesh*, Engine*)
void TextureLightShader::UpdateMeshData(Mesh *meshIn, Engine *engine)
{
    Mesh *mesh = (Mesh *)meshIn;
    glUniformMatrix4fv(this->f_2c, 1, 0, ef(engine, 0x104));
    if (this->f_68 >= 0)
        glUniformMatrix4fv(this->f_68, 1, 0, ef(engine, 0x1c4));
    glUniformMatrix3fv(this->f_34, 1, 0, ef(engine, 0x204));
    if (this->f_30 >= 0)
        glUniformMatrix4fv(this->f_30, 1, 0, ef(engine, 0x144));

    if (this->f_9 != 0) {
        if (this->f_6c >= 0)
            glUniform1i(this->f_6c, mesh->field_0x85);
        glUniform4fv(this->f_40, 1, ef(engine, 0xd0));

        glUniform3fv(this->f_48, 1, ef(engine, 0x2cc));
        glUniform3fv(this->f_50, 1, ef(engine, 0x2fc));
        glUniform3fv(this->f_58, 1, ef(engine, 0x2e4));

        glUniform4f(this->f_38, eff(engine, 0x330), eff(engine, 0x334),
                    eff(engine, 0x338), eff(engine, 0x378));

        if (ei(engine, 0x32c) < 2) {
            glUniform3f(this->f_4c, 0, 0, 0);
            glUniform3f(this->f_54, 0, 0, 0);
            glUniform3f(this->f_5c, 0, 0, 0);
        } else {
            glUniform3fv(this->f_4c, 1, ef(engine, 0x2d8));
            glUniform3fv(this->f_54, 1, ef(engine, 0x308));
            glUniform3fv(this->f_5c, 1, ef(engine, 0x2f0));
        }

        glUniform4f(this->f_3c, eff(engine, 0x33c), eff(engine, 0x340),
                    eff(engine, 0x344), eff(engine, 0x37c));
        glUniform1f(this->f_60, eff(engine, 0x2c8));
        if (this->f_64 >= 0)
            glUniform3f(this->f_64, eff(engine, 0x34c), eff(engine, 0x350),
                        eff(engine, 0x354));
        this->f_9 = 0;
    }

    glEnableVertexAttribArray(this->f_20);
    glEnableVertexAttribArray(this->f_24);
    glEnableVertexAttribArray(this->f_28);

    if (mesh->field_0x5c != 0) {
        glBindBuffer(0x8892, mesh->field_0x60);
        glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x68);
        glVertexAttribPointer(this->f_24, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x6c);
        glVertexAttribPointer(this->f_28, 3, 0x1406, 0, 0, 0);
        return;
    }

    glVertexAttribPointer(this->f_20, 3, 0x1406, 0, 0,
                          mesh->field_0x4);
    glVertexAttribPointer(this->f_24, 2, 0x1406, 0, 0,
                          mesh->field_0x8);
    glVertexAttribPointer(this->f_28, 3, 0x1406, 0, 0,
                          mesh->field_0x10);
}

} // namespace AbyssEngine

// ---- SetInActive_8dcf0.cpp ----
namespace AbyssEngine {

void TextureLightShader::SetInActive()
{
    glDisableVertexAttribArray(this->f_20);
    glDisableVertexAttribArray(this->f_24);
    return glDisableVertexAttribArray(this->f_28);
}

} // namespace AbyssEngine

// ---- TextureLightShader_8dadc.cpp ----
// Hidden PC-relative globals recovered from the constructor disasm:
//   g_ctorCounter : *(int**) at 0x9dae8 — read its [0] into local_14 for stack guard.
//   g_shaderSrc   : *(int**) at 0x9dafa — value copied into *g_shaderDst.
//   g_shaderDst   : *(int**) at 0x9db02 — destination of the copied source pointer.
//   g_vtableBase  : *(int*)  at 0x9dafe — vtable address; this->vtable = base + 8.
//   g_nameLiteral : char* at 0x9db12 — the "TextureLightShader" name string literal.
__attribute__((visibility("hidden"))) extern int *g_tls_ctorCounter;
__attribute__((visibility("hidden"))) extern int **g_tls_shaderSrc;
__attribute__((visibility("hidden"))) extern int **g_tls_shaderDst;
__attribute__((visibility("hidden"))) extern int g_tls_vtableBase;
__attribute__((visibility("hidden"))) extern const char g_tls_nameLiteral[];

namespace AbyssEngine {

// TextureLightShader::TextureLightShader()
TextureLightShader::TextureLightShader()
{
    int *counter = g_tls_ctorCounter;
    int guard = *counter;

    ShaderBaseStruct_ctor(this);

    // Install vtable (base + 8 to skip RTTI/offset slots).
    this->f_0 = g_tls_vtableBase + 8;

    // Copy the shader-source pointer into its destination slot.
    *g_tls_shaderDst = *g_tls_shaderSrc;

    // Initialize the shader-name String member at this+0xc.
    String tmp;
    String_ctor_char(&tmp, g_tls_nameLiteral, false);
    String *dst = (String *)((char *)this + 0xc);
    String_assign(dst, &tmp);
    String_dtor(&tmp);

    if (*counter - guard != 0)
        __stack_chk_fail(*counter - guard);
}

} // namespace AbyssEngine
