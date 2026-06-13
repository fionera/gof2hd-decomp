#include "gof2/game/core/TextureConference.h"
#include "gof2/engine/core/ApplicationManager.h"

extern "C" long long __divdi3_like(int a_lo, int a_hi, int b_lo, int b_hi);

namespace AbyssEngine {

void TextureConference::SetInActive()
{
    glDisableVertexAttribArray(this->attribPosition);
    return glDisableVertexAttribArray(this->attribTexCoord);
}

} // namespace AbyssEngine

void _ZN11AbyssEngine17TextureConferenceD0Ev(
    AbyssEngine::TextureConference *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

namespace AbyssEngine {

void TextureConference::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(this->mvpMatrixLoc, 1, 0, (char *)engine + 0x104);
    if (this->offsetDirty != 0) {
        glUniform4fv(this->offsetLoc, 1, (float *)((char *)engine + 0xd0));
        this->offsetDirty = 0;
    }

    long long elapsed =
        ((ApplicationManager *)field_ptr(engine, 0x30))->GetElapsedTimeMillis();
    long long t = __divdi3_like((int)elapsed, (int)((unsigned long long)elapsed >> 32), 5, 0);
    t = t + this->animTime;
    long long clamped = (0xe10 > t) ? t : (t - 0xe10);
    this->animTime = clamped;
    glUniform1i(this->colorLoc, (int)clamped);

    glEnableVertexAttribArray(this->attribPosition);
    glEnableVertexAttribArray(this->attribTexCoord);
    glVertexAttribPointer(this->attribPosition, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    if ((field_u8(mesh, 0x0) & 2) != 0)
        glVertexAttribPointer(this->attribTexCoord, 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
}

} // namespace AbyssEngine

namespace AbyssEngine {

void TextureConference::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram(
        "TextureConference.vsh", "TextureConference.fsh");
    this->programHandle = program;

    this->textureLoc = glGetUniformLocation(program, "u_texture");
    this->attribPosition = glGetAttribLocation(this->programHandle, "a_position");
    this->attribTexCoord = glGetAttribLocation(this->programHandle, "a_texCoord");
    this->mvpMatrixLoc = glGetUniformLocation(this->programHandle, "u_mvp");
    this->colorLoc = glGetUniformLocation(this->programHandle, "u_color");
    this->offsetLoc = glGetUniformLocation(this->programHandle, "u_offset");

    glUseProgram(this->programHandle);
    return glUniform1i(this->textureLoc, 0);
}

} // namespace AbyssEngine

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
