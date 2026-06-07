#ifndef GOF2_DECOMP_BLURSHADER_CLASS_H
#define GOF2_DECOMP_BLURSHADER_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace AbyssEngine {

struct Engine;
struct FBOContainer;
struct Mesh;
struct ShaderBaseStruct;

struct String {
    String();
    String(const char *text, bool copy);
    ~String();
    String &operator=(const String &other);
};

namespace AEMath {

struct Vector {
    float x;
    float y;
    float z;

    float &operator[](int index);
    float operator[](int index) const;
};

} // namespace AEMath

struct BlurShader {
    BlurShader();
    ~BlurShader();

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    static void RenderEffect(FBOContainer *fbo, Engine *engine, float amount, AEMath::Vector vector);
    void RenderEffect(FBOContainer *fbo, FBOContainer **target, Engine *engine,
                      float amount, AEMath::Vector vector);
};

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

#endif
