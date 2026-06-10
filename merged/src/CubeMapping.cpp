#include "CubeMapping.h"




// ---- Init_907d8.cpp ----
namespace AbyssEngine {

void CubeMapping::Init(Engine *)
{
    int program = ShaderBaseStruct_ES2LoadProgram(this, "CubeMapping.vsh", "CubeMapping.fsh");
    i32(this, 0x04) = program;

    i32(this, 0x20) = glGetAttribLocation(program, "a0");
    i32(this, 0x24) = glGetAttribLocation(i32(this, 0x04), "a1");
    i32(this, 0x28) = glGetAttribLocation(i32(this, 0x04), "a2");

    i32(this, 0x2c) = glGetUniformLocation(i32(this, 0x04), "u0");
    i32(this, 0x30) = glGetUniformLocation(i32(this, 0x04), "u1");
    i32(this, 0x34) = glGetUniformLocation(i32(this, 0x04), "u2");
    i32(this, 0x38) = glGetUniformLocation(i32(this, 0x04), "u3");
    i32(this, 0x40) = glGetUniformLocation(i32(this, 0x04), "u4");
    i32(this, 0x3c) = glGetUniformLocation(i32(this, 0x04), "u5");
    i32(this, 0x44) = glGetUniformLocation(i32(this, 0x04), "u6");
    i32(this, 0x48) = glGetUniformLocation(i32(this, 0x04), "u7");
    i32(this, 0x4c) = glGetUniformLocation(i32(this, 0x04), "u8");
    i32(this, 0x50) = glGetUniformLocation(i32(this, 0x04), "u9");
    i32(this, 0x54) = glGetUniformLocation(i32(this, 0x04), "u10");
    i32(this, 0x58) = glGetUniformLocation(i32(this, 0x04), "u11");

    glUseProgram(i32(this, 0x04));
    glUniform1i(i32(this, 0x40), 0);
    return glUniform1i(i32(this, 0x3c), 1);
}

} // namespace AbyssEngine

// ---- _CubeMapping_90ab0.cpp ----
extern "C" void _ZN11AbyssEngine11CubeMappingD0Ev(CubeMapping *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}

// ---- CubeMapping_9074c.cpp ----
namespace AbyssEngine {

CubeMapping::CubeMapping()
{
    ShaderBaseStruct_ctor(this);
    *(int *)this = (int)(CubeMapping_vtable) + 8;
    CubeMapping_typeInfoDest = CubeMapping_typeInfoSource;
    String tmp(CubeMapping_name, false);
    *(String *)((char *)this + 0xc) = tmp;
}

} // namespace AbyssEngine

// ---- SetInActive_90910.cpp ----
namespace AbyssEngine {

void CubeMapping::SetInActive()
{
    glDisableVertexAttribArray(i32(this, 0x20));
    glDisableVertexAttribArray(i32(this, 0x24));
    return glDisableVertexAttribArray(i32(this, 0x28));
}

} // namespace AbyssEngine

// ---- UpdateMeshData_90934.cpp ----
namespace AbyssEngine {

void CubeMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (u8(this, 0x9) != 0) {
        void (*uniform4fv)(int, int, const float *) = glUniform4fv;
        uniform4fv(i32(this, 0x58), 1, (float *)((char *)engine + 0xd0));
        uniform4fv(i32(this, 0x48), 1, (float *)((char *)engine + 0x2cc));
        uniform4fv(i32(this, 0x4c), 1, (float *)((char *)engine + 0x2fc));
        uniform4fv(i32(this, 0x50), 1, (float *)((char *)engine + 0x2e4));
        glUniform1f(i32(this, 0x54), f32(engine, 0x2c8));
        u8(this, 0x9) = 0;
    }

    glUniform1f(i32(this, 0x44), f32(engine, 0xcc));
    glUniformMatrix4fv(i32(this, 0x2c), 1, 0, (float *)((char *)engine + 0x104));
    glUniformMatrix3fv(i32(this, 0x30), 1, 0, (float *)((char *)engine + 0x204));
    glUniform4f(i32(this, 0x38), f32(engine, 0x330), f32(engine, 0x334), f32(engine, 0x338),
                f32(engine, 0x378));
    glUniform3f(i32(this, 0x34), f32(engine, 0x34c), f32(engine, 0x350), f32(engine, 0x354));

    void (*enableVertexAttribArray)(int) = glEnableVertexAttribArray;
    enableVertexAttribArray(i32(this, 0x20));
    enableVertexAttribArray(i32(this, 0x28));
    enableVertexAttribArray(i32(this, 0x24));

    if (u8(mesh, 0x5c) == 0) {
        glVertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0,
                              *(void **)((char *)mesh + 0x4));
        if ((u8(mesh, 0x0) & 2) != 0)
            glVertexAttribPointer(i32(this, 0x28), 2, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x8));
        if ((u8(mesh, 0x0) & 4) != 0)
            glVertexAttribPointer(i32(this, 0x24), 3, 0x1406, 0, 0,
                                  *(void **)((char *)mesh + 0x10));
    } else {
        void (*bindBuffer)(unsigned int, unsigned int) = glBindBuffer;
        void (*vertexAttribPointer)(int, int, unsigned int, uint8_t, int, const void *) =
            glVertexAttribPointer;
        bindBuffer(0x8892, i32(mesh, 0x60));
        vertexAttribPointer(i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(mesh, 0x68));
        vertexAttribPointer(i32(this, 0x28), 2, 0x1406, 0, 0, 0);
        bindBuffer(0x8892, i32(mesh, 0x6c));
        vertexAttribPointer(i32(this, 0x24), 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
