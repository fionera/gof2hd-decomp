#include "gof2/TextureAlphaTestShader.h"

// AbyssEngine::Mesh is forward-declared only (its full header conflicts with this TU's
// global typedefs), so read the few Mesh fields used here by typed offset.
static inline void *&meshPtr(void *m, int off) { return *(void **)((char *)m + off); }
static inline unsigned int &meshU32(void *m, int off) { return *(unsigned int *)((char *)m + off); }

// String(const char*, bool) ctor -- provided by the engine (no in-tree definition).
extern "C" void String_ctor_cstr(String *self, const char *text, bool copy);




// ---- Init_89230.cpp ----

void TextureAlphaTestShader::Init(Engine *)
{
    int program = ShaderBaseStruct_ES2LoadProgram((ShaderBaseStruct *)this,
                                                  "TextureAlphaTestShader.vsh",
                                                  "TextureAlphaTestShader.fsh");
    i32(this, 0x04) = program;
    ConnectShaderComponents(program, 0);

    program = ShaderBaseStruct_ES2LoadProgram((ShaderBaseStruct *)this,
                                              "TextureAlphaTestShader.vsh",
                                              "TextureAlphaTestShaderAlpha.fsh");
    i32(this, 0x20) = program;
    ConnectShaderComponents(program, 1);
}


// ---- SetInActive_89278.cpp ----

void TextureAlphaTestShader::SetInActive()
{
    for (int i = 0; i != 2; i++) {
        char *base = (char *)this + i * 4;
        glDisableVertexAttribArray(*(int *)(base + 0x24));
        glDisableVertexAttribArray(*(int *)(base + 0x2c));
    }
}


// ---- UpdateMeshData_892a4.cpp ----

void TextureAlphaTestShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    uint8_t programIndex = g_TextureAlphaTestShader_programIndex;
    char *shader = (char *)this + programIndex * 4;

    glUniformMatrix4fv(*(int *)(shader + 0x34), 1, 0, (float *)((char *)engine + 0x104));

    if (u8(this, 0x09) != 0) {
        int location = *(int *)(shader + 0x3c);
        if (location >= 0) {
            glUniform4fv(location, 1, (float *)((char *)engine + 0xd0));
        }

        location = *(int *)(shader + 0x4c);
        if (location >= 0) {
            glUniform3fv(location, 1, Vector_cast_to_float((Vector *)((char *)engine + 0x3f0)));
        }

        location = *(int *)(shader + 0x5c);
        if (location >= 0) {
            glUniform1f(location, f32(engine, 0x3e8));
        }

        location = *(int *)(shader + 0x54);
        if (location >= 0) {
            glUniform1f(location, f32(engine, 0x3ec));
        }

        location = *(int *)(shader + 0x64);
        if (location >= 0) {
            glUniform1i(location, g_TextureAlphaTestShader_programIndex);
        }

        location = *(int *)(shader + 0x6c);
        if (location >= 0) {
            glUniform3f(location, f32(engine, 0x34c), f32(engine, 0x350), f32(engine, 0x354));
        }

        u8(this, 0x09) = 0;
    }

    if ((((uint32_t)u8(mesh, 0x00)) << 30) < 0x80000000u) {
        return;
    }

    int *position = (int *)(shader + 0x24);
    int *texcoord = position + 2;
    glEnableVertexAttribArray(*position);
    glEnableVertexAttribArray(*texcoord);

    if (u8(mesh, 0x5c) == 0) {
        glVertexAttribPointer(*position, 3, 0x1406, 0, 0, meshPtr(mesh, 0x4));
        glVertexAttribPointer(*texcoord, 2, 0x1406, 0, 0, meshPtr(mesh, 0x8));
    } else {
        glBindBuffer(0x8892, meshU32(mesh, 0x60));
        glVertexAttribPointer(*position, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, meshU32(mesh, 0x68));
        glVertexAttribPointer(*texcoord, 2, 0x1406, 0, 0, 0);
    }
}


// ---- ConnectShaderComponents_8916c.cpp ----

void TextureAlphaTestShader::ConnectShaderComponents(int program, int index)
{
    int (*getUniformLocation)(int, const char *) = glGetUniformLocation;
    char *base = (char *)this + index * 4;
    *(int *)(base + 0x44) = getUniformLocation(program, "u_Texture");
    *(int *)(base + 0x24) = glGetAttribLocation(program, "a_Position");
    *(int *)(base + 0x2c) = glGetAttribLocation(program, "a_TexCoord");
    *(int *)(base + 0x34) = getUniformLocation(program, "u_MVPMatrix");
    *(int *)(base + 0x3c) = getUniformLocation(program, "u_Color");
    *(int *)(base + 0x4c) = getUniformLocation(program, "u_LightPos");
    *(int *)(base + 0x54) = getUniformLocation(program, "u_Ambient");
    *(int *)(base + 0x5c) = getUniformLocation(program, "u_Diffuse");
    *(int *)(base + 0x64) = getUniformLocation(program, "u_Sampler");
    *(int *)(base + 0x6c) = getUniformLocation(program, "u_FogColor");

    glUseProgram(program);
    glUniform1i(*(int *)(base + 0x44), 0);
}


// ---- TextureAlphaTestShader_890c0.cpp ----

TextureAlphaTestShader::TextureAlphaTestShader()
{
    uint32_t storage[4];
    storage[3] = __stack_chk_guard;

    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    *(char *volatile *)this = g_TextureAlphaTestShader_vtable + 8;
    g_TextureAlphaTestShader_staticDest = g_TextureAlphaTestShader_staticSource;

    String_ctor_cstr((String *)storage, "TextureAlphaTestShader", false);
    string_at(this, 0x0c) = *(String *)storage;
    ((String *)storage)->~String();

    int diff = __stack_chk_guard - storage[3];
    if (diff == 0) {
        return;
    }
    __stack_chk_fail(diff);
}


// ---- _TextureAlphaTestShader_893cc.cpp ----

TextureAlphaTestShader::~TextureAlphaTestShader()
{
    ((ShaderBaseStruct *)this)->~ShaderBaseStruct();
    ::operator delete(this);
}


// ---- UseShader_8914c.cpp ----

void TextureAlphaTestShader::UseShader(bool)
{
    if (g_TextureAlphaTestShader_useAlphaProgram != 0) {
        int program = i32(this, 0x20);
        if (program != 0) {
            glUseProgram(program);
            return;
        }
    }
    glUseProgram(i32(this, 0x04));
}

