#ifndef GOF2_SIMPLEREFRACTIONSHADER_H
#define GOF2_SIMPLEREFRACTIONSHADER_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

// Engine is the global ::Engine type (defined in Engine.h); referenced here by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::SimpleRefractionShader — GLES2 refraction shader (derives from ShaderBaseStruct).
// Caches four vertex-attribute locations and the uniform locations resolved by Init from the
// linked program.
class SimpleRefractionShader : public ShaderBaseStruct {
public:
    static int ShaderIndex;

    int aPositionLoc;       // attribute a_position
    int aNormalLoc;         // attribute a_normal
    int aTangentLoc;        // attribute a_tangent
    int aTexCoordLoc;       // attribute a_texCoord
    int uM0Loc;             // uniform u_m0
    int uM1Loc;             // uniform u_m1
    int uTex0Loc;           // uniform u_tex0
    int uTex1Loc;           // uniform u_tex1
    int uM4Loc;             // uniform u_m4
    int uM3Loc;             // uniform u_m3
    int uM2Loc;             // uniform u_m2
    int uRefractLoc;        // uniform u_refract
    int uM5Loc;             // uniform u_m5
    int uM6Loc;             // uniform u_m6

    SimpleRefractionShader();

    void Init(::Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, ::Engine *engine);
};

} // namespace AbyssEngine

#endif
