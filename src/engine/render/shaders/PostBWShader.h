#ifndef GOF2_POSTBWSHADER_H
#define GOF2_POSTBWSHADER_H
#include "common.h"
#include "engine/render/ShaderBaseStruct.h"

// AbyssEngine::PostBWShader - post-process black & white shader. Renders a grayscale
// full-screen pass over the scene framebuffer. Derives from ShaderBaseStruct.
// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;
class FBOContainer;

class PostBWShader : public ShaderBaseStruct {
public:
    int aPosition;      // attribute a_Position
    int uMvpMatrix;     // uniform u_MVPMatrix
    int aTexCoord;      // attribute a_TexCoord
    int sTexture;       // uniform s_Texture

    PostBWShader();

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void RenderEffect(FBOContainer *fbo, Engine *engine);
};

} // namespace AbyssEngine

#endif
