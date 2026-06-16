#ifndef GOF2_TEXTURESHADER_H
#define GOF2_TEXTURESHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// AbyssEngine::TextureShader — GLES2 textured-mesh shader. The base program
// (ShaderBaseStruct::program) renders opaque meshes; an extended program adds the
// secondary slot. Per-slot uniform/attribute locations are stored as 2-element
// arrays indexed by the active slot.
// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

class TextureShader : public ShaderBaseStruct {
public:
    uint32_t programExt;            // extended GL program handle (slot 1)
    int positionAttrib[2];         // position
    int texcoordAttrib[2];         // texcoord
    int mvpUniform[2];             // mvp
    int colorUniform[2];           // color
    int textureUniform[2];         // texture
    int worldViewUniform[2];       // worldView
    int textureModeUniform[2];     // textureMode
    int lightUniform[2];           // light
    int fogNearUniform[2];         // fogNear
    int fogFarUniform[2];          // fogFar
    int activeTextureUniform[2];   // activeTexture
    int fogColorUniform[2];        // fogColor
    int alphaUniform[2];           // alpha

    TextureShader();
    void Init(Engine *engine);
    void UseShader(bool flag);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void ConnectShaderComponents(uint32_t program, int slot);
};

} // namespace AbyssEngine

#endif
