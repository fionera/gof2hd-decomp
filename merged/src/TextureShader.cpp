#include "TextureShader.h"




// ---- Init_91084.cpp ----
namespace AbyssEngine {

void TextureShader::Init(Engine *)
{
    const char *vertexShader = "TextureShader.vsh";
    uint32_t program = ShaderBaseStruct_ES2LoadProgram(this, vertexShader, "TextureShader.fsh");
    field<uint32_t>(this, 0x04) = program;
    ConnectShaderComponents(program, 0);

    program = ShaderBaseStruct_ES2LoadProgram(this, vertexShader, "TextureShaderExt.fsh");
    field<uint32_t>(this, 0x20) = program;
    return ConnectShaderComponents(program, 1);
}

} // namespace AbyssEngine

// ---- _TextureShader_91258.cpp ----
extern "C" void _ZN11AbyssEngine13TextureShaderD0Ev(TextureShader *self)
{
    operator delete(ShaderBaseStruct_dtor(self));
}

// ---- UseShader_90f74.cpp ----
namespace AbyssEngine {

void TextureShader::UseShader(bool)
{
    if (g_TextureShader_hasSecondProgram != 0) {
        uint32_t program = field<uint32_t>(this, 0x20);
        if (program != 0) {
            return glUseProgram(program);
        }
    }
    return glUseProgram(field<uint32_t>(this, 0x04));
}

} // namespace AbyssEngine

// ---- SetInActive_910cc.cpp ----
namespace AbyssEngine {

void TextureShader::SetInActive()
{
    for (uint32_t i = 0; i != 2; i += 1) {
        char *slot = bytes(this) + i * 4;
        glDisableVertexAttribArray(*(uint32_t *)(slot + 0x24));
        glDisableVertexAttribArray(*(uint32_t *)(slot + 0x2c));
    }
}

} // namespace AbyssEngine

// ---- TextureShader_90ee8.cpp ----
extern "C" TextureShader *TextureShader_TextureShader(TextureShader *self)
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
    *(void *volatile *)self = (char *)g_TextureShader_vtable + 8;
    *target = *source;

    String_ctor_char((String *)frame.nameStorage, "TextureShader", false);
    String_assign((String *)(bytes(self) + 0x0c), (String *)frame.nameStorage);
    String_dtor((String *)frame.nameStorage);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)frame.cookie;
    if (guardDelta == 0) {
        return self;
    }
    __stack_chk_fail(guardDelta);
}

// ---- UpdateMeshData_910f8.cpp ----
namespace AbyssEngine {

void TextureShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    uint8_t slotIndex = g_TextureShader_activeSlot;
    char *shader = bytes(this) + slotIndex * 4;
    char *meshBytes = bytes(mesh);
    char *engineBytes = bytes(engine);

    glUniformMatrix4fv(*(int *)(shader + 0x34), 1, 0, engineBytes + 0x104);

    int location = *(int *)(shader + 0x4c);
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, 0, engineBytes + 0x1c4);
    }

    location = *(int *)(shader + 0x84);
    if (location >= 0) {
        glUniform1f(location, 1.0f - *(float *)(meshBytes + 0x1c));
    }

    if (*(uint8_t *)(bytes(this) + 0x09) != 0) {
        glUniform4fv(*(int *)(shader + 0x3c), 1, (float *)(engineBytes + 0xd0));

        location = *(int *)(shader + 0x54);
        if (location >= 0) {
            glUniform1i(location, *(uint8_t *)(meshBytes + 0x85));
        }

        location = *(int *)(shader + 0x5c);
        if (location >= 0) {
            glUniform3fv(location, 1, Vector_to_float((Vector *)(engineBytes + 0x3f0)));
        }

        location = *(int *)(shader + 0x6c);
        if (location >= 0) {
            glUniform1f(location, *(float *)(engineBytes + 0x3e8));
        }

        location = *(int *)(shader + 0x64);
        if (location >= 0) {
            glUniform1f(location, *(float *)(engineBytes + 0x3ec));
        }

        location = *(int *)(shader + 0x74);
        if (location >= 0) {
            glUniform1i(location, g_TextureShader_activeSlot);
        }

        location = *(int *)(shader + 0x7c);
        if (location >= 0) {
            glUniform3f(location, *(float *)(engineBytes + 0x34c), *(float *)(engineBytes + 0x350),
                        *(float *)(engineBytes + 0x354));
        }

        *(uint8_t *)(bytes(this) + 0x09) = 0;
    }

    if ((int)(*(uint8_t *)meshBytes << 30) < 0) {
        shader += 0x24;
        char *position = shader;
        glEnableVertexAttribArray(*(uint32_t *)position);

        shader += 8;
        glEnableVertexAttribArray(*(uint32_t *)shader);

        if (*(uint8_t *)(meshBytes + 0x5c) == 0) {
            glVertexAttribPointer(*(uint32_t *)position, 3, 0x1406, 0, 0, *(void **)(meshBytes + 0x04));
            glVertexAttribPointer(*(uint32_t *)shader, 2, 0x1406, 0, 0, *(void **)(meshBytes + 0x08));
        } else {
            glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x60));
            glVertexAttribPointer(*(uint32_t *)position, 3, 0x1406, 0, 0, 0);
            glBindBuffer(0x8892, *(uint32_t *)(meshBytes + 0x68));
            glVertexAttribPointer(*(uint32_t *)shader, 2, 0x1406, 0, 0, 0);
        }
    }
}

} // namespace AbyssEngine

// ---- ConnectShaderComponents_90f94.cpp ----
namespace AbyssEngine {

void TextureShader::ConnectShaderComponents(uint32_t program, int slot)
{
    char *shader = bytes(this) + slot * 4;
    int (*uniformLocation)(uint32_t, const char *) = glGetUniformLocation;

    *(int *)(shader + 0x44) = uniformLocation(program, "texture");
    *(int *)(shader + 0x24) = glGetAttribLocation(program, "position");
    *(int *)(shader + 0x2c) = glGetAttribLocation(program, "texcoord");
    *(int *)(shader + 0x34) = uniformLocation(program, "mvp");
    *(int *)(shader + 0x3c) = uniformLocation(program, "color");
    *(int *)(shader + 0x5c) = uniformLocation(program, "light");
    *(int *)(shader + 0x64) = uniformLocation(program, "fogNear");
    *(int *)(shader + 0x6c) = uniformLocation(program, "fogFar");
    *(int *)(shader + 0x74) = uniformLocation(program, "activeTexture");
    *(int *)(shader + 0x7c) = uniformLocation(program, "fogColor");
    *(int *)(shader + 0x84) = uniformLocation(program, "alpha");
    *(int *)(shader + 0x54) = uniformLocation(program, "textureMode");
    *(int *)(shader + 0x4c) = uniformLocation(program, "worldView");

    glUseProgram(program);
    return glUniform1i(*(int *)(shader + 0x44), 0);
}

} // namespace AbyssEngine
