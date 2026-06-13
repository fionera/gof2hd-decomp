#include "gof2/engine/render/shaders/CubeNormalMapping.h"
#include "gof2/engine/render/Mesh.h"

namespace AbyssEngine {

void CubeNormalMapping::UpdateMeshData(Mesh *meshArg, Engine *engine)
{
    AbyssEngine::Mesh *mesh = (AbyssEngine::Mesh *)meshArg;
    char *e = (char *)engine;
    if (this->dirty != 0) {
        glUniform4fv(this->uniformU12, 1, (float *)(e + 0xd0));
        glUniform4fv(this->uniformU8, 1, (float *)(e + 0x2cc));
        glUniform4fv(this->uniformU9, 1, (float *)(e + 0x2fc));
        glUniform4fv(this->uniformU10, 1, (float *)(e + 0x2e4));
        glUniform1f(this->uniformU11, *(float *)(e + 0x2c8));
        this->dirty = 0;
    }

    glUniform1f(this->uniformU7, *(float *)(e + 0xcc));
    glUniformMatrix4fv(this->uniformU0, 1, 0, e + 0x104);
    glUniformMatrix3fv(this->uniformU1, 1, 0, e + 0x204);
    glUniform3f(this->uniformU3, *(float *)(e + 0x330), *(float *)(e + 0x334),
                *(float *)(e + 0x338));
    glUniform3f(this->uniformU2, *(float *)(e + 0x34c), *(float *)(e + 0x350),
                *(float *)(e + 0x354));

    glEnableVertexAttribArray(this->attribA0);
    glEnableVertexAttribArray(this->attribA2);
    glEnableVertexAttribArray(this->attribA1);
    glEnableVertexAttribArray(this->attribA3);
    glEnableVertexAttribArray(this->attribA4);

    if (mesh->field_0x5c == 0) {
        glVertexAttribPointer(this->attribA0, 3, 0x1406, 0, 0, mesh->field_0x4);
        if ((mesh->field_0x0 & 2) != 0)
            glVertexAttribPointer(this->attribA2, 2, 0x1406, 0, 0, mesh->field_0x8);
        if ((mesh->field_0x0 & 4) != 0)
            glVertexAttribPointer(this->attribA1, 3, 0x1406, 0, 0, mesh->field_0x10);
        if (this->attribA3 >= 0)
            glVertexAttribPointer(this->attribA3, 3, 0x1406, 0, 0, *(void **)&mesh->field_0x14);
        if (this->attribA4 >= 0)
            glVertexAttribPointer(this->attribA4, 3, 0x1406, 0, 0, *(void **)&mesh->field_0x18);
    } else {
        glBindBuffer(0x8892, mesh->field_0x60);
        glVertexAttribPointer(this->attribA0, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x68);
        glVertexAttribPointer(this->attribA2, 2, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x6c);
        glVertexAttribPointer(this->attribA1, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x70);
        glVertexAttribPointer(this->attribA3, 3, 0x1406, 0, 0, 0);
        glBindBuffer(0x8892, mesh->field_0x74);
        glVertexAttribPointer(this->attribA4, 3, 0x1406, 0, 0, 0);
    }
}

} // namespace AbyssEngine

void _ZN11AbyssEngine17CubeNormalMappingD0Ev(
    AbyssEngine::CubeNormalMapping *self)
{
    AbyssEngine::ShaderBaseStruct *base = (AbyssEngine::ShaderBaseStruct *)self;
    base->~ShaderBaseStruct();
    operator delete(base);
}

namespace AbyssEngine {

void CubeNormalMapping::Init(Engine *)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("CubeNormalMapping.vsh", "CubeNormalMapping.fsh");
    this->program = program;

    this->attribA0 = glGetAttribLocation(program, "a0");
    this->attribA1 = glGetAttribLocation(this->program, "a1");
    this->attribA2 = glGetAttribLocation(this->program, "a2");
    this->attribA3 = glGetAttribLocation(this->program, "a3");
    this->attribA4 = glGetAttribLocation(this->program, "a4");

    this->uniformU0 = glGetUniformLocation(this->program, "u0");
    this->uniformU1 = glGetUniformLocation(this->program, "u1");
    this->uniformU2 = glGetUniformLocation(this->program, "u2");
    this->uniformU3 = glGetUniformLocation(this->program, "u3");
    this->uniformU4 = glGetUniformLocation(this->program, "u4");
    this->uniformU5 = glGetUniformLocation(this->program, "u5");
    this->uniformU6 = glGetUniformLocation(this->program, "u6");
    this->uniformU7 = glGetUniformLocation(this->program, "u7");
    this->uniformU8 = glGetUniformLocation(this->program, "u8");
    this->uniformU9 = glGetUniformLocation(this->program, "u9");
    this->uniformU10 = glGetUniformLocation(this->program, "u10");
    this->uniformU11 = glGetUniformLocation(this->program, "u11");
    this->uniformU12 = glGetUniformLocation(this->program, "u12");

    glUseProgram(this->program);
    glUniform1i(this->uniformU4, 0);
    glUniform1i(this->uniformU6, 1);
    glUniform1i(this->uniformU5, 2);
}

} // namespace AbyssEngine

extern "C" {
extern void *_ZTIN11AbyssEngine17CubeNormalMappingE;
extern void *CubeNormalMapping_typeInfoSlot;
extern const char CubeNormalMapping_name[];
extern char _ZTVN11AbyssEngine17CubeNormalMappingE[];
}

namespace AbyssEngine {

CubeNormalMapping::CubeNormalMapping()
{
    new ((ShaderBaseStruct *)this) ShaderBaseStruct();
    *(void **)this = (void *)(_ZTVN11AbyssEngine17CubeNormalMappingE + 8);
    CubeNormalMapping_typeInfoSlot = &_ZTIN11AbyssEngine17CubeNormalMappingE;
    String tmp;
    tmp.s = u"CubeNormalMapping";
    this->name = tmp;
}

} // namespace AbyssEngine

namespace AbyssEngine {

void CubeNormalMapping::SetInActive()
{
    glDisableVertexAttribArray(this->attribA0);
    glDisableVertexAttribArray(this->attribA1);
    glDisableVertexAttribArray(this->attribA2);
    glDisableVertexAttribArray(this->attribA3);
    glDisableVertexAttribArray(this->attribA4);
}

} // namespace AbyssEngine
