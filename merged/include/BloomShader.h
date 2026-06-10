#ifndef GOF2_DECOMP_BLOOMSHADER_CLASS_H
#define GOF2_DECOMP_BLOOMSHADER_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace AbyssEngine {

struct Engine;
struct FBOContainer;
struct Mesh;
struct ShaderBaseStruct;

struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    String();
    String(const char *text, bool copy);
    ~String();
    String &operator=(const String &other);
};

struct BloomShader {
    BloomShader();
    ~BloomShader();

    void Init(Engine *engine);
    void InternalInit(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void RenderEffect(FBOContainer *fbo, Engine *engine);
    void RenderEffect(FBOContainer *fbo, FBOContainer **target, Engine *engine);
};

} // namespace AbyssEngine

using BloomShader = AbyssEngine::BloomShader;
using Engine = AbyssEngine::Engine;
using FBOContainer = AbyssEngine::FBOContainer;
using Mesh = AbyssEngine::Mesh;
using ShaderBaseStruct = AbyssEngine::ShaderBaseStruct;
using String = AbyssEngine::String;

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

#endif
