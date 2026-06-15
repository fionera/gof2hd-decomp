#ifndef GOF2_BLOOMSHADER_H
#define GOF2_BLOOMSHADER_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

// AbyssEngine::BloomShader — GLES2 multi-pass bloom post-process effect.
// Derives from ShaderBaseStruct and owns five shader programs (luma extraction,
// downsample, horizontal/vertical blur, final composite) plus four render-target
// FBOs (luma, blurH, blurV, black). Each program caches its attribute/uniform
// location handles after Init.
namespace AbyssEngine {

struct Engine;
struct Mesh;
class FBOContainer;

// The luma program handle lives in the ShaderBaseStruct base (ShaderBaseStruct::program);
// its cached attribute/uniform locations sit at the tail of this class.
class BloomShader : public ShaderBaseStruct {
public:
    unsigned int downSampleProgram;     // GL program handle (downsample)
    int downSampleAttribPosition;       // a_position
    int downSampleUniformWorldMatrix;   // u_WorldMatrix
    unsigned int downSampleAttribTexCoord; // a_texCoord
    int downSampleUniformTexture;       // s_texture
    FBOContainer *fboLuma;              // luma render target

    unsigned int blurHProgram;          // GL program handle (horizontal blur)
    int blurHAttribPosition;            // a_position
    int blurHUniformWorldMatrix;        // u_WorldMatrix
    unsigned int blurHAttribTexCoord;   // a_texCoord
    int blurHUniformTexture;            // s_texture
    int blurHUniformTexSize;            // texSize
    FBOContainer *fboBlurH;             // horizontal blur render target

    unsigned int blurVProgram;          // GL program handle (vertical blur)
    int blurVAttribPosition;            // a_position
    int blurVUniformWorldMatrix;        // u_WorldMatrix
    unsigned int blurVAttribTexCoord;   // a_texCoord
    int blurVUniformTexture;            // s_texture
    int blurVUniformTexSize;            // texSize
    FBOContainer *fboBlurV;             // vertical blur render target
    FBOContainer *fboBlack;             // cleared black render target

    unsigned int finalProgram;          // GL program handle (final composite)
    int finalAttribPosition;            // a_position
    int finalUniformWorldMatrix;        // u_WorldMatrix
    unsigned int finalAttribTexCoord;   // a_texCoord
    int finalUniformTexture;            // s_texture
    int finalUniformTextureBloom;       // s_texture_bloom

    int lumaAttribPosition;             // a_position (luma program)
    int lumaUniformWorldMatrix;         // u_WorldMatrix
    unsigned int lumaAttribTexCoord;    // a_texCoord
    int lumaUniformTexture;             // s_texture

    BloomShader();
    void Init(Engine *engine);
    void InternalInit(Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
    void RenderEffect(FBOContainer *source, Engine *engine);
    void RenderEffect(FBOContainer *source, FBOContainer **target, Engine *engine);
};

} // namespace AbyssEngine

#endif
