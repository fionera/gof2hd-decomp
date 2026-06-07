#include "class.h"

extern "C" unsigned int ShaderBaseStruct_ES2LoadProgram(ShaderBaseStruct *self,
                                                        const char *vertex,
                                                        const char *fragment);
extern "C" int glGetAttribLocation(unsigned int program, const char *name);
extern "C" int glGetUniformLocation(unsigned int program, const char *name);
extern "C" void glUseProgram(unsigned int program);
extern "C" void glUniform1i(int location, int value);

namespace AbyssEngine {

void BlurShader::Init(Engine *)
{
    unsigned int program = ShaderBaseStruct_ES2LoadProgram((ShaderBaseStruct *)this,
                                                           "BlurShader.vsh",
                                                           "BlurShader.fsh");
    *(unsigned int *)((char *)this + 0x24) = program;
    *(int *)((char *)this + 0x28) = glGetAttribLocation(program, "a_position");
    *(int *)((char *)this + 0x30) =
        glGetAttribLocation(*(unsigned int *)((char *)this + 0x24), "a_texCoord");

    int (*uniform)(unsigned int, const char *) = glGetUniformLocation;
    *(int *)((char *)this + 0x2c) =
        uniform(*(unsigned int *)((char *)this + 0x24), "u_mvpMatrix");
    *(int *)((char *)this + 0x34) =
        uniform(*(unsigned int *)((char *)this + 0x24), "s_texture");
    *(int *)((char *)this + 0x38) =
        uniform(*(unsigned int *)((char *)this + 0x24), "u_texelSize");
    *(int *)((char *)this + 0x44) =
        uniform(*(unsigned int *)((char *)this + 0x24), "u_center");
    *(int *)((char *)this + 0x3c) =
        uniform(*(unsigned int *)((char *)this + 0x24), "u_blurAmount");
    *(int *)((char *)this + 0x40) =
        uniform(*(unsigned int *)((char *)this + 0x24), "u_strength");
    glUseProgram(*(unsigned int *)((char *)this + 0x24));
    return glUniform1i(*(int *)((char *)this + 0x34), 0);
}

} // namespace AbyssEngine
