#ifndef GOF2_TEXTUREVTXCOLORSHADER_H
#define GOF2_TEXTUREVTXCOLORSHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// AbyssEngine::TextureVtxColorShader — textured shader with per-vertex colour.
// Derives from ShaderBaseStruct and keeps two GL programs: the normal program
// (ShaderBaseStruct::program, index 0) and a fog program (fogProgram, index 1).
// Every uniform / attribute location is therefore cached as a two-element array
// indexed by the active program.
// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;

namespace AbyssEngine {

class Mesh;

// The normal program handle lives in the ShaderBaseStruct base (ShaderBaseStruct::program);
// the fog program and all cached attribute/uniform locations sit at the tail of this class.
class TextureVtxColorShader : public ShaderBaseStruct {
public:
    int fogProgram;                  // GL program handle (fog variant, index 1)
    // Uniform / attribute locations, indexed by program (0 = normal, 1 = fog).
    int loc_a_position[2];           // a_position
    int loc_a_texCoord[2];           // a_texCoord
    int loc_a_VertexColor[2];        // a_VertexColor
    int loc_u_WorldMatrix[2];        // u_WorldMatrix
    int loc_glColor[2];              // glColor
    int loc_s_texture[2];            // s_texture
    int loc_u_UVAnimation[2];        // u_UVAnimation
    int loc_u_UvMatrix[2];           // u_UvMatrix
    int loc_u_fogColor[2];           // u_fogColor
    int loc_u_fogMaxDist[2];         // u_fogMaxDist
    int loc_u_fogMinDist[2];         // u_fogMinDist
    int loc_u_EnableFog[2];          // u_EnableFog
    int loc_u_eyeposmodel[2];        // u_eyeposmodel
    int loc_u_DarkenValue[2];        // u_DarkenValue

    static int ShaderIndex;

    TextureVtxColorShader();

    void Init(Engine *engine) override;
    void SetInActive() override;
    void ConnectShaderComponents(uint32_t program, int index);
    void UseShader(bool useExtra) override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
};

} // namespace AbyssEngine

#endif
