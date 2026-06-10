#include "TextureVtxColorShader.h"


extern "C" void _ZN11AbyssEngine6StringD1Ev(AbyssEngine::String *self);

// ---- Init_8bed0.cpp ----
namespace AbyssEngine {

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

    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram(vertexShader, fragmentShader);
    field_i32(this, 0x04) = program;
    ConnectShaderComponents(program, 0);

    int fogProgram = ((ShaderBaseStruct *)this)->ES2LoadProgram(vertexShader, "\n");
    field_i32(this, 0x20) = fogProgram;
    return ConnectShaderComponents(fogProgram, 1);
}

} // namespace AbyssEngine

// ---- _TextureVtxColorShader_8c10c.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructD2Ev(
    AbyssEngine::ShaderBaseStruct *self);

extern "C" void _ZN11AbyssEngine21TextureVtxColorShaderD0Ev(
    AbyssEngine::TextureVtxColorShader *self)
{
    operator delete(_ZN11AbyssEngine16ShaderBaseStructD2Ev(
        (AbyssEngine::ShaderBaseStruct *)self));
}

// ---- SetInActive_8bf18.cpp ----
namespace AbyssEngine {

void TextureVtxColorShader::SetInActive()
{
    void (*disableVertexAttribArray)(uint32_t) = glDisableVertexAttribArray;

    for (int i = 0; i != 2; i++) {
        char *entry = (char *)this + i * 4;
        disableVertexAttribArray(*(uint32_t *)(entry + 0x24));
        disableVertexAttribArray(*(uint32_t *)(entry + 0x2c));
        disableVertexAttribArray(*(uint32_t *)(entry + 0x34));
    }
}

} // namespace AbyssEngine

// ---- TextureVtxColorShader_8bd1c.cpp ----
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructC2Ev(
    AbyssEngine::ShaderBaseStruct *self);
extern "C" void _ZN11AbyssEngine6StringC1EPKcb(
    AbyssEngine::String *self, const char *text, bool copy);
extern "C" AbyssEngine::String *_ZN11AbyssEngine6StringaSERKS0_(
    AbyssEngine::String *self, const AbyssEngine::String *other);

namespace AbyssEngine {

static inline String *shaderName(TextureVtxColorShader *self)
{
    return (String *)((char *)self + 0x0c);
}

struct ConstructorFrame {
    char name[sizeof(String)];
    volatile uint32_t stackGuard;
};

} // namespace AbyssEngine

extern "C" AbyssEngine::TextureVtxColorShader *
_ZN11AbyssEngine21TextureVtxColorShaderC2Ev(AbyssEngine::TextureVtxColorShader *self)
{
    AbyssEngine::ConstructorFrame frame;
    frame.stackGuard = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;

    _ZN11AbyssEngine16ShaderBaseStructC2Ev((AbyssEngine::ShaderBaseStruct *)self);
    *(void *volatile *)self = _ZTVN11AbyssEngine21TextureVtxColorShaderE + 8;
    AbyssEngine::TextureVtxColorShader::ShaderIndex =
        AbyssEngine::ShaderBaseStruct::shaderIndexIntern;

    _ZN11AbyssEngine6StringC1EPKcb(
        (AbyssEngine::String *)frame.name, "TextureVtxColorShader", false);
    _ZN11AbyssEngine6StringaSERKS0_(
        AbyssEngine::shaderName(self), (AbyssEngine::String *)frame.name);
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)frame.name);

    uint32_t stackDifference = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - frame.stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
    return self;
}

// ---- ConnectShaderComponents_8bdc8.cpp ----
namespace AbyssEngine {

void TextureVtxColorShader::ConnectShaderComponents(uint32_t program, int index)
{
    int (*getUniformLocation)(uint32_t, const char *) = glGetUniformLocation;
    int (*getAttribLocation)(uint32_t, const char *) = glGetAttribLocation;
    char *entry = (char *)this + index * 4;

    *(int *)(entry + 0x4c) = getUniformLocation(program, "s_texture");
    *(int *)(entry + 0x24) = getAttribLocation(program, "a_position");
    *(int *)(entry + 0x2c) = getAttribLocation(program, "a_texCoord");
    *(int *)(entry + 0x34) = getAttribLocation(program, "a_VertexColor");
    *(int *)(entry + 0x3c) = getUniformLocation(program, "u_WorldMatrix");
    *(int *)(entry + 0x44) = getUniformLocation(program, "glColor");
    *(int *)(entry + 0x8c) = getUniformLocation(program, "u_DarkenValue");
    *(int *)(entry + 0x64) = getUniformLocation(program, "u_fogColor");
    *(int *)(entry + 0x6c) = getUniformLocation(program, "u_fogMaxDist");
    *(int *)(entry + 0x74) = getUniformLocation(program, "u_fogMinDist");
    *(int *)(entry + 0x7c) = getUniformLocation(program, "u_EnableFog");
    *(int *)(entry + 0x84) = getUniformLocation(program, "u_eyeposmodel");
    *(int *)(entry + 0x54) = getUniformLocation(program, "u_UVAnimation");
    *(int *)(entry + 0x5c) = getUniformLocation(program, "u_UvMatrix");

    glUseProgram(program);
    return glUniform1i(*(int *)(entry + 0x4c), 0);
}

} // namespace AbyssEngine

// ---- UseShader_8bda8.cpp ----
namespace AbyssEngine {

__attribute__((minsize)) void TextureVtxColorShader::UseShader(bool)
{
    int program;
    if (Engine::fogEnabled && (program = field_i32(this, 0x20)) != 0) {
        return glUseProgram(program);
    }
    return glUseProgram(field_i32(this, 0x04));
}

} // namespace AbyssEngine

// ---- UpdateMeshData_8bf4c.cpp ----
namespace AbyssEngine {

void TextureVtxColorShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    uint8_t *fogEnabled = (uint8_t *)&Engine::fogEnabled;
    uint8_t fogIndex = *fogEnabled;
    char *entry = (char *)this + fogIndex * 4;

    glUniformMatrix4fv(*(int *)(entry + 0x3c), 1, 0, (float *)((char *)engine + 0x104));

    int location = *(int *)(entry + 0x5c);
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, 0, (float *)((char *)engine + 0x1c4));
    }

    location = *(int *)(entry + 0x8c);
    if (location >= 0) {
        glUniform1f(location, 1.0f - *(float *)((char *)mesh + 0x1c));
    }

    if (field_u8(this, 0x09) != 0) {
        glUniform4fv(*(int *)(entry + 0x44), 1, (float *)((char *)engine + 0xd0));

        location = *(int *)(entry + 0x54);
        if (location >= 0) {
            glUniform1i(location, *(uint8_t *)((char *)mesh + 0x85));
        }

        location = *(int *)(entry + 0x64);
        if (location >= 0) {
            AEMath::Vector *eye = (AEMath::Vector *)((char *)engine + 0x3f0);
            glUniform3fv(location, 1, eye->operator float *());
        }

        location = *(int *)(entry + 0x74);
        if (location >= 0) {
            glUniform1f(location, field_f32(engine, 0x3e8));
        }

        location = *(int *)(entry + 0x6c);
        if (location >= 0) {
            glUniform1f(location, field_f32(engine, 0x3ec));
        }

        location = *(int *)(entry + 0x7c);
        if (location >= 0) {
            glUniform1i(location, *fogEnabled);
        }

        location = *(int *)(entry + 0x84);
        if (location >= 0) {
            glUniform3f(
                location,
                field_f32(engine, 0x34c),
                field_f32(engine, 0x350),
                field_f32(engine, 0x354));
        }

        field_u8(this, 0x09) = 0;
    }

    void (*enableVertexAttribArray)(uint32_t) = glEnableVertexAttribArray;
    char *position = entry;
    enableVertexAttribArray(*(uint32_t *)(position += 0x24));
    char *texCoord = entry;
    enableVertexAttribArray(*(uint32_t *)(texCoord += 0x2c));
    char *vertexColor = entry;
    enableVertexAttribArray(*(uint32_t *)(vertexColor += 0x34));

    if (*(uint8_t *)((char *)mesh + 0x5c) != 0) {
        void (*bindBuffer)(uint32_t, uint32_t) = glBindBuffer;
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, *(uint32_t *)((char *)mesh + 0x60));
        vertexAttribPointer(*(uint32_t *)position, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, *(uint32_t *)((char *)mesh + 0x68));
        vertexAttribPointer(*(uint32_t *)texCoord, 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, *(uint32_t *)((char *)mesh + 0x78));
        vertexAttribPointer(*(uint32_t *)vertexColor, 4, 0x1406, 0, 0, 0);
        return;
    }

    void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
        glVertexAttribPointer;
    vertexAttribPointer(*(uint32_t *)position, 3, 0x1406, 0, 0, field_ptr(mesh, 0x04));
    vertexAttribPointer(*(uint32_t *)texCoord, 2, 0x1406, 0, 0, field_ptr(mesh, 0x08));
    return vertexAttribPointer(*(uint32_t *)vertexColor, 4, 0x1406, 0, 0, field_ptr(mesh, 0x0c));
}

} // namespace AbyssEngine
