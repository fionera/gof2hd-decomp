#ifndef GOF2_DRAWFBOSHADER_H
#define GOF2_DRAWFBOSHADER_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

namespace AbyssEngine {

class Engine;
class Mesh;
class FBOContainer;

// AbyssEngine::DrawFBOShader — GLES2 full-screen FBO blit shader (derives from ShaderBaseStruct).
class DrawFBOShader : public ShaderBaseStruct {
public:
    static int ShaderIndex;

    int positionLoc;            // attribute "position"
    int worldViewMatrixLoc;     // uniform "worldViewMatrix"
    int texCoordLoc;            // attribute "texCoord"
    int textureLoc;             // uniform "texture"

    DrawFBOShader();

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void RenderEffect(FBOContainer *fbo, Engine *engine);
};

} // namespace AbyssEngine

#endif
