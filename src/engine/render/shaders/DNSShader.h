#ifndef GOF2_DNSSHADER_H
#define GOF2_DNSSHADER_H
#include "common.h"
#include "engine/render/ShaderBaseStruct.h"

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::DNSShader — GLES2 "DNS" lighting shader (derives from ShaderBaseStruct).
// Caches four vertex-attribute locations (a_position/a_normal/a_tangent/a_binormal) and
// thirteen uniform locations (u_m0..u_m12) after Init resolves them from the linked program.
class DNSShader : public ShaderBaseStruct {
public:
    int aPositionLoc;          // attribute a_position
    int aNormalLoc;            // attribute a_normal
    int aTangentLoc;           // attribute a_tangent
    int aBinormalLoc;          // attribute a_binormal
    int uM0Loc;                // uniform u_m0
    int uM1Loc;                // uniform u_m1
    int uM2Loc;                // uniform u_m2
    int uM3Loc;                // uniform u_m3
    int uM4Loc;                // uniform u_m4
    int uM5Loc;                // uniform u_m5
    int uM6Loc;                // uniform u_m6
    int uM7Loc;                // uniform u_m7
    int uM8Loc;                // uniform u_m8
    int uM9Loc;                // uniform u_m9
    int uM10Loc;               // uniform u_m10
    int uM11Loc;               // uniform u_m11
    int uM12Loc;               // uniform u_m12

    DNSShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
