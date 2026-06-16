#ifndef GOF2_COLORMIXADD_H
#define GOF2_COLORMIXADD_H
#include "common.h"
#include "engine/render/ShaderBaseStruct.h"

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::ColorMixAdd — GLES2 color-mix/add shader (derives from ShaderBaseStruct).
// Caches two vertex-attribute locations (a0,a1) and six uniform locations (u0..u5)
// after Init resolves them from the linked program.
class ColorMixAdd : public ShaderBaseStruct {
public:
    static int ShaderIndex;

    int a0Loc;              // attribute a0
    int a1Loc;              // attribute a1
    int u1Loc;              // uniform u1
    int u2Loc;              // uniform u2
    int u0Loc;              // uniform u0
    int u4Loc;              // uniform u4
    int u3Loc;              // uniform u3
    int u5Loc;              // uniform u5

    ColorMixAdd();
    void Init(Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine

extern "C" char _ZTVN11AbyssEngine11ColorMixAddE[];

#endif
