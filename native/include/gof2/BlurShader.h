#ifndef GOF2_BLURSHADER_H
#define GOF2_BLURSHADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

struct Engine;
struct FBOContainer;
struct Mesh;
struct ShaderBaseStruct;



namespace AEMath {



} // namespace AEMath



} // namespace AbyssEngine

using BlurShader = AbyssEngine::BlurShader;
using Engine = AbyssEngine::Engine;
using FBOContainer = AbyssEngine::FBOContainer;
using Mesh = AbyssEngine::Mesh;
using ShaderBaseStruct = AbyssEngine::ShaderBaseStruct;
using String = AbyssEngine::String;
using Vector = AbyssEngine::AEMath::Vector;

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

static inline uint32_t stack_guard_delta(uint32_t saved, uint32_t current)
{
    return current - saved;
}

struct BlurShader {
    uint8_t field_0x9;                  // +0x9
    unsigned int field_0x24;            // +0x24
    int field_0x28;                     // +0x28
    int field_0x2c;                     // +0x2c
    int field_0x30;                     // +0x30
    int field_0x34;                     // +0x34
    int field_0x38;                     // +0x38
    int field_0x3c;                     // +0x3c
    int field_0x40;                     // +0x40
    int field_0x44;                     // +0x44
    unsigned int field_0x48;            // +0x48
    unsigned int field_0x50;            // +0x50
    float field_0x58;                   // +0x58
    float field_0x5c;                   // +0x5c
};
#endif
