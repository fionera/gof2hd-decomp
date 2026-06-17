#ifndef GOF2_GENERICSHADER_H
#define GOF2_GENERICSHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// AbyssEngine::GenericShader — the standard per-mesh GLES2 program (GenericShader.vsh/.fsh).
// Derives from ShaderBaseStruct, which owns the GL program handle; this class caches the
// attribute/uniform location handles queried at Init and feeds per-frame matrix, light and
// material data plus the mesh vertex arrays into the program.
// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;

namespace AbyssEngine {

class Mesh;

class GenericShader : public ShaderBaseStruct {
public:
    // attribute locations
    int aPosition;            // a0
    int aTexCoord;            // a1
    int aNormal;              // a2
    int aTangent;             // a3
    int aBitangent;           // a4
    // uniform locations
    int uMvpMatrix;           // u0
    int uNormalMatrix;        // u1
    int uLightPosition;       // u2
    int uEyePosition;         // u3
    int uTexture0;            // u4 (sampler, texture unit 0)
    int uTexture1;            // u5 (sampler, texture unit 1)
    int uTexture2;            // u6 (sampler, texture unit 2)
    int uAmbientColor;        // u7
    int uDiffuseColor;        // u8
    int uSpecularColor;       // u9
    int uEmissiveColor;       // u10
    int uMaterialShininess;   // u11

    GenericShader();
    void Init(Engine *engine) override;
    void SetInActive() override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
};

} // namespace AbyssEngine

#endif
