#ifndef GOF2_BUMPSHADER_H
#define GOF2_BUMPSHADER_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::BumpShader — GLES2 bump-mapping shader (derives from ShaderBaseStruct).
// Caches three vertex-attribute locations (a0..a2) and 22 uniform locations (u0..u21)
// after Init resolves them from the linked program.
class BumpShader : public ShaderBaseStruct {
public:
    int a0Loc;          // attribute a0 (position)
    int a1Loc;          // attribute a1 (texcoord)
    int a2Loc;          // attribute a2 (normal)
    int u0Loc;          // uniform u0
    int u1Loc;          // uniform u1
    int u2Loc;          // uniform u2
    int u3Loc;          // uniform u3
    int u4Loc;          // uniform u4
    int u5Loc;          // uniform u5
    int u6Loc;          // uniform u6
    int u7Loc;          // uniform u7
    int u8Loc;          // uniform u8
    int u11Loc;         // uniform u11
    int u9Loc;          // uniform u9
    int u12Loc;         // uniform u12
    int u10Loc;         // uniform u10
    int u13Loc;         // uniform u13
    int u14Loc;         // uniform u14
    int u15Loc;         // uniform u15
    int u16Loc;         // uniform u16
    int u17Loc;         // uniform u17
    int u18Loc;         // uniform u18
    int u19Loc;         // uniform u19
    int u20Loc;         // uniform u20
    int u21Loc;         // uniform u21

    BumpShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);

    // Bind a sampler uniform to a texture unit (thin glUniform1i wrapper).
    static void setSampler(int location, int unit);
};

} // namespace AbyssEngine

#endif
