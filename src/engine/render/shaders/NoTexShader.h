#ifndef GOF2_NOTEXSHADER_H
#define GOF2_NOTEXSHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// Galaxy on Fire 2 - AbyssEngine::NoTexShader (GLES2 untextured, single-color shader).
// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;

namespace AbyssEngine {

class Mesh;

class NoTexShader : public ShaderBaseStruct {
public:
    int aPosition;      // attribute a_position
    int uMvpMatrix;     // uniform u_mvp
    int uColor;         // uniform u_color

    NoTexShader();
    void Init(Engine *engine) override;
    void SetInActive() override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
};

} // namespace AbyssEngine

#endif
