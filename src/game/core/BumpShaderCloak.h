#ifndef GOF2_BUMPSHADERCLOAK_H
#define GOF2_BUMPSHADERCLOAK_H
#include "common.h"
#include "engine/render/ShaderBaseStruct.h"

extern "C" char _ZTVN11AbyssEngine15BumpShaderCloakE[];

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::BumpShaderCloak — GLES2 refraction/cloak shader (derives from ShaderBaseStruct).
// Caches the program's vertex attribute and uniform locations after Init; a uniform location is
// -1 when the GLSL compiler optimised the corresponding name away.
class BumpShaderCloak : public ShaderBaseStruct {
public:
    int attrib_a0;      // vertex position
    int attrib_a1;      // texcoord
    int attrib_a2;      // normal
    int attrib_a3;      // tangent
    int attrib_a4;      // binormal

    int uniform_u0;     // mvp matrix (mat4)
    int uniform_u1;     // normal matrix (mat3)
    int uniform_u2;     // light/ambient color
    int uniform_u3;     // fallback diffuse color
    int uniform_u4;
    int uniform_u5;     // texture sampler 0
    int uniform_u6;     // texture sampler 1
    int uniform_u7;     // texture sampler 6
    int uniform_u8;
    int uniform_u9;
    int uniform_u12;
    int uniform_u10;
    int uniform_u13;
    int uniform_u11;
    int uniform_u14;
    int uniform_u15;
    int uniform_u16;
    int uniform_u18;    // mesh-derived param @mesh+0x1c
    int uniform_u19;    // mesh-derived param @mesh+0x20
    int uniform_u17;    // viewport inverse-size (vec2)
    int uniform_u20;    // refraction sampler 7

    static int ShaderIndex;

    BumpShaderCloak();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
