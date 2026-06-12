#ifndef GOF2_BLOOMSHADER_H
#define GOF2_BLOOMSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

struct FBOContainer;
struct Mesh;

// AbyssEngine::ShaderBaseStruct base layout used by BloomShader.
struct ShaderBaseStruct {
    void *field_0x0;                    // +0x0 vtable
    int field_0x4;                      // +0x4 GL program handle (luma)
    volatile uint16_t field_0x8;        // +0x8

    uint32_t ES2LoadProgram(const char *vertexSource, const char *fragmentSource);
};

// AbyssEngine::BloomShader
class BloomShader : public ShaderBaseStruct  {
public:
    uint8_t field_0x9;                  // +0x9
    String field_0xc;                   // +0xc shader name
    unsigned int field_0x20;            // +0x20
    unsigned int field_0x24;            // +0x24
    int field_0x28;                     // +0x28
    unsigned int field_0x2c;            // +0x2c
    int field_0x30;                     // +0x30
    FBOContainer* field_0x34;           // +0x34
    unsigned int field_0x38;            // +0x38
    unsigned int field_0x3c;            // +0x3c
    int field_0x40;                     // +0x40
    unsigned int field_0x44;            // +0x44
    int field_0x48;                     // +0x48
    int field_0x4c;                     // +0x4c
    FBOContainer* field_0x50;           // +0x50
    unsigned int field_0x54;            // +0x54
    unsigned int field_0x58;            // +0x58
    int field_0x5c;                     // +0x5c
    unsigned int field_0x60;            // +0x60
    int field_0x64;                     // +0x64
    int field_0x68;                     // +0x68
    FBOContainer* field_0x6c;           // +0x6c
    FBOContainer* field_0x70;           // +0x70
    unsigned int field_0x74;            // +0x74
    unsigned int field_0x78;            // +0x78
    int field_0x7c;                     // +0x7c
    unsigned int field_0x80;            // +0x80
    int field_0x84;                     // +0x84
    int field_0x88;                     // +0x88
    unsigned int field_0x8c;            // +0x8c
    int field_0x90;                     // +0x90
    unsigned int field_0x94;            // +0x94
    int field_0x98;                     // +0x98

    BloomShader();
    void Init(::Engine *engine);
    void InternalInit(::Engine *engine);
    void UpdateMeshData(Mesh *mesh, ::Engine *engine);
    void SetInActive();
    void RenderEffect(FBOContainer *source, ::Engine *engine);
    void RenderEffect(FBOContainer *source, FBOContainer **target, ::Engine *engine);
};

} // namespace AbyssEngine


#endif
