#include "gof2/TextureShader.h"

// Engine / Mesh are opaque to this translation unit; their fields are accessed
// by raw offset since their real layouts live in other (non-batch) classes.
static inline char *bytes(void *self) { return (char *)self; }

// ---- Init_91084.cpp ----
namespace AbyssEngine {

void TextureShader::Init(Engine *)
{
    const char *vertexShader = "TextureShader.vsh";
    uint32_t program = ShaderBaseStruct_ES2LoadProgram(this, vertexShader, "TextureShader.fsh");
    this->field_0x4 = program;
    ConnectShaderComponents(program, 0);

    program = ShaderBaseStruct_ES2LoadProgram(this, vertexShader, "TextureShaderExt.fsh");
    this->field_0x20 = program;
    return ConnectShaderComponents(program, 1);
}

} // namespace AbyssEngine

// ---- _TextureShader_91258.cpp ----
extern "C" void _ZN11AbyssEngine13TextureShaderD0Ev(AbyssEngine::TextureShader *self)
{
    operator delete(ShaderBaseStruct_dtor(self));
}

// ---- UseShader_90f74.cpp ----
namespace AbyssEngine {

void TextureShader::UseShader(bool)
{
    if (g_TextureShader_hasSecondProgram != 0) {
        uint32_t program = this->field_0x20;
        if (program != 0) {
            return glUseProgram(program);
        }
    }
    return glUseProgram(this->field_0x4);
}

} // namespace AbyssEngine

// ---- SetInActive_910cc.cpp ----
namespace AbyssEngine {

void TextureShader::SetInActive()
{
    for (uint32_t i = 0; i != 2; i += 1) {
        glDisableVertexAttribArray(this->field_0x24[i]);
        glDisableVertexAttribArray(this->field_0x2c[i]);
    }
}

} // namespace AbyssEngine

// ---- TextureShader_90ee8.cpp ----
extern "C" AbyssEngine::TextureShader *TextureShader_TextureShader(AbyssEngine::TextureShader *self)
{
    struct Frame {
        void *nameStorage[3];
        void *cookie;
    } frame;

    frame.nameStorage[0] = self;
    frame.cookie = __stack_chk_guard;

    ShaderBaseStruct_ctor(self);

    void **source = (void **)g_TextureShader_typeInfoSource;
    void **target = (void **)g_TextureShader_typeInfoTarget;
    self->field_0x0 = (char *)g_TextureShader_vtable + 8;
    *target = *source;

    AbyssEngine::String_ctor_char((String *)frame.nameStorage, "TextureShader", false);
    AbyssEngine::String_assign(&self->field_0xc, (String *)frame.nameStorage);
    AbyssEngine::String_dtor((String *)frame.nameStorage);

    uint32_t guardDelta =
        (uint32_t)((__UINTPTR_TYPE__)__stack_chk_guard - (__UINTPTR_TYPE__)frame.cookie);
    if (guardDelta == 0) {
        return self;
    }
    __stack_chk_fail(guardDelta);
}

// ---- UpdateMeshData_910f8.cpp ----
namespace AbyssEngine {

void TextureShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    int slot = g_TextureShader_activeSlot;
    char *meshBytes = bytes(mesh);
    char *engineBytes = bytes(engine);

    glUniformMatrix4fv(this->field_0x34[slot], 1, 0, engineBytes + 0x104);

    int location = this->field_0x4c[slot];
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, 0, engineBytes + 0x1c4);
    }

    location = this->field_0x84[slot];
    if (location >= 0) {
        glUniform1f(location, 1.0f - *(float *)(meshBytes + 0x1c));
    }

    if (this->field_0x9 != 0) {
        glUniform4fv(this->field_0x3c[slot], 1, (float *)(engineBytes + 0xd0));

        location = this->field_0x54[slot];
        if (location >= 0) {
            glUniform1i(location, *(uint8_t *)(meshBytes + 0x85));
        }

        location = this->field_0x5c[slot];
        if (location >= 0) {
            glUniform3fv(location, 1, Vector_to_float((AEMath::Vector *)(engineBytes + 0x3f0)));
        }

        location = this->field_0x6c[slot];
        if (location >= 0) {
            glUniform1f(location, *(float *)(engineBytes + 0x3e8));
        }

        location = this->field_0x64[slot];
        if (location >= 0) {
            glUniform1f(location, *(float *)(engineBytes + 0x3ec));
        }

        location = this->field_0x74[slot];
        if (location >= 0) {
            glUniform1i(location, g_TextureShader_activeSlot);
        }

        location = this->field_0x7c[slot];
        if (location >= 0) {
            glUniform3f(location, *(float *)(engineBytes + 0x34c), *(float *)(engineBytes + 0x350),
                        *(float *)(engineBytes + 0x354));
        }

        this->field_0x9 = 0;
    }

    if ((int)(*(uint8_t *)meshBytes << 30) < 0) {
        glEnableVertexAttribArray(this->field_0x24[slot]);
        glEnableVertexAttribArray(this->field_0x2c[slot]);

        if (*(uint8_t *)(meshBytes + 0x5c) == 0) {
            glVertexAttribPointer(this->field_0x24[slot], 3, 0x1406, 0, 0, *(void **)(meshBytes + 0x04));
            glVertexAttribPointer(this->field_0x2c[slot], 2, 0x1406, 0, 0, *(void **)(meshBytes + 0x08));
        } else {
            glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x60));
            glVertexAttribPointer(this->field_0x24[slot], 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x68));
            glVertexAttribPointer(this->field_0x2c[slot], 2, 0x1406, 0, 0, 0);
        }
    }
}

} // namespace AbyssEngine

// ---- ConnectShaderComponents_90f94.cpp ----
namespace AbyssEngine {

void TextureShader::ConnectShaderComponents(uint32_t program, int slot)
{
    int (*uniformLocation)(uint32_t, const char *) = glGetUniformLocation;

    this->field_0x44[slot] = uniformLocation(program, "texture");
    this->field_0x24[slot] = glGetAttribLocation(program, "position");
    this->field_0x2c[slot] = glGetAttribLocation(program, "texcoord");
    this->field_0x34[slot] = uniformLocation(program, "mvp");
    this->field_0x3c[slot] = uniformLocation(program, "color");
    this->field_0x5c[slot] = uniformLocation(program, "light");
    this->field_0x64[slot] = uniformLocation(program, "fogNear");
    this->field_0x6c[slot] = uniformLocation(program, "fogFar");
    this->field_0x74[slot] = uniformLocation(program, "activeTexture");
    this->field_0x7c[slot] = uniformLocation(program, "fogColor");
    this->field_0x84[slot] = uniformLocation(program, "alpha");
    this->field_0x54[slot] = uniformLocation(program, "textureMode");
    this->field_0x4c[slot] = uniformLocation(program, "worldView");

    glUseProgram(program);
    return glUniform1i(this->field_0x44[slot], 0);
}

} // namespace AbyssEngine
