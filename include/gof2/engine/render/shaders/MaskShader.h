#ifndef GOF2_MASKSHADER_H
#define GOF2_MASKSHADER_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

// Galaxy on Fire 2 - AbyssEngine::MaskShader (GLES2 masked/textured shader).

struct Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::MaskShader
class MaskShader : public ShaderBaseStruct {
public:
    int a_position;     // attribute a_position
    int a_texCoord;     // attribute a_texCoord
    int a_color;        // attribute a_color
    int u_mvpMatrix;    // uniform u_mvpMatrix
    int u_texture0;     // uniform u_texture0
    int u_texture1;     // uniform u_texture1
    int u_color;        // uniform u_color

    static int ShaderIndex;

    MaskShader();
    void Init(::Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, ::Engine *engine);
};

} // namespace AbyssEngine

#endif
