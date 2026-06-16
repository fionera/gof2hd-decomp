#ifndef GOF2_GLOWPPSHADER_H
#define GOF2_GLOWPPSHADER_H
#include "common.h"
#include "engine/render/ShaderBaseStruct.h"

// Galaxy on Fire 2 -- AbyssEngine::GlowPPShader.
// GLES2 post-process glow/bloom shader. Derives from ShaderBaseStruct and owns four
// FBOContainer render targets plus attribute/uniform locations for four sub-programs
// (copy / blurX / blurY / combine).

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;
class FBOContainer;

class GlowPPShader : public ShaderBaseStruct {
public:
    unsigned int copyProgram;       // copy program GL handle
    int copyAttribPosition;         // "position"
    int copyUniformWorldView;       // "worldView"
    int copyAttribTexCoord;         // "texcoord"
    int copyUniformTexture;         // "texture"

    FBOContainer *copyTarget;       // copy render target

    unsigned int blurXProgram;      // blurX program GL handle
    int blurXAttribPosition;        // "position"
    int blurXUniformWorldView;      // "worldView"
    int blurXAttribTexCoord;        // "texcoord"
    int blurXUniformTexture;        // "texture"
    int blurXUniformSampleSize;     // "sampleSize"

    FBOContainer *blurXTarget;      // blurX render target

    unsigned int blurYProgram;      // blurY program GL handle
    int blurYAttribPosition;        // "position"
    int blurYUniformWorldView;      // "worldView"
    int blurYAttribTexCoord;        // "texcoord"
    int blurYUniformTexture;        // "texture"
    int blurYUniformSampleSize;     // "sampleSize"

    FBOContainer *blurYTarget;      // blurY render target

    unsigned int combineProgram;    // combine program GL handle
    int combineAttribPosition;      // "position"
    int combineUniformWorldView;    // "worldView"
    int combineAttribTexCoord;      // "texcoord"
    int combineUniformTexture;      // "texture"
    int combineUniformTexture2;     // "texture2"

    int meshAttribPosition;         // attribute used in UpdateMeshData/SetInActive
    int meshAttribTexCoord;         // attribute used in UpdateMeshData/SetInActive

    FBOContainer *backgroundTarget; // combine/background render target

    GlowPPShader();
    void Init();
    void InternalInit(Engine *engine);
    void RenderEffect(FBOContainer *source, FBOContainer **target, Engine *engine);
    void RenderEffect_simple(FBOContainer *source, Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine
#endif
