#ifndef GOF2_GLOWSHADER_H
#define GOF2_GLOWSHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::GlowShader — GLES2 glow shader (derives from ShaderBaseStruct).
// Caches two vertex-attribute locations (a_position/a_texCoord) and three uniform
// locations (u_mvp/u_color/u_texture) after Init resolves them from the linked program.
class GlowShader : public ShaderBaseStruct {
public:
    static int ShaderIndex;

    int a_positionLoc;          // attribute a_position
    int a_texCoordLoc;          // attribute a_texCoord
    int u_mvpLoc;               // uniform u_mvp
    int u_colorLoc;             // uniform u_color
    int u_textureLoc;           // uniform u_texture

    GlowShader();
    void Init(Engine *engine) override;
    void SetInActive() override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
};

} // namespace AbyssEngine


#endif
