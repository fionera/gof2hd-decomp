#include "gof2/TextureVtxColorShader.h"
#include "gof2/Mesh.h"

namespace AbyssEngine {

// ---- Init_8bed0.cpp ----
void TextureVtxColorShader::Init(Engine *)
{
    const char *vertexShader =
        "attribute highp vec4 a_position;\n"
        "attribute mediump vec2 a_texCoord;\n"
        "attribute mediump vec4 a_VertexColor;\n";
    const char *fragmentShader =
        "precision lowp float;\n"
        "varying mediump vec2 v_texCoord;\n"
        "varying mediump vec4 v_VertexColor;\n";

    this->program = ((ShaderBaseStruct *)this)->ES2LoadProgram(vertexShader, fragmentShader);
    ConnectShaderComponents(this->program, 0);

    this->fogProgram = ((ShaderBaseStruct *)this)->ES2LoadProgram(vertexShader, "\n");
    ConnectShaderComponents(this->fogProgram, 1);
}

// ---- ConnectShaderComponents_8bdc8.cpp ----
void TextureVtxColorShader::ConnectShaderComponents(uint32_t program, int index)
{
    loc_s_texture[index]      = glGetUniformLocation(program, "s_texture");
    loc_a_position[index]     = glGetAttribLocation(program, "a_position");
    loc_a_texCoord[index]     = glGetAttribLocation(program, "a_texCoord");
    loc_a_VertexColor[index]  = glGetAttribLocation(program, "a_VertexColor");
    loc_u_WorldMatrix[index]  = glGetUniformLocation(program, "u_WorldMatrix");
    loc_glColor[index]        = glGetUniformLocation(program, "glColor");
    loc_u_DarkenValue[index]  = glGetUniformLocation(program, "u_DarkenValue");
    loc_u_fogColor[index]     = glGetUniformLocation(program, "u_fogColor");
    loc_u_fogMaxDist[index]   = glGetUniformLocation(program, "u_fogMaxDist");
    loc_u_fogMinDist[index]   = glGetUniformLocation(program, "u_fogMinDist");
    loc_u_EnableFog[index]    = glGetUniformLocation(program, "u_EnableFog");
    loc_u_eyeposmodel[index]  = glGetUniformLocation(program, "u_eyeposmodel");
    loc_u_UVAnimation[index]  = glGetUniformLocation(program, "u_UVAnimation");
    loc_u_UvMatrix[index]     = glGetUniformLocation(program, "u_UvMatrix");

    glUseProgram(program);
    glUniform1i(loc_s_texture[index], 0);
}

// ---- UseShader_8bda8.cpp ----
__attribute__((minsize)) void TextureVtxColorShader::UseShader(bool)
{
    if (Engine::fogEnabled && this->fogProgram != 0) {
        glUseProgram(this->fogProgram);
        return;
    }
    glUseProgram(this->program);
}

// ---- SetInActive_8bf18.cpp ----
void TextureVtxColorShader::SetInActive()
{
    for (int i = 0; i != 2; i++) {
        glDisableVertexAttribArray((uint32_t)loc_a_position[i]);
        glDisableVertexAttribArray((uint32_t)loc_a_texCoord[i]);
        glDisableVertexAttribArray((uint32_t)loc_a_VertexColor[i]);
    }
}

// ---- UpdateMeshData_8bf4c.cpp ----
void TextureVtxColorShader::UpdateMeshData(::Mesh *mesh, Engine *engine)
{
    int index = Engine::fogEnabled ? 1 : 0;

    glUniformMatrix4fv(loc_u_WorldMatrix[index], 1, 0, engine->worldMatrix);

    int location = loc_u_UvMatrix[index];
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, 0, engine->uvMatrix);
    }

    location = loc_u_DarkenValue[index];
    if (location >= 0) {
        glUniform1f(location, 1.0f - (float)mesh->field_0x1c);
    }

    if (this->needsUniformUpdate != 0) {
        glUniform4fv(loc_glColor[index], 1, engine->glColor);

        location = loc_u_UVAnimation[index];
        if (location >= 0) {
            glUniform1i(location, mesh->field_0x85);
        }

        location = loc_u_fogColor[index];
        if (location >= 0) {
            glUniform3fv(location, 1, engine->eyePosModel);
        }

        location = loc_u_fogMinDist[index];
        if (location >= 0) {
            glUniform1f(location, ((float *)engine)[0x3e8 / 4]);
        }

        location = loc_u_fogMaxDist[index];
        if (location >= 0) {
            glUniform1f(location, ((float *)engine)[0x3ec / 4]);
        }

        location = loc_u_EnableFog[index];
        if (location >= 0) {
            glUniform1i(location, Engine::fogEnabled ? 1 : 0);
        }

        location = loc_u_eyeposmodel[index];
        if (location >= 0) {
            glUniform3f(location, engine->eyePosModelX, engine->eyePosModelY, engine->eyePosModelZ);
        }

        this->needsUniformUpdate = 0;
    }

    glEnableVertexAttribArray((uint32_t)loc_a_position[index]);
    glEnableVertexAttribArray((uint32_t)loc_a_texCoord[index]);
    glEnableVertexAttribArray((uint32_t)loc_a_VertexColor[index]);

    if (mesh->field_0x5c != 0) {
        glBindBuffer(0x8892, mesh->field_0x60);
        glVertexAttribPointer((uint32_t)loc_a_position[index], 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x68);
        glVertexAttribPointer((uint32_t)loc_a_texCoord[index], 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x78);
        glVertexAttribPointer((uint32_t)loc_a_VertexColor[index], 4, 0x1406, 0, 0, 0);
        return;
    }

    glVertexAttribPointer((uint32_t)loc_a_position[index], 3, 0x1406, 0, 0, (const void *)mesh->field_0x4);
    glVertexAttribPointer((uint32_t)loc_a_texCoord[index], 2, 0x1406, 0, 0, (const void *)mesh->field_0x8);
    glVertexAttribPointer((uint32_t)loc_a_VertexColor[index], 4, 0x1406, 0, 0, (const void *)(__SIZE_TYPE__)mesh->field_0xc);
}

} // namespace AbyssEngine

// ---- TextureVtxColorShader_8bd1c.cpp (constructor) ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructC2Ev(
    AbyssEngine::ShaderBaseStruct *self);
extern "C" void _ZN11AbyssEngine6StringC1EPKcb(
    AbyssEngine::String *self, const char *text, bool copy);
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringaSERKS0_(
    AbyssEngine::String *self, const AbyssEngine::String *other);
extern "C" void _ZN11AbyssEngine6StringD1Ev(AbyssEngine::String *self);

extern "C" AbyssEngine::TextureVtxColorShader *
_ZN11AbyssEngine21TextureVtxColorShaderC2Ev(AbyssEngine::TextureVtxColorShader *self)
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((AbyssEngine::ShaderBaseStruct *)self);
    self->vtable = (void *)(_ZTVN11AbyssEngine21TextureVtxColorShaderE + 8);
    AbyssEngine::TextureVtxColorShader::ShaderIndex =
        AbyssEngine::ShaderBaseStruct::shaderIndexIntern;

    AbyssEngine::String name;
    _ZN11AbyssEngine6StringC1EPKcb(&name, "TextureVtxColorShader", false);
    _ZN11AbyssEngine6StringaSERKS0_(&self->shaderName, &name);
    _ZN11AbyssEngine6StringD1Ev(&name);
    return self;
}

// ---- _TextureVtxColorShader_8c10c.cpp (deleting destructor) ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine21TextureVtxColorShaderD0Ev(
    AbyssEngine::TextureVtxColorShader *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}
