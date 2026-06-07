#include "class.h"

extern "C" void glActiveTexture(uint32_t texture);
extern "C" void _ZN11AbyssEngine6Engine16ActivateRefractFBOEv(AbyssEngine::Engine *self);

namespace AbyssEngine {

void BumpShaderRefract::Init(Engine *engine)
{
    int program = ((ShaderBaseStruct *)this)->ES2LoadProgram("BumpShaderRefract.vsh", "BumpShaderRefract.fsh");
    field_i32(this, 0x04) = program;

    field_i32(this, 0x20) = glGetAttribLocation(program, "a0");
    field_i32(this, 0x24) = glGetAttribLocation(field_i32(this, 0x04), "a1");

    field_i32(this, 0x28) = glGetUniformLocation(field_i32(this, 0x04), "u0");
    field_i32(this, 0x2c) = glGetUniformLocation(field_i32(this, 0x04), "u1");
    field_i32(this, 0x30) = glGetUniformLocation(field_i32(this, 0x04), "u2");
    field_i32(this, 0x34) = glGetUniformLocation(field_i32(this, 0x04), "u3");
    field_i32(this, 0x38) = glGetUniformLocation(field_i32(this, 0x04), "u4");
    field_i32(this, 0x40) = glGetUniformLocation(field_i32(this, 0x04), "u5");
    field_i32(this, 0x44) = glGetUniformLocation(field_i32(this, 0x04), "u6");
    field_i32(this, 0x48) = glGetUniformLocation(field_i32(this, 0x04), "u7");

    glActiveTexture(0x84c7);
    _ZN11AbyssEngine6Engine16ActivateRefractFBOEv(engine);
    field_i32(this, 0x3c) = glGetUniformLocation(field_i32(this, 0x04), "u8");

    glUseProgram(field_i32(this, 0x04));
    for (int i = 0; i != 2; i++) {
        int loc = field_i32(this, i * 4 + 0x30);
        if (loc >= 0)
            glUniform1i(loc, i);
    }
    glUniform1i(field_i32(this, 0x3c), 7);
}

} // namespace AbyssEngine
