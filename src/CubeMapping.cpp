#include "gof2/CubeMapping.h"

// ---- Init_907d8.cpp ----
namespace AbyssEngine {

void CubeMapping::Init(Engine *)
{
    int program = ShaderBaseStruct_ES2LoadProgram(this, "CubeMapping.vsh", "CubeMapping.fsh");
    this->field_0x4 = program;

    this->field_0x20 = glGetAttribLocation(program, "a0");
    this->field_0x24 = glGetAttribLocation(this->field_0x4, "a1");
    this->field_0x28 = glGetAttribLocation(this->field_0x4, "a2");

    this->field_0x2c = glGetUniformLocation(this->field_0x4, "u0");
    this->field_0x30 = glGetUniformLocation(this->field_0x4, "u1");
    this->field_0x34 = glGetUniformLocation(this->field_0x4, "u2");
    this->field_0x38 = glGetUniformLocation(this->field_0x4, "u3");
    this->field_0x40 = glGetUniformLocation(this->field_0x4, "u4");
    this->field_0x3c = glGetUniformLocation(this->field_0x4, "u5");
    this->field_0x44 = glGetUniformLocation(this->field_0x4, "u6");
    this->field_0x48 = glGetUniformLocation(this->field_0x4, "u7");
    this->field_0x4c = glGetUniformLocation(this->field_0x4, "u8");
    this->field_0x50 = glGetUniformLocation(this->field_0x4, "u9");
    this->field_0x54 = glGetUniformLocation(this->field_0x4, "u10");
    this->field_0x58 = glGetUniformLocation(this->field_0x4, "u11");

    glUseProgram(this->field_0x4);
    glUniform1i(this->field_0x40, 0);
    return glUniform1i(this->field_0x3c, 1);
}

} // namespace AbyssEngine

// ---- _CubeMapping_90ab0.cpp ----
extern "C" void _ZN11AbyssEngine11CubeMappingD0Ev(AbyssEngine::CubeMapping *self)
{
    operator_delete(ShaderBaseStruct_dtor(self));
}

// ---- CubeMapping_9074c.cpp ----
namespace AbyssEngine {

CubeMapping::CubeMapping()
{
    ShaderBaseStruct_ctor(this);

    // install vtable (target adds +8 to the table base)
    this->field_0x0 = (void *)(CubeMapping_vtable + 8);
    CubeMapping_typeInfoDest = CubeMapping_typeInfoSource;

    String name;
    String_ctor_char(&name, CubeMapping_name, false);
    String_assign(&this->field_0xc, &name);
    String_dtor(&name);
}

} // namespace AbyssEngine

// ---- SetInActive_90910.cpp ----
namespace AbyssEngine {

void CubeMapping::SetInActive()
{
    glDisableVertexAttribArray(this->field_0x20);
    glDisableVertexAttribArray(this->field_0x24);
    return glDisableVertexAttribArray(this->field_0x28);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_90934.cpp ----
namespace AbyssEngine {

void CubeMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *eng = (char *)engine;
    char *msh = (char *)mesh;

    if (this->field_0x9 != 0) {
        glUniform4fv(this->field_0x58, 1, (const float *)(eng + 0xd0));
        glUniform4fv(this->field_0x48, 1, (const float *)(eng + 0x2cc));
        glUniform4fv(this->field_0x4c, 1, (const float *)(eng + 0x2fc));
        glUniform4fv(this->field_0x50, 1, (const float *)(eng + 0x2e4));
        glUniform1f(this->field_0x54, *(float *)(eng + 0x2c8));
        this->field_0x9 = 0;
    }

    glUniform1f(this->field_0x44, *(float *)(eng + 0xcc));
    glUniformMatrix4fv(this->field_0x2c, 1, 0, (const float *)(eng + 0x104));
    glUniformMatrix3fv(this->field_0x30, 1, 0, (const float *)(eng + 0x204));
    glUniform4f(this->field_0x38, *(float *)(eng + 0x330), *(float *)(eng + 0x334),
                *(float *)(eng + 0x338), *(float *)(eng + 0x378));
    glUniform3f(this->field_0x34, *(float *)(eng + 0x34c), *(float *)(eng + 0x350),
                *(float *)(eng + 0x354));

    glEnableVertexAttribArray(this->field_0x20);
    glEnableVertexAttribArray(this->field_0x28);
    glEnableVertexAttribArray(this->field_0x24);

    if (*(uint8_t *)(msh + 0x5c) == 0) {
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, *(void **)(msh + 0x4));
        if ((*(uint8_t *)msh & 2) != 0)
            glVertexAttribPointer(this->field_0x28, 2, 0x1406, 0, 0, *(void **)(msh + 0x8));
        if ((*(uint8_t *)msh & 4) != 0)
            glVertexAttribPointer(this->field_0x24, 3, 0x1406, 0, 0, *(void **)(msh + 0x10));
    } else {
        glBindBuffer(0x8892, *(int *)(msh + 0x60));
        glVertexAttribPointer(this->field_0x20, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(msh + 0x68));
        glVertexAttribPointer(this->field_0x28, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(msh + 0x6c));
        glVertexAttribPointer(this->field_0x24, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
