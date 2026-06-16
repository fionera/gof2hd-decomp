#ifndef GOF2_SANDBOXSHADER_H
#define GOF2_SANDBOXSHADER_H
#include "common.h"
#include "engine/render/ShaderBaseStruct.h"

// AbyssEngine::SandboxShader — GLES2 bump-mapped mesh program (SandboxShader.vsh/.fsh).
// Derives from ShaderBaseStruct, which owns the GL program handle; this class caches the
// attribute/uniform location handles queried at Init and feeds per-frame matrix, light and
// material data plus the mesh vertex arrays into the program.
// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

class SandboxShader : public ShaderBaseStruct {
public:
    // attribute locations
    int attrPosition;   // a_position
    int attrNormal;     // a_normal
    int attrTangent;    // a_tangent
    int attrBinormal;   // a_binormal
    int attrTexCoord;   // a_texCoord
    // uniform locations
    int uniformA;       // u_a
    int uniformB;       // u_b
    int uniformC;       // u_c
    int uniformF;       // u_f
    int uniformD;       // u_d
    int uniformE;       // u_e
    int uniformG;       // u_g

    SandboxShader();
    void Init(Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine
#endif
