#include "gof2/TextureConference.h"
#include "gof2/ApplicationManager.h"

extern "C" long long __divdi3_like(int a_lo, int a_hi, int b_lo, int b_hi);

// ---- SetInActive_8cc34.cpp ----
namespace AbyssEngine {

void TextureConference::SetInActive()
{
    glDisableVertexAttribArray(field_i32(this, 0x20));
    return glDisableVertexAttribArray(field_i32(this, 0x24));
}

} // namespace AbyssEngine

// ---- _TextureConference_8cd0a.cpp ----
void _ZN11AbyssEngine17TextureConferenceD0Ev(
    AbyssEngine::TextureConference *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

// ---- UpdateMeshData_8cc4a.cpp ----
namespace AbyssEngine {

void TextureConference::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x2c), 1, 0, (char *)engine + 0x104);
    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x30), 1, (float *)((char *)engine + 0xd0));
        field_u8(this, 0x9) = 0;
    }

    long long elapsed =
        ((ApplicationManager *)field_ptr(engine, 0x30))->GetElapsedTimeMillis();
    long long t = __divdi3_like((int)elapsed, (int)((unsigned long long)elapsed >> 32), 5, 0);
    t = t + this->animTime;
    long long clamped = (0xe10 > t) ? t : (t - 0xe10);
    this->animTime = clamped;
    glUniform1i(field_i32(this, 0x28), (int)clamped);

    glEnableVertexAttribArray(field_i32(this, 0x20));
    glEnableVertexAttribArray(field_i32(this, 0x24));
    glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    if ((field_u8(mesh, 0x0) & 2) != 0)
        glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
}

} // namespace AbyssEngine

// ---- Init_8cb98.cpp ----
namespace AbyssEngine {

void TextureConference::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram(
        "TextureConference.vsh", "TextureConference.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x34) = glGetUniformLocation(program, "u_texture");
    field_i32(this, 0x20) = glGetAttribLocation(field_i32(this, 0x04), "a_position");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a_texCoord");
    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u_mvp");
    field_i32(this, 0x28) = glGetUniformLocation(field_i32(this, 0x04), "u_color");
    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u_offset");

    glUseProgram(field_i32(this, 0x04));
    return glUniform1i(field_i32(this, 0x34), 0);
}

} // namespace AbyssEngine

// ---- TextureConference_8cb08.cpp ----
extern "C" AbyssEngine::TextureConference *
_ZN11AbyssEngine17TextureConferenceC2Ev(AbyssEngine::TextureConference *self)
{
    new ((AbyssEngine::ShaderBaseStruct *)self) AbyssEngine::ShaderBaseStruct();
    *(void *volatile *)self = _ZTVN11AbyssEngine17TextureConferenceE + 8;
    AbyssEngine::TextureConference::ShaderIndex =
        AbyssEngine::ShaderBaseStruct::shaderIndexIntern;

    String name("TextureConference");
    self->shaderName.assign(&name);

    self->animTime = 0;
    return self;
}
