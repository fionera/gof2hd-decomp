#ifndef GOF2_BUMPMAPPING_H
#define GOF2_BUMPMAPPING_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

extern "C" char _ZTVN11AbyssEngine11BumpMappingE[];

namespace AbyssEngine {

class Engine;
class Mesh;

// AbyssEngine::BumpMapping — GLES2 bump-mapping shader (derives from ShaderBaseStruct).
// Caches five vertex-attribute locations and four uniform locations after Init resolves
// them from the linked program.
class BumpMapping : public ShaderBaseStruct {
public:
    int a0Loc;              // attribute a_position
    int a1Loc;              // attribute a_normal
    int a2Loc;              // attribute a_tangent
    int a3Loc;              // attribute a_binormal
    int a4Loc;              // attribute a_texCoord
    int u0Loc;              // uniform u_mvpMatrix
    int u1Loc;              // uniform u_lightDir
    int u2Loc;              // uniform u_texture
    int u3Loc;              // uniform u_normalMap

    static int ShaderIndex;

    BumpMapping();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
