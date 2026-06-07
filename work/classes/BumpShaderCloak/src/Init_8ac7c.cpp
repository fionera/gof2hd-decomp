#include "class.h"

extern "C" void _ZN11AbyssEngine6Engine18ActivateRefractFBOEv(AbyssEngine::Engine *self);

namespace AbyssEngine {

void BumpShaderCloak::Init(Engine *engine)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpShaderCloak.vsh", "BumpShaderCloak.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a0");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a1");
    field_i32(this, 0x28) = glGetAttribLocation(field_i32(this, 0x04), "a2");
    field_i32(this, 0x2c) = glGetAttribLocation(field_i32(this, 0x04), "a3");
    field_i32(this, 0x30) = glGetAttribLocation(field_i32(this, 0x04), "a4");

    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u0");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u1");
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u2");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u3");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u5");
    field_i32(this, 0x4c) = glGetUniformLocation(field_i32(this, 0x04), "u6");
    field_i32(this, 0x50) = glGetUniformLocation(field_i32(this, 0x04), "u7");
    field_i32(this, 0x58) = glGetUniformLocation(field_i32(this, 0x04), "u8");
    field_i32(this, 0x5c) = glGetUniformLocation(field_i32(this, 0x04), "u9");
    field_i32(this, 0x64) = glGetUniformLocation(field_i32(this, 0x04), "u10");
    field_i32(this, 0x6c) = glGetUniformLocation(field_i32(this, 0x04), "u11");
    field_i32(this, 0x60) = glGetUniformLocation(field_i32(this, 0x04), "u12");
    field_i32(this, 0x68) = glGetUniformLocation(field_i32(this, 0x04), "u13");
    field_i32(this, 0x70) = glGetUniformLocation(field_i32(this, 0x04), "u14");
    field_i32(this, 0x74) = glGetUniformLocation(field_i32(this, 0x04), "u15");
    field_i32(this, 0x78) = glGetUniformLocation(field_i32(this, 0x04), "u16");
    field_i32(this, 0x8c) = glGetUniformLocation(field_i32(this, 0x04), "u17");

    glActiveTexture(0x84c7);
    _ZN11AbyssEngine6Engine18ActivateRefractFBOEv(engine);

    field_i32(this, 0x80) = glGetUniformLocation(field_i32(this, 0x04), "u18");
    field_i32(this, 0x84) = glGetUniformLocation(field_i32(this, 0x04), "u19");
    field_i32(this, 0x90) = glGetUniformLocation(field_i32(this, 0x04), "u20");

    glUseProgram(field_i32(this, 0x04));
    void (*uniform1i)(int, int) = glUniform1i;
    uniform1i(field_i32(this, 0x48), 0);
    uniform1i(field_i32(this, 0x4c), 1);
    uniform1i(field_i32(this, 0x50), 6);
    return uniform1i(field_i32(this, 0x90), 7);
}

} // namespace AbyssEngine
