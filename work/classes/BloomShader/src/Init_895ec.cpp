#include "class.h"

extern "C" unsigned int ShaderBaseStruct_ES2LoadProgram(ShaderBaseStruct *self,
                                                        const char *vertex,
                                                        const char *fragment);
extern "C" int glGetAttribLocation(unsigned int program, const char *name);
extern "C" int glGetUniformLocation(unsigned int program, const char *name);
extern "C" void glUseProgram(unsigned int program);
extern "C" void glUniform1i(int location, int value);

namespace AbyssEngine {

void BloomShader::Init(Engine *)
{
    unsigned int (*loadProgram)(ShaderBaseStruct *, const char *, const char *) =
        ShaderBaseStruct_ES2LoadProgram;
    int (*attrib)(unsigned int, const char *) = glGetAttribLocation;
    int (*uniform)(unsigned int, const char *) = glGetUniformLocation;
    void (*useProgram)(unsigned int) = glUseProgram;
    void (*uniform1i)(int, int) = glUniform1i;

    const char *vertex = "BloomShader.vsh";
    const char *positionName = "a_position";
    const char *texCoordName = "a_texCoord";
    const char *matrixName = "u_WorldMatrix";
    const char *samplerName = "s_texture";
    const char *texSizeName = "texSize";

    *(unsigned int *)((char *)this + 0x4) =
        loadProgram((ShaderBaseStruct *)this, vertex, "BloomShaderLuma.fsh");
    *(unsigned int *)((char *)this + 0x20) =
        loadProgram((ShaderBaseStruct *)this, vertex, "BloomShaderDownSample.fsh");
    *(unsigned int *)((char *)this + 0x38) =
        loadProgram((ShaderBaseStruct *)this, vertex, "BloomShaderBlurH.fsh");
    *(unsigned int *)((char *)this + 0x54) =
        loadProgram((ShaderBaseStruct *)this, vertex, "BloomShaderBlurV.fsh");
    *(unsigned int *)((char *)this + 0x74) =
        loadProgram((ShaderBaseStruct *)this, vertex, "BloomShaderFinal.fsh");

    *(int *)((char *)this + 0x8c) = attrib(*(unsigned int *)((char *)this + 0x4), positionName);
    *(int *)((char *)this + 0x94) = attrib(*(unsigned int *)((char *)this + 0x4), texCoordName);
    *(int *)((char *)this + 0x90) = uniform(*(unsigned int *)((char *)this + 0x4), matrixName);
    *(int *)((char *)this + 0x98) = uniform(*(unsigned int *)((char *)this + 0x4), samplerName);
    useProgram(*(unsigned int *)((char *)this + 0x4));
    uniform1i(*(int *)((char *)this + 0x98), 0);

    *(int *)((char *)this + 0x24) = attrib(*(unsigned int *)((char *)this + 0x20), positionName);
    *(int *)((char *)this + 0x2c) = attrib(*(unsigned int *)((char *)this + 0x20), texCoordName);
    *(int *)((char *)this + 0x28) = uniform(*(unsigned int *)((char *)this + 0x20), matrixName);
    *(int *)((char *)this + 0x30) = uniform(*(unsigned int *)((char *)this + 0x20), samplerName);
    useProgram(*(unsigned int *)((char *)this + 0x20));
    uniform1i(*(int *)((char *)this + 0x30), 0);

    *(int *)((char *)this + 0x3c) = attrib(*(unsigned int *)((char *)this + 0x38), positionName);
    *(int *)((char *)this + 0x44) = attrib(*(unsigned int *)((char *)this + 0x38), texCoordName);
    *(int *)((char *)this + 0x40) = uniform(*(unsigned int *)((char *)this + 0x38), matrixName);
    *(int *)((char *)this + 0x48) = uniform(*(unsigned int *)((char *)this + 0x38), samplerName);
    *(int *)((char *)this + 0x4c) = uniform(*(unsigned int *)((char *)this + 0x38), texSizeName);
    useProgram(*(unsigned int *)((char *)this + 0x38));
    uniform1i(*(int *)((char *)this + 0x48), 0);

    *(int *)((char *)this + 0x58) = attrib(*(unsigned int *)((char *)this + 0x54), positionName);
    *(int *)((char *)this + 0x60) = attrib(*(unsigned int *)((char *)this + 0x54), texCoordName);
    *(int *)((char *)this + 0x5c) = uniform(*(unsigned int *)((char *)this + 0x54), matrixName);
    *(int *)((char *)this + 0x64) = uniform(*(unsigned int *)((char *)this + 0x54), samplerName);
    *(int *)((char *)this + 0x68) = uniform(*(unsigned int *)((char *)this + 0x54), texSizeName);
    useProgram(*(unsigned int *)((char *)this + 0x54));
    uniform1i(*(int *)((char *)this + 0x64), 0);

    *(int *)((char *)this + 0x78) = attrib(*(unsigned int *)((char *)this + 0x74), positionName);
    *(int *)((char *)this + 0x80) = attrib(*(unsigned int *)((char *)this + 0x74), texCoordName);
    *(int *)((char *)this + 0x7c) = uniform(*(unsigned int *)((char *)this + 0x74), matrixName);
    *(int *)((char *)this + 0x84) = uniform(*(unsigned int *)((char *)this + 0x74), samplerName);
    *(int *)((char *)this + 0x88) =
        uniform(*(unsigned int *)((char *)this + 0x74), "s_texture_bloom");
    useProgram(*(unsigned int *)((char *)this + 0x74));
    uniform1i(*(int *)((char *)this + 0x84), 0);
    return uniform1i(*(int *)((char *)this + 0x88), 1);
}

} // namespace AbyssEngine
