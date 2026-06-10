#ifndef GOF2_BLURSHADER_H
#define GOF2_BLURSHADER_H
#include "gof2/common.h"
#include "gof2/Mesh.h"
#include "gof2/Engine.h"
#include <new>
// Galaxy on Fire 2 -- AbyssEngine::BlurShader (Android libgof2hdaa.so, armv7 Thumb).
// GLES2 radial/box blur post-process shader. Derives from ShaderBaseStruct: program handle at
// 0x24, attribute/uniform locations at 0x28..0x50, tuning floats at 0x58/0x5c, dirty flag at 0x9.
//
// Engine is the real gof2/Engine.h type (it self-compiles now); BlurShader touches a handful of its
// fields/methods (field_0x30/0x380.., GetDisplayWidth/Height, DrawQuad, SetWorldViewMatrix).

static inline uint32_t stack_guard_delta(uint32_t saved, uint32_t current)
{
    return current - saved;
}

namespace AbyssEngine {

// AbyssEngine::BlurShader — derives from ShaderBaseStruct (shared shader storage layout).
// Engine/Mesh/FBOContainer are the global (gof2/fwd.h + gof2/Mesh.h) types; qualified with :: so
// name lookup does not resolve them to the AbyssEngine-namespace forward declarations.
struct BlurShader {
    void Init(::Engine *engine);
    void SetInActive();
    void UpdateMeshData(AbyssEngine::Mesh *mesh, ::Engine *engine);
    void RenderEffect(::FBOContainer *fbo, ::FBOContainer **target, ::Engine *engine,
                      float amount, Vector vector);
    BlurShader();
    ~BlurShader();

    char     pad_0x0[0x9];
    uint8_t  field_0x9;                 // +0x9  dirty flag
    char     pad_0xa[0x24 - 0xa];
    unsigned int field_0x24;            // +0x24 program handle
    int      field_0x28;                // +0x28 a_position
    int      field_0x2c;                // +0x2c u_mvpMatrix
    int      field_0x30;                // +0x30 a_texCoord
    int      field_0x34;                // +0x34 s_texture
    int      field_0x38;                // +0x38 u_texelSize
    int      field_0x3c;                // +0x3c u_blurAmount
    int      field_0x40;                // +0x40 u_strength
    int      field_0x44;                // +0x44 u_center
    unsigned int field_0x48;            // +0x48 vertex attrib slot
    char     pad_0x4c[0x50 - 0x4c];
    unsigned int field_0x50;            // +0x50 texcoord attrib slot
    char     pad_0x54[0x58 - 0x54];
    float    field_0x58;                // +0x58 strength
    float    field_0x5c;                // +0x5c blur scale
};

} // namespace AbyssEngine

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

#endif
