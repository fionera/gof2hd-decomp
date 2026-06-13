#include "gof2/engine/render/shaders/VertexColorAlphaTextureShader.h"
#include "gof2/game/core/String.h"

// Engine / Mesh are opaque here; their fields are accessed by raw offset since
// their real layouts live in other (non-batch) classes.
static inline int &eng_i32(void *self, uint32_t offset) { return *(int *)((char *)self + offset); }
static inline uint8_t &eng_u8(void *self, uint32_t offset) { return *(uint8_t *)((char *)self + offset); }
static inline float &eng_f32(void *self, uint32_t offset) { return *(float *)((char *)self + offset); }
static inline void *eng_ptr(void *self, uint32_t offset) { return *(void **)((char *)self + offset); }

// ---- Init_91b6c.cpp ----
namespace AbyssEngine {

void VertexColorAlphaTextureShader::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("VCATShader.vsh", "VCATShader.fsh");
    this->program = program;

    this->attrib0 = glGetAttribLocation(program, "a0");
    this->attrib1 = glGetAttribLocation(this->program, "a1");
    this->attrib2 = glGetAttribLocation(this->program, "a2");
    this->attrib3 = glGetAttribLocation(this->program, "a3");
    this->attrib4 = glGetAttribLocation(this->program, "a4");
    this->attrib5 = glGetAttribLocation(this->program, "a5");

    this->uniform0 = glGetUniformLocation(this->program, "u0");
    this->uniform1 = glGetUniformLocation(this->program, "u1");
    this->uniform2 = glGetUniformLocation(this->program, "u2");
    this->uniform3 = glGetUniformLocation(this->program, "u3");
    this->uniform4 = glGetUniformLocation(this->program, "u4");
    this->uniform5 = glGetUniformLocation(this->program, "u5");
    this->uniform6 = glGetUniformLocation(this->program, "u6");
    this->uniform7 = glGetUniformLocation(this->program, "u7");
    this->uniform8 = glGetUniformLocation(this->program, "u8");

    glUseProgram(this->program);
    return glUniform1i(this->uniform4, 0);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_91ce4.cpp ----
namespace AbyssEngine {

void VertexColorAlphaTextureShader::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (this->uniform0 >= 0)
        glUniformMatrix4fv(this->uniform0, 1, 0, (char *)engine + 0x104);
    if (this->uniform1 >= 0)
        glUniformMatrix3fv(this->uniform1, 1, 0, (char *)engine + 0x204);

    if (this->dirty != 0) {
        if (this->uniform2 >= 0)
            glUniform3f(this->uniform2, eng_f32(engine, 0x330), eng_f32(engine, 0x334),
                        eng_f32(engine, 0x338));
        if (this->uniform3 >= 0)
            glUniform3f(this->uniform3, eng_f32(engine, 0x34c), eng_f32(engine, 0x350),
                        eng_f32(engine, 0x354));
        if (this->uniform5 >= 0)
            glUniform4fv(this->uniform5, 1, (float *)((char *)engine + 0xd0));
        if (this->uniform6 >= 0)
            glUniform4fv(this->uniform6, 1, (float *)((char *)engine + 0x2a8));
        if (this->uniform7 >= 0)
            glUniform4fv(this->uniform7, 1, (float *)((char *)engine + 0x298));
        if (this->uniform8 >= 0)
            glUniform4fv(this->uniform8, 1, (float *)((char *)engine + 0x2b8));
        this->dirty = 0;
    }

    if (this->attrib1 >= 0)
        glEnableVertexAttribArray(this->attrib1);
    if (this->attrib2 >= 0)
        glEnableVertexAttribArray(this->attrib2);
    if (this->attrib3 >= 0)
        glEnableVertexAttribArray(this->attrib3);
    if (this->attrib4 >= 0)
        glEnableVertexAttribArray(this->attrib4);
    if (this->attrib5 >= 0)
        glEnableVertexAttribArray(this->attrib5);
    if (this->attrib0 >= 0)
        glEnableVertexAttribArray(this->attrib0);

    if (eng_u8(mesh, 0x5c) == 0) {
        if (this->attrib1 >= 0)
            glVertexAttribPointer(this->attrib1, 3, 0x1406, 0, 0, eng_ptr(mesh, 0x4));
        if (this->attrib2 >= 0)
            glVertexAttribPointer(this->attrib2, 2, 0x1406, 0, 0, eng_ptr(mesh, 0x8));
        if (this->attrib3 >= 0)
            glVertexAttribPointer(this->attrib3, 3, 0x1406, 0, 0, eng_ptr(mesh, 0x10));
        if (this->attrib4 >= 0)
            glVertexAttribPointer(this->attrib4, 3, 0x1406, 0, 0, eng_ptr(mesh, 0x14));
        if (this->attrib5 >= 0)
            glVertexAttribPointer(this->attrib5, 3, 0x1406, 0, 0, eng_ptr(mesh, 0x18));
        if (this->attrib0 >= 0)
            glVertexAttribPointer(this->attrib0, 4, 0x1406, 0, 0, eng_ptr(mesh, 0xc));
    } else {
        void (*bindBuffer)(uint32_t, uint32_t) = glBindBuffer;
        void (*vertexAttribPointer)(uint32_t, int, uint32_t, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, eng_i32(mesh, 0x60));
        vertexAttribPointer(this->attrib1, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, eng_i32(mesh, 0x68));
        vertexAttribPointer(this->attrib2, 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, eng_i32(mesh, 0x6c));
        vertexAttribPointer(this->attrib3, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, eng_i32(mesh, 0x70));
        vertexAttribPointer(this->attrib4, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, eng_i32(mesh, 0x74));
        vertexAttribPointer(this->attrib5, 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, eng_i32(mesh, 0x78));
        vertexAttribPointer(this->attrib0, 4, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

// ---- SetInActive_91c9c.cpp ----
namespace AbyssEngine {

void VertexColorAlphaTextureShader::SetInActive()
{
    int loc;
    loc = this->attrib1;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->attrib2;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->attrib3;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->attrib4;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->attrib5;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
    loc = this->attrib0;
    if (loc >= 0)
        glDisableVertexAttribArray(loc);
}

} // namespace AbyssEngine

// ---- _VertexColorAlphaTextureShader_91f28.cpp ----
void _ZN11AbyssEngine29VertexColorAlphaTextureShaderD0Ev(
    AbyssEngine::VertexColorAlphaTextureShader *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

// ---- VertexColorAlphaTextureShader_91ae0.cpp ----
namespace AbyssEngine {

// Global shader-index counter copied into a destination slot during construction.
__attribute__((visibility("hidden"))) extern int g_shaderIndexSrc;
__attribute__((visibility("hidden"))) extern int g_shaderIndexDst;

VertexColorAlphaTextureShader::VertexColorAlphaTextureShader()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();

    // Install the derived vtable (base address + 8 -> first virtual slot).
    this->vtable = (void *)(_ZTVN11AbyssEngine29VertexColorAlphaTextureShaderE + 8);

    // Publish the current shader index.
    g_shaderIndexDst = g_shaderIndexSrc;

    // Store the shader resource name in the embedded String at +0x0c.
    String name;
    ((String *)(&name))->ctor_char("VCATShader", false);
    ((String *)(&this->name))->assign(&name);
    ((String *)(&name))->dtor();
}

} // namespace AbyssEngine
