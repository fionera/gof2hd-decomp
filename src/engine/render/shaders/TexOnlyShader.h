#ifndef GOF2_TEXONLYSHADER_H
#define GOF2_TEXONLYSHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// Galaxy on Fire 2 - AbyssEngine::TexOnlyShader (minimal GLES2 shader that only samples a texture).

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

class TexOnlyShader : public ShaderBaseStruct {
public:
    int aPosition;      // attribute a_position
    int aTexCoord;      // attribute a_texCoord
    int uWorldMatrix;   // uniform u_WorldMatrix
    int sTexture;       // uniform s_texture

    TexOnlyShader();

    void Init(Engine *engine) override;
    void SetInActive() override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
};

} // namespace AbyssEngine

#endif
