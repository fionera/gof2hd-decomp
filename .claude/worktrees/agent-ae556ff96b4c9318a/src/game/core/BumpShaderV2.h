#ifndef GOF2_BUMPSHADERV2_H
#define GOF2_BUMPSHADERV2_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// AbyssEngine::BumpShaderV2 — GLES2 bump-mapping shader program. Derives from
// ShaderBaseStruct; caches attribute/uniform locations after Init and uploads the
// per-mesh / per-frame matrices and lighting block in UpdateMeshData.

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;

namespace AbyssEngine {

struct Mesh;

// Cached attribute/uniform location handles, filled in by Init. The per-material lighting
// block upload in UpdateMeshData is gated by the inherited ShaderBaseStruct::dirty flag.
class BumpShaderV2 : public ShaderBaseStruct {
public:
    int aPosition;       // a_position
    int aNormal;         // a_normal
    int aTexCoord;       // a_texCoord
    int aTangent;        // a_tangent
    int aBinormal;       // a_binormal
    int uMvpMatrix;      // u_mvpMatrix
    int uNormalMatrix;   // u_normalMatrix
    int uLightDir;       // u_lightDir
    int uEyePos;         // u_eyePos
    int uTexture0;       // u_texture0
    int uTexture1;       // u_texture1
    int uAmbient;        // u_ambient
    int uDiffuse;        // u_diffuse
    int uSpecular;       // u_specular
    int uEmissive;       // u_emissive

    BumpShaderV2();
    void Init(Engine *engine) override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    void SetInActive() override;
};

} // namespace AbyssEngine

#endif
