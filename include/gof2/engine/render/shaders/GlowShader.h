#ifndef GOF2_GLOWSHADER_H
#define GOF2_GLOWSHADER_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

namespace AbyssEngine {

class Engine;
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
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

extern "C" char _ZTVN11AbyssEngine10GlowShaderE[];

#endif
