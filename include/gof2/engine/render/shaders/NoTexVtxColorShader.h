#ifndef GOF2_NOTEXVTXCOLORSHADER_H
#define GOF2_NOTEXVTXCOLORSHADER_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

// Galaxy on Fire 2 - AbyssEngine::NoTexVtxColorShader (GLES2 untextured, per-vertex-color shader).
namespace AbyssEngine {

class Engine;
class Mesh;

// AbyssEngine::NoTexVtxColorShader
class NoTexVtxColorShader : public ShaderBaseStruct {
public:
    int aPosition;      // attribute a_position
    int aColor;         // attribute a_color
    int uMvpMatrix;     // uniform u_mvp
    int uColor;         // uniform u_color

    NoTexVtxColorShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
