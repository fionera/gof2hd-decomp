#ifndef GOF2_MASKSHADER_H
#define GOF2_MASKSHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// Galaxy on Fire 2 - AbyssEngine::MaskShader (GLES2 masked/textured shader).

struct Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::MaskShader
class MaskShader : public ShaderBaseStruct {
public:
    int a_position;     // +0x20 attribute a_position
    int a_texCoord;     // +0x24 attribute a_texCoord
    int a_color;        // +0x28 attribute a_color (mask tex coord)
    int u_mvpMatrix;    // +0x2c uniform u_mvpMatrix (world matrix)
    int u_texture0;     // +0x30 uniform u_texture0
    int u_texture1;     // +0x34 uniform u_texture1
    int u_color;        // +0x38 uniform u_color

    static int ShaderIndex;

    MaskShader();
    void Init(::Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, ::Engine *engine);
};

} // namespace AbyssEngine

#endif
