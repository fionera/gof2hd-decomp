#ifndef GOF2_NOTEXSHADER_H
#define GOF2_NOTEXSHADER_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

// Galaxy on Fire 2 - AbyssEngine::NoTexShader (GLES2 untextured, single-color shader).
namespace AbyssEngine {

class Engine;
class Mesh;

class NoTexShader : public ShaderBaseStruct {
public:
    int aPosition;      // attribute a_position
    int uMvpMatrix;     // uniform u_mvp
    int uColor;         // uniform u_color

    NoTexShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
