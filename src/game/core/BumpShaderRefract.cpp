#include "gof2/BumpShaderRefract.h"
#include "gof2/Engine.h"
namespace AbyssEngine {

int BumpShaderRefract::ShaderIndex;

// AbyssEngine::BumpShaderRefract::SetInActive()
void BumpShaderRefract::SetInActive()
{
    if (field_0x20 >= 0)
        glDisableVertexAttribArray(field_0x20);
    if (field_0x24 >= 0)
        glDisableVertexAttribArray(field_0x24);
    if (field_0x28 >= 0)
        glDisableVertexAttribArray(field_0x28);
}

// AbyssEngine::BumpShaderRefract::BumpShaderRefract()
__attribute__((minsize)) BumpShaderRefract::BumpShaderRefract()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    field_0x0 = (char *)_ZTVN11AbyssEngine17BumpShaderRefractE + 8;
    ShaderIndex = ShaderBaseStruct::shaderIndexIntern;
    field_0xc.s = u"BumpShaderRefract";
    
}

// AbyssEngine::BumpShaderRefract::Init(AbyssEngine::Engine*)
void BumpShaderRefract::Init(Engine *engine)
{
    int program = ES2LoadProgram("BumpShaderRefract.vsh", "BumpShaderRefract.fsh");
    field_0x4 = program;

    field_0x20 = glGetAttribLocation(program, "a0");
    field_0x24 = glGetAttribLocation(field_0x4, "a1");

    field_0x28 = glGetUniformLocation(field_0x4, "u0");
    field_0x2c = glGetUniformLocation(field_0x4, "u1");
    field_0x30 = glGetUniformLocation(field_0x4, "u2");
    field_0x34 = glGetUniformLocation(field_0x4, "u3");
    field_0x38 = glGetUniformLocation(field_0x4, "u4");
    field_0x40 = glGetUniformLocation(field_0x4, "u5");
    field_0x44 = glGetUniformLocation(field_0x4, "u6");
    field_0x48 = glGetUniformLocation(field_0x4, "u7");

    glActiveTexture(0x84c7);
    ((::Engine *)engine)->ActivateRefractFBO();
    field_0x3c = glGetUniformLocation(field_0x4, "u8");

    glUseProgram(field_0x4);
    // samplers u2 (+0x30) and u3 (+0x34): consecutive int fields, indexed at runtime.
    int *samplers = &field_0x30;
    for (int i = 0; i != 2; i++) {
        int loc = samplers[i];
        if (loc >= 0)
            glUniform1i(loc, i);
    }
    glUniform1i(field_0x3c, 7);
}

// AbyssEngine::BumpShaderRefract::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*)
void BumpShaderRefract::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *e = (char *)engine;
    char *m = (char *)mesh;

    if (field_0x2c >= 0)
        glUniformMatrix4fv(field_0x2c, 1, 0, e + 0x104);
    if (field_0x40 >= 0)
        glUniformMatrix4fv(field_0x40, 1, 0, e + 0x1c4);

    if (field_0x9 != 0) {
        if (field_0x44 >= 0)
            glUniform1i(field_0x44, *(uint8_t *)(m + 0x85));
        glUniform4fv(field_0x48, 1, (float *)(e + 0xd0));
        int loc = field_0x38;
        if (loc >= 0) {
            float w = (float)((::Engine *)engine)->GetDisplayWidth();
            float h = (float)((::Engine *)engine)->GetDisplayHeight();
            glUniform2f(loc, 1.0f / w, 1.0f / h);
        }
        glActiveTexture(0x84c7);
        ((::Engine *)engine)->ActivateRefractFBO();
        field_0x9 = 0;
    }

    if (field_0x20 >= 0)
        glEnableVertexAttribArray(field_0x20);
    if (field_0x24 >= 0)
        glEnableVertexAttribArray(field_0x24);
    if (field_0x28 >= 0)
        glEnableVertexAttribArray(field_0x28);

    int loc0 = field_0x20;
    const void *ptr;
    int last;
    if (*(uint8_t *)(m + 0x5c) == 0) {
        if (loc0 >= 0)
            glVertexAttribPointer(loc0, 3, 0x1406, 0, 0, *(void **)(m + 0x4));
        if (field_0x24 >= 0)
            glVertexAttribPointer(field_0x24, 2, 0x1406, 0, 0, *(void **)(m + 0x8));
        last = field_0x28;
        if (last < 0)
            return;
        ptr = *(void **)(m + 0xc);
    } else {
        if (loc0 >= 0) {
            glBindBuffer(0x8892, *(uint32_t *)(m + 0x60));
            glVertexAttribPointer(field_0x20, 3, 0x1406, 0, 0, 0);
        }
        if (field_0x24 >= 0) {
            glBindBuffer(0x8892, *(uint32_t *)(m + 0x68));
            glVertexAttribPointer(field_0x24, 2, 0x1406, 0, 0, 0);
        }
        if (field_0x28 < 0)
            return;
        glBindBuffer(0x8892, *(uint32_t *)(m + 0x78));
        ptr = 0;
        last = field_0x28;
    }
    glVertexAttribPointer(last, 4, 0x1406, 0, 0, ptr);
}

} // namespace AbyssEngine

// ---- _BumpShaderRefract_8fac2.cpp ----
// AbyssEngine::BumpShaderRefract::~BumpShaderRefract() (deleting dtor)
void _ZN11AbyssEngine17BumpShaderRefractD0Ev(AbyssEngine::BumpShaderRefract *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    ::operator delete(base);
}
