#include "gof2/engine/render/shaders/VertexColorShader.h"
#include "gof2/engine/render/ShaderBaseStruct.h"
#include "gof2/game/core/String.h"


extern "C" void glDisableVertexAttribArray(unsigned int index);
extern "C" void glUseProgram(unsigned int program);
extern "C" void glUniform1i(int location, int value);
extern "C" __attribute__((visibility("hidden"))) GetShaderLocation *glGetAttribLocation_ptr;
extern "C" __attribute__((visibility("hidden"))) GetShaderLocation *glGetUniformLocation_ptr;
extern "C" char g_VertexColorShader_vtable[];
extern "C" __attribute__((visibility("hidden"))) int *g_VertexColorShader_typeinfo_src;
extern "C" __attribute__((visibility("hidden"))) int *g_VertexColorShader_typeinfo_dst;
extern "C" void glUniformMatrix4fv(int location, int count, bool transpose, const void *value);
extern "C" void glUniformMatrix3fv(int location, int count, bool transpose, const void *value);
extern "C" void glUniform3f(int location, uint32_t x, uint32_t y, uint32_t z);
extern "C" void glUniform4fv(int location, int count, const void *value);
extern "C" void glEnableVertexAttribArray(unsigned int index);
extern "C" __attribute__((visibility("hidden"))) BindBuffer *glBindBuffer_ptr;
extern "C" __attribute__((visibility("hidden"))) VertexAttribPointer *glVertexAttribPointer_ptr;

// ---- SetInActive_9032c.cpp ----
void VertexColorShader_SetInActive(AbyssEngine::VertexColorShader *self)
{
    if (ae_i32(self, 0x20) >= 0) {
        glDisableVertexAttribArray(ae_u32(self, 0x20));
    }
    if (ae_i32(self, 0x24) >= 0) {
        glDisableVertexAttribArray(ae_u32(self, 0x24));
    }
    if (ae_i32(self, 0x28) >= 0) {
        glDisableVertexAttribArray(ae_u32(self, 0x28));
    }
    if (ae_i32(self, 0x2c) >= 0) {
        glDisableVertexAttribArray(ae_u32(self, 0x2c));
    }
    if (ae_i32(self, 0x30) >= 0) {
        glDisableVertexAttribArray(ae_u32(self, 0x30));
    }
    if (ae_i32(self, 0x34) >= 0) {
        return glDisableVertexAttribArray(ae_u32(self, 0x34));
    }
}

// ---- Init_901fc.cpp ----
void VertexColorShader_Init(AbyssEngine::VertexColorShader *self)
{
    unsigned int program = ((AbyssEngine::ShaderBaseStruct *)(self))->ES2LoadProgram("vtx_color_vs", "vtx_color_fs");
    ae_u32(self, 4) = program;

    GetShaderLocation *getAttribLocation = glGetAttribLocation_ptr;
    ae_i32(self, 0x34) = getAttribLocation(program, "color");
    ae_i32(self, 0x20) = getAttribLocation(ae_u32(self, 4), "position");
    ae_i32(self, 0x24) = getAttribLocation(ae_u32(self, 4), "texcoord");
    ae_i32(self, 0x28) = getAttribLocation(ae_u32(self, 4), "normal");
    ae_i32(self, 0x2c) = getAttribLocation(ae_u32(self, 4), "tangent");
    ae_i32(self, 0x30) = getAttribLocation(ae_u32(self, 4), "binormal");
    GetShaderLocation *getUniformLocation = glGetUniformLocation_ptr;
    ae_i32(self, 0x38) = getUniformLocation(ae_u32(self, 4), "worldViewProj");
    ae_i32(self, 0x3c) = getUniformLocation(ae_u32(self, 4), "normalMatrix");
    ae_i32(self, 0x40) = getUniformLocation(ae_u32(self, 4), "lightDir");
    ae_i32(self, 0x44) = getUniformLocation(ae_u32(self, 4), "lightColor");
    ae_i32(self, 0x48) = getUniformLocation(ae_u32(self, 4), "ambientColor");
    ae_i32(self, 0x4c) = getUniformLocation(ae_u32(self, 4), "materialDiffuse");
    ae_i32(self, 0x50) = getUniformLocation(ae_u32(self, 4), "materialAmbient");
    ae_i32(self, 0x54) = getUniformLocation(ae_u32(self, 4), "materialSpecular");
    ae_i32(self, 0x58) = getUniformLocation(ae_u32(self, 4), "materialShininess");

    glUseProgram(ae_u32(self, 4));
    return glUniform1i(ae_i32(self, 0x48), 0);
}

// ---- VertexColorShader_90170.cpp ----
namespace AbyssEngine {

VertexColorShader::VertexColorShader()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    *(void * volatile *)this = g_VertexColorShader_vtable + 8;
    *g_VertexColorShader_typeinfo_dst = *g_VertexColorShader_typeinfo_src;

    // name = String("VertexColorShader", false); this->name = name; ~name.
    String name;
    ((String *)(&name))->ctor_char("VertexColorShader", false);
    ((String *)(&this->name))->assign(&name);
    ((String *)(&name))->dtor();
}

} // namespace AbyssEngine

// ---- _VertexColorShader_905b8.cpp ----
namespace AbyssEngine {

VertexColorShader::~VertexColorShader()
{
    ((ShaderBaseStruct *)this)->~ShaderBaseStruct();
}

} // namespace AbyssEngine

// ---- UpdateMeshData_90374.cpp ----
extern "C" void glVertexAttribPointer(unsigned int index, int size, unsigned int type, bool normalized,
                                      int stride, const void *pointer);


void VertexColorShader_UpdateMeshData(AbyssEngine::VertexColorShader *self,
                                                  AbyssEngine::Mesh *mesh,
                                                  AbyssEngine::Engine *engine)
{
    if (ae_i32(self, 0x38) >= 0) {
        glUniformMatrix4fv(ae_i32(self, 0x38), 1, false, (char *)engine + 0x104);
    }
    if (ae_i32(self, 0x3c) >= 0) {
        glUniformMatrix3fv(ae_i32(self, 0x3c), 1, false, (char *)engine + 0x204);
    }

    if (ae_u8(self, 9) != 0) {
        if (ae_i32(self, 0x40) >= 0) {
            uint32_t *lightDir = (uint32_t *)((char *)engine + 0x330);
            glUniform3f(ae_i32(self, 0x40), lightDir[0], lightDir[1], lightDir[2]);
        }
        if (ae_i32(self, 0x44) >= 0) {
            uint32_t *lightColor = (uint32_t *)((char *)engine + 0x34c);
            glUniform3f(ae_i32(self, 0x44), lightColor[0], lightColor[1], lightColor[2]);
        }
        if (ae_i32(self, 0x4c) >= 0) {
            glUniform4fv(ae_i32(self, 0x4c), 1, (char *)engine + 0xd0);
        }
        if (ae_i32(self, 0x50) >= 0) {
            glUniform4fv(ae_i32(self, 0x50), 1, (char *)engine + 0x2a8);
        }
        if (ae_i32(self, 0x54) >= 0) {
            glUniform4fv(ae_i32(self, 0x54), 1, (char *)engine + 0x298);
        }
        if (ae_i32(self, 0x58) >= 0) {
            glUniform4fv(ae_i32(self, 0x58), 1, (char *)engine + 0x2b8);
        }
        ae_u8(self, 9) = 0;
    }

    if (ae_i32(self, 0x20) >= 0) {
        glEnableVertexAttribArray(ae_u32(self, 0x20));
    }
    if (ae_i32(self, 0x24) >= 0) {
        glEnableVertexAttribArray(ae_u32(self, 0x24));
    }
    if (ae_i32(self, 0x28) >= 0) {
        glEnableVertexAttribArray(ae_u32(self, 0x28));
    }
    if (ae_i32(self, 0x2c) >= 0) {
        glEnableVertexAttribArray(ae_u32(self, 0x2c));
    }
    if (ae_i32(self, 0x30) >= 0) {
        glEnableVertexAttribArray(ae_u32(self, 0x30));
    }
    if (ae_i32(self, 0x34) >= 0) {
        glEnableVertexAttribArray(ae_u32(self, 0x34));
    }

    if (ae_u8(mesh, 0x5c) != 0) {
        BindBuffer *bindBuffer = glBindBuffer_ptr;
        bindBuffer(0x8892, ae_u32(mesh, 0x60));
        VertexAttribPointer *vertexAttribPointer = glVertexAttribPointer_ptr;
        vertexAttribPointer(ae_u32(self, 0x20), 3, 0x1406, false, 0, 0);
        bindBuffer(0x8892, ae_u32(mesh, 0x68));
        vertexAttribPointer(ae_u32(self, 0x24), 2, 0x1406, false, 0, 0);
        bindBuffer(0x8892, ae_u32(mesh, 0x6c));
        vertexAttribPointer(ae_u32(self, 0x28), 3, 0x1406, false, 0, 0);
        bindBuffer(0x8892, ae_u32(mesh, 0x70));
        vertexAttribPointer(ae_u32(self, 0x2c), 3, 0x1406, false, 0, 0);
        bindBuffer(0x8892, ae_u32(mesh, 0x74));
        vertexAttribPointer(ae_u32(self, 0x30), 3, 0x1406, false, 0, 0);
        bindBuffer(0x8892, ae_u32(mesh, 0x78));
        return vertexAttribPointer(ae_u32(self, 0x34), 4, 0x1406, false, 0, 0);
    }

    if (ae_i32(self, 0x20) >= 0) {
        glVertexAttribPointer(ae_u32(self, 0x20), 3, 0x1406, false, 0, ae_ptr(mesh, 4));
    }
    if (ae_i32(self, 0x24) >= 0) {
        glVertexAttribPointer(ae_u32(self, 0x24), 2, 0x1406, false, 0, ae_ptr(mesh, 8));
    }
    if (ae_i32(self, 0x28) >= 0) {
        glVertexAttribPointer(ae_u32(self, 0x28), 3, 0x1406, false, 0, ae_ptr(mesh, 0x10));
    }
    if (ae_i32(self, 0x2c) >= 0) {
        glVertexAttribPointer(ae_u32(self, 0x2c), 3, 0x1406, false, 0, ae_ptr(mesh, 0x14));
    }
    if (ae_i32(self, 0x30) >= 0) {
        glVertexAttribPointer(ae_u32(self, 0x30), 3, 0x1406, false, 0, ae_ptr(mesh, 0x18));
    }
    if (ae_i32(self, 0x34) >= 0) {
        glVertexAttribPointer(ae_u32(self, 0x34), 4, 0x1406, false, 0, ae_ptr(mesh, 0x0c));
    }
}
