#include "gof2/TextureShader.h"
#include "gof2/ShaderBaseStruct.h"
#include "gof2/String.h"

// Engine / Mesh are opaque to this translation unit; their fields are accessed
// by raw offset since their real layouts live in other (non-batch) classes.
static inline char *bytes(void *self) { return (char *)self; }

// ---- Init_91084.cpp ----
namespace AbyssEngine {

void TextureShader::Init(Engine *)
{
    const char *vertexShader = "TextureShader.vsh";
    uint32_t program = ((ShaderBaseStruct *)(this))->ES2LoadProgram(vertexShader, "TextureShader.fsh");
    this->program = program;
    ConnectShaderComponents(program, 0);

    program = ((ShaderBaseStruct *)(this))->ES2LoadProgram(vertexShader, "TextureShaderExt.fsh");
    this->programExt = program;
    return ConnectShaderComponents(program, 1);
}

} // namespace AbyssEngine

// ---- _TextureShader_91258.cpp ----
void _ZN11AbyssEngine13TextureShaderD0Ev(AbyssEngine::TextureShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

// ---- UseShader_90f74.cpp ----
namespace AbyssEngine {

void TextureShader::UseShader(bool)
{
    if (g_TextureShader_hasSecondProgram != 0) {
        uint32_t program = this->programExt;
        if (program != 0) {
            return glUseProgram(program);
        }
    }
    return glUseProgram(this->program);
}

} // namespace AbyssEngine

// ---- SetInActive_910cc.cpp ----
namespace AbyssEngine {

void TextureShader::SetInActive()
{
    for (uint32_t i = 0; i != 2; i += 1) {
        glDisableVertexAttribArray(this->positionAttrib[i]);
        glDisableVertexAttribArray(this->texcoordAttrib[i]);
    }
}

} // namespace AbyssEngine

// ---- TextureShader_90ee8.cpp ----
AbyssEngine::TextureShader *TextureShader_TextureShader(AbyssEngine::TextureShader *self)
{
    struct Frame {
        void *nameStorage[3];
        void *cookie;
    } frame;

    frame.nameStorage[0] = self;

    new ((AbyssEngine::ShaderBaseStruct *)self) AbyssEngine::ShaderBaseStruct();

    void **source = (void **)g_TextureShader_typeInfoSource;
    void **target = (void **)g_TextureShader_typeInfoTarget;
    self->vtable = (char *)g_TextureShader_vtable + 8;
    *target = *source;

    ((String *)((String *)frame.nameStorage))->ctor_char("TextureShader", false);
    ((String *)(&self->name))->assign((String *)frame.nameStorage);
    ((String *)((String *)frame.nameStorage))->dtor();

    return self;
}

// ---- UpdateMeshData_910f8.cpp ----
namespace AbyssEngine {

void TextureShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    int slot = g_TextureShader_activeSlot;
    char *meshBytes = bytes(mesh);
    char *engineBytes = bytes(engine);

    glUniformMatrix4fv(this->mvpUniform[slot], 1, 0, engineBytes + 0x104);

    int location = this->worldViewUniform[slot];
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, 0, engineBytes + 0x1c4);
    }

    location = this->alphaUniform[slot];
    if (location >= 0) {
        glUniform1f(location, 1.0f - *(float *)(meshBytes + 0x1c));
    }

    if (this->dirty != 0) {
        glUniform4fv(this->colorUniform[slot], 1, (float *)(engineBytes + 0xd0));

        location = this->textureModeUniform[slot];
        if (location >= 0) {
            glUniform1i(location, *(uint8_t *)(meshBytes + 0x85));
        }

        location = this->lightUniform[slot];
        if (location >= 0) {
            glUniform3fv(location, 1, Vector_to_float((AEMath::Vector *)(engineBytes + 0x3f0)));
        }

        location = this->fogFarUniform[slot];
        if (location >= 0) {
            glUniform1f(location, *(float *)(engineBytes + 0x3e8));
        }

        location = this->fogNearUniform[slot];
        if (location >= 0) {
            glUniform1f(location, *(float *)(engineBytes + 0x3ec));
        }

        location = this->activeTextureUniform[slot];
        if (location >= 0) {
            glUniform1i(location, g_TextureShader_activeSlot);
        }

        location = this->fogColorUniform[slot];
        if (location >= 0) {
            glUniform3f(location, *(float *)(engineBytes + 0x34c), *(float *)(engineBytes + 0x350),
                        *(float *)(engineBytes + 0x354));
        }

        this->dirty = 0;
    }

    if ((int)(*(uint8_t *)meshBytes << 30) < 0) {
        glEnableVertexAttribArray(this->positionAttrib[slot]);
        glEnableVertexAttribArray(this->texcoordAttrib[slot]);

        if (*(uint8_t *)(meshBytes + 0x5c) == 0) {
            glVertexAttribPointer(this->positionAttrib[slot], 3, 0x1406, 0, 0, *(void **)(meshBytes + 0x04));
            glVertexAttribPointer(this->texcoordAttrib[slot], 2, 0x1406, 0, 0, *(void **)(meshBytes + 0x08));
        } else {
            glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x60));
            glVertexAttribPointer(this->positionAttrib[slot], 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x68));
            glVertexAttribPointer(this->texcoordAttrib[slot], 2, 0x1406, 0, 0, 0);
        }
    }
}

} // namespace AbyssEngine

// ---- ConnectShaderComponents_90f94.cpp ----
namespace AbyssEngine {

void TextureShader::ConnectShaderComponents(uint32_t program, int slot)
{
    int (*uniformLocation)(uint32_t, const char *) = glGetUniformLocation;

    this->textureUniform[slot] = uniformLocation(program, "texture");
    this->positionAttrib[slot] = glGetAttribLocation(program, "position");
    this->texcoordAttrib[slot] = glGetAttribLocation(program, "texcoord");
    this->mvpUniform[slot] = uniformLocation(program, "mvp");
    this->colorUniform[slot] = uniformLocation(program, "color");
    this->lightUniform[slot] = uniformLocation(program, "light");
    this->fogNearUniform[slot] = uniformLocation(program, "fogNear");
    this->fogFarUniform[slot] = uniformLocation(program, "fogFar");
    this->activeTextureUniform[slot] = uniformLocation(program, "activeTexture");
    this->fogColorUniform[slot] = uniformLocation(program, "fogColor");
    this->alphaUniform[slot] = uniformLocation(program, "alpha");
    this->textureModeUniform[slot] = uniformLocation(program, "textureMode");
    this->worldViewUniform[slot] = uniformLocation(program, "worldView");

    glUseProgram(program);
    return glUniform1i(this->textureUniform[slot], 0);
}

} // namespace AbyssEngine
