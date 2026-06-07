#include "class.h"

typedef uint32_t LoadProgramFn(ShaderBaseStruct *, const char *, const char *);
typedef uint32_t LocationFn(uint32_t, const char *);
typedef void UseProgramFn(uint32_t);
typedef void Uniform1iFn(uint32_t, int32_t);

extern "C" __attribute__((visibility("hidden"))) LoadProgramFn *g_GlowPPShader_LoadProgram;
extern "C" __attribute__((visibility("hidden"))) LocationFn *g_GlowPPShader_GetAttribLocation;
extern "C" __attribute__((visibility("hidden"))) LocationFn *g_GlowPPShader_GetUniformLocation;
extern "C" __attribute__((visibility("hidden"))) UseProgramFn *g_GlowPPShader_UseProgram;
extern "C" __attribute__((visibility("hidden"))) Uniform1iFn *g_GlowPPShader_Uniform1i;

extern "C" void GlowPPShader_Init(GlowPPShader *self)
{
    const char *vertex = "GlowPPShader.vert";
    LoadProgramFn *loadProgram = g_GlowPPShader_LoadProgram;
    field_u32(self, 0x20) = loadProgram((ShaderBaseStruct *)self, vertex, "GlowPPShader.copy.frag");
    field_u32(self, 0x40) = loadProgram((ShaderBaseStruct *)self, vertex, "GlowPPShader.blurX.frag");
    field_u32(self, 0x5c) = loadProgram((ShaderBaseStruct *)self, vertex, "GlowPPShader.blurY.frag");
    field_u32(self, 0x78) = loadProgram((ShaderBaseStruct *)self, vertex, "GlowPPShader.combine.frag");

    LocationFn *getAttribLocation = g_GlowPPShader_GetAttribLocation;
    const char *position = "position";
    field_u32(self, 0x24) = getAttribLocation(field_u32(self, 0x20), position);
    const char *texcoord = "texcoord";
    field_u32(self, 0x2c) = getAttribLocation(field_u32(self, 0x20), texcoord);

    LocationFn *getUniformLocation = g_GlowPPShader_GetUniformLocation;
    const char *worldView = "worldView";
    field_u32(self, 0x28) = getUniformLocation(field_u32(self, 0x20), worldView);
    const char *texture = "texture";
    field_u32(self, 0x30) = getUniformLocation(field_u32(self, 0x20), texture);

    UseProgramFn *useProgram = g_GlowPPShader_UseProgram;
    useProgram(field_u32(self, 0x20));
    Uniform1iFn *uniform1i = g_GlowPPShader_Uniform1i;
    uniform1i(field_u32(self, 0x30), 0);

    field_u32(self, 0x44) = getAttribLocation(field_u32(self, 0x40), position);
    field_u32(self, 0x4c) = getAttribLocation(field_u32(self, 0x40), texcoord);
    field_u32(self, 0x48) = getUniformLocation(field_u32(self, 0x40), worldView);
    field_u32(self, 0x50) = getUniformLocation(field_u32(self, 0x40), texture);
    const char *sampleSize = "sampleSize";
    field_u32(self, 0x54) = getUniformLocation(field_u32(self, 0x40), sampleSize);
    useProgram(field_u32(self, 0x40));
    uniform1i(field_u32(self, 0x50), 0);

    field_u32(self, 0x60) = getAttribLocation(field_u32(self, 0x5c), position);
    field_u32(self, 0x68) = getAttribLocation(field_u32(self, 0x5c), texcoord);
    field_u32(self, 0x64) = getUniformLocation(field_u32(self, 0x5c), worldView);
    field_u32(self, 0x6c) = getUniformLocation(field_u32(self, 0x5c), texture);
    field_u32(self, 0x70) = getUniformLocation(field_u32(self, 0x5c), sampleSize);
    useProgram(field_u32(self, 0x5c));
    uniform1i(field_u32(self, 0x6c), 0);

    field_u32(self, 0x7c) = getAttribLocation(field_u32(self, 0x78), position);
    field_u32(self, 0x84) = getAttribLocation(field_u32(self, 0x78), texcoord);
    field_u32(self, 0x80) = getUniformLocation(field_u32(self, 0x78), worldView);
    field_u32(self, 0x88) = getUniformLocation(field_u32(self, 0x78), texture);
    const char *texture2 = "texture2";
    field_u32(self, 0x8c) = getUniformLocation(field_u32(self, 0x78), texture2);
    useProgram(field_u32(self, 0x78));
    uniform1i(field_u32(self, 0x88), 0);
    return uniform1i(field_u32(self, 0x8c), 1);
}
