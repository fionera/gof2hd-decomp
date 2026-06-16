#ifndef GOF2_NOTEXSHADER_H
#define GOF2_NOTEXSHADER_H
#include "common.h"
#include "engine/render/ShaderBaseStruct.h"

// Galaxy on Fire 2 - AbyssEngine::NoTexShader (GLES2 untextured, single-color shader).
// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

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
