#ifndef GOF2_TEXONLYSHADER_H
#define GOF2_TEXONLYSHADER_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

// Galaxy on Fire 2 - AbyssEngine::TexOnlyShader (minimal GLES2 shader that only samples a texture).

namespace AbyssEngine {

class Engine;
class Mesh;

class TexOnlyShader : public ShaderBaseStruct {
public:
    int aPosition;      // attribute a_position
    int aTexCoord;      // attribute a_texCoord
    int uWorldMatrix;   // uniform u_WorldMatrix
    int sTexture;       // uniform s_texture

    TexOnlyShader();

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
