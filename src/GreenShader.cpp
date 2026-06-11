#include "gof2/GreenShader.h"
#include "gof2/ShaderBaseStruct.h"

extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringC1EPKcb(
    AbyssEngine::String *self, const char *text, bool copy);
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringaSERKS0_(
    AbyssEngine::String *self, const AbyssEngine::String *other);
extern "C" void _ZN11AbyssEngine6StringD1Ev(AbyssEngine::String *self);

// ---- SetInActive_883c0.cpp ----
namespace AbyssEngine {

void GreenShader::SetInActive()
{
    if (i32(this, 0x20) >= 0)
        glDisableVertexAttribArray(i32(this, 0x20));
    if (i32(this, 0x24) >= 0)
        glDisableVertexAttribArray(i32(this, 0x24));
    if (i32(this, 0x28) >= 0)
        glDisableVertexAttribArray(i32(this, 0x28));
    if (i32(this, 0x2c) >= 0)
        glDisableVertexAttribArray(i32(this, 0x2c));
    if (i32(this, 0x30) >= 0)
        glDisableVertexAttribArray(i32(this, 0x30));
}

} // namespace AbyssEngine

// ---- _GreenShader_8847e.cpp ----
void _ZN11AbyssEngine11GreenShaderD0Ev(AbyssEngine::GreenShader *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}

// ---- Init_882a0.cpp ----
namespace AbyssEngine {

void GreenShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)(this))->ES2LoadProgram("GreenShader.vsh", "GreenShader.fsh");
    i32(this, 0x04) = program;

    i32(this, 0x20) = glGetAttribLocation(program, "a0");
    i32(this, 0x24) = glGetAttribLocation(i32(this, 0x04), "a1");
    i32(this, 0x28) = glGetAttribLocation(i32(this, 0x04), "a2");
    i32(this, 0x2c) = glGetAttribLocation(i32(this, 0x04), "a3");
    i32(this, 0x30) = glGetAttribLocation(i32(this, 0x04), "a4");

    i32(this, 0x34) = glGetUniformLocation(i32(this, 0x04), "u0");
    i32(this, 0x38) = glGetUniformLocation(i32(this, 0x04), "u1");
    i32(this, 0x3c) = glGetUniformLocation(i32(this, 0x04), "u2");
    i32(this, 0x40) = glGetUniformLocation(i32(this, 0x04), "u3");
    i32(this, 0x44) = glGetUniformLocation(i32(this, 0x04), "u4");
    i32(this, 0x48) = glGetUniformLocation(i32(this, 0x04), "u5");
    i32(this, 0x4c) = glGetUniformLocation(i32(this, 0x04), "u6");
    i32(this, 0x50) = glGetUniformLocation(i32(this, 0x04), "u7");
    i32(this, 0x54) = glGetUniformLocation(i32(this, 0x04), "u8");

    glUseProgram(i32(this, 0x04));
    return glUniform1i(i32(this, 0x44), 0);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_883fe.cpp ----
namespace AbyssEngine {

void GreenShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (i32(this, 0x34) >= 0)
        glUniformMatrix4fv(i32(this, 0x34), 1, 0, (const float *)((char *)engine + 0x104));

    if (i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(i32(this, 0x20));
    if (i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(i32(this, 0x24));
    if (i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(i32(this, 0x28));
    if (i32(this, 0x2c) >= 0)
        glEnableVertexAttribArray(i32(this, 0x2c));
    if (i32(this, 0x30) >= 0)
        glEnableVertexAttribArray(i32(this, 0x30));

    if (u8(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, i32(mesh, 0x60));
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, 0);
    } else {
        if (i32(this, 0x20) < 0)
            return;
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, (const void *)i32(mesh, 4));
    }
}

} // namespace AbyssEngine

// ---- GreenShader_88214.cpp ----
namespace AbyssEngine {

GreenShader::GreenShader()
{

    ShaderBaseStruct_ctor(this);
    i32(this, 0x0) = (int)(__UINTPTR_TYPE__)GreenShader_vtable + 8;
    GreenShader_typeInfoDest = GreenShader_typeInfoSource;

    {
        char tempStorage[sizeof(String)];
        String *temp = (String *)tempStorage;
        _ZN11AbyssEngine6StringC1EPKcb(temp, GreenShader_name, false);
        _ZN11AbyssEngine6StringaSERKS0_(&this->field_0xc, temp);
        _ZN11AbyssEngine6StringD1Ev(temp);
    }

    
}

} // namespace AbyssEngine
