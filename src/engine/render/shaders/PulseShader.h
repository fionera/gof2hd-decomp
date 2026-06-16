#ifndef GOF2_PULSESHADER_H
#define GOF2_PULSESHADER_H
#include "common.h"
#include "engine/render/ShaderBaseStruct.h"

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::PulseShader — GLES2 pulse shader (derives from ShaderBaseStruct).
// Caches five vertex-attribute locations (a0..a4) and ten uniform locations (u0..u9)
// after Init resolves them from the linked program.
class PulseShader : public ShaderBaseStruct {
public:
    int a0Loc;              // attribute a0
    int a1Loc;              // attribute a1
    int a2Loc;              // attribute a2
    int a3Loc;              // attribute a3
    int a4Loc;              // attribute a4
    int u0Loc;              // uniform u0
    int u1Loc;              // uniform u1
    int u2Loc;              // uniform u2
    int u3Loc;              // uniform u3
    int u5Loc;              // uniform u5
    int u6Loc;              // uniform u6
    int u7Loc;              // uniform u7
    int u8Loc;              // uniform u8
    int u9Loc;              // uniform u9
    int u4Loc;              // uniform u4

    PulseShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
