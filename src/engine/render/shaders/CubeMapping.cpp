#include "gof2/engine/render/shaders/CubeMapping.h"
#include "gof2/engine/render/ShaderBaseStruct.h"
#include "gof2/game/core/String.h"

// ---- Init_907d8.cpp ----
namespace AbyssEngine {

void CubeMapping::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)(this))->ES2LoadProgram("CubeMapping.vsh", "CubeMapping.fsh");
    this->program = program;

    this->aPosition = glGetAttribLocation(program, "a0");
    this->aNormal = glGetAttribLocation(this->program, "a1");
    this->aTexCoord = glGetAttribLocation(this->program, "a2");

    this->uMvp = glGetUniformLocation(this->program, "u0");
    this->uNormalMatrix = glGetUniformLocation(this->program, "u1");
    this->uniform2 = glGetUniformLocation(this->program, "u2");
    this->uniform3 = glGetUniformLocation(this->program, "u3");
    this->uniform4 = glGetUniformLocation(this->program, "u4");
    this->uniform5 = glGetUniformLocation(this->program, "u5");
    this->uniform6 = glGetUniformLocation(this->program, "u6");
    this->uniform7 = glGetUniformLocation(this->program, "u7");
    this->uniform8 = glGetUniformLocation(this->program, "u8");
    this->uniform9 = glGetUniformLocation(this->program, "u9");
    this->uniform10 = glGetUniformLocation(this->program, "u10");
    this->uniform11 = glGetUniformLocation(this->program, "u11");

    glUseProgram(this->program);
    glUniform1i(this->uniform4, 0);
    return glUniform1i(this->uniform5, 1);
}

} // namespace AbyssEngine

// ---- _CubeMapping_90ab0.cpp ----
void _ZN11AbyssEngine11CubeMappingD0Ev(AbyssEngine::CubeMapping *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    ::operator delete(base);
}

// ---- CubeMapping_9074c.cpp ----
namespace AbyssEngine {

CubeMapping::CubeMapping()
{
    new ((AbyssEngine::ShaderBaseStruct *)this) ShaderBaseStruct();

    // install vtable (target adds +8 to the table base)
    this->field_0x0 = (void *)(CubeMapping_vtable + 8);
    CubeMapping_typeInfoDest = CubeMapping_typeInfoSource;

    String name;
    ((String *)(&name))->ctor_char(CubeMapping_name, false);
    ((String *)(&this->name))->assign(&name);
    ((String *)(&name))->dtor();
}

} // namespace AbyssEngine

// ---- SetInActive_90910.cpp ----
namespace AbyssEngine {

void CubeMapping::SetInActive()
{
    glDisableVertexAttribArray(this->aPosition);
    glDisableVertexAttribArray(this->aNormal);
    return glDisableVertexAttribArray(this->aTexCoord);
}

} // namespace AbyssEngine

// ---- UpdateMeshData_90934.cpp ----
namespace AbyssEngine {

void CubeMapping::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    char *eng = (char *)engine;
    char *msh = (char *)mesh;

    if (this->uniformsDirty != 0) {
        glUniform4fv(this->uniform11, 1, (const float *)(eng + 0xd0));
        glUniform4fv(this->uniform7, 1, (const float *)(eng + 0x2cc));
        glUniform4fv(this->uniform8, 1, (const float *)(eng + 0x2fc));
        glUniform4fv(this->uniform9, 1, (const float *)(eng + 0x2e4));
        glUniform1f(this->uniform10, *(float *)(eng + 0x2c8));
        this->uniformsDirty = 0;
    }

    glUniform1f(this->uniform6, *(float *)(eng + 0xcc));
    glUniformMatrix4fv(this->uMvp, 1, 0, (const float *)(eng + 0x104));
    glUniformMatrix3fv(this->uNormalMatrix, 1, 0, (const float *)(eng + 0x204));
    glUniform4f(this->uniform3, *(float *)(eng + 0x330), *(float *)(eng + 0x334),
                *(float *)(eng + 0x338), *(float *)(eng + 0x378));
    glUniform3f(this->uniform2, *(float *)(eng + 0x34c), *(float *)(eng + 0x350),
                *(float *)(eng + 0x354));

    glEnableVertexAttribArray(this->aPosition);
    glEnableVertexAttribArray(this->aTexCoord);
    glEnableVertexAttribArray(this->aNormal);

    if (*(uint8_t *)(msh + 0x5c) == 0) {
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, *(void **)(msh + 0x4));
        if ((*(uint8_t *)msh & 2) != 0)
            glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, *(void **)(msh + 0x8));
        if ((*(uint8_t *)msh & 4) != 0)
            glVertexAttribPointer(this->aNormal, 3, 0x1406, 0, 0, *(void **)(msh + 0x10));
    } else {
        glBindBuffer(0x8892, *(int *)(msh + 0x60));
        glVertexAttribPointer(this->aPosition, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(msh + 0x68));
        glVertexAttribPointer(this->aTexCoord, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, *(int *)(msh + 0x6c));
        glVertexAttribPointer(this->aNormal, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine
