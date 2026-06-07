#include "class.h"

typedef int GetShaderLocation(unsigned int program, const char *name);

extern "C" unsigned int ShaderBaseStruct_ES2LoadProgram(void *self, const char *vertex, const char *fragment);
extern "C" void glUseProgram(unsigned int program);
extern "C" void glUniform1i(int location, int value);

extern "C" __attribute__((visibility("hidden"))) GetShaderLocation *glGetAttribLocation_ptr;
extern "C" __attribute__((visibility("hidden"))) GetShaderLocation *glGetUniformLocation_ptr;

extern "C" void VertexColorShader_Init(AbyssEngine::VertexColorShader *self)
{
    unsigned int program = ShaderBaseStruct_ES2LoadProgram(self, "vtx_color_vs", "vtx_color_fs");
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
