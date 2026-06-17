#ifndef GOF2_BLOOMSHADER_H
#define GOF2_BLOOMSHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// AbyssEngine::BloomShader — GLES2 multi-pass bloom post-process effect.
// Derives from ShaderBaseStruct and owns five shader programs (luma extraction,
// downsample, horizontal/vertical blur, final composite) plus four render-target
// FBOs (luma, blurH, blurV, black). Each program caches its attribute/uniform
// location handles after Init.
// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;

namespace AbyssEngine {

struct Mesh;
class FBOContainer;

// The luma program handle lives in the ShaderBaseStruct base (ShaderBaseStruct::program);
// its cached attribute/uniform locations sit at the tail of this class.
class BloomShader : public ShaderBaseStruct {
public:
    unsigned int downSampleProgram;     // +0x20  GL program handle (downsample)
    int downSampleAttribPosition;       // +0x24  a_position
    int downSampleUniformWorldMatrix;   // +0x28  u_WorldMatrix
    unsigned int downSampleAttribTexCoord; // +0x2c  a_texCoord
    int downSampleUniformTexture;       // +0x30  s_texture
    FBOContainer *fboLuma;              // +0x34  luma render target

    unsigned int blurHProgram;          // +0x38  GL program handle (horizontal blur)
    int blurHAttribPosition;            // +0x3c  a_position
    int blurHUniformWorldMatrix;        // +0x40  u_WorldMatrix
    unsigned int blurHAttribTexCoord;   // +0x44  a_texCoord
    int blurHUniformTexture;            // +0x48  s_texture
    int blurHUniformTexSize;            // +0x4c  texSize
    FBOContainer *fboBlurH;             // +0x50  horizontal blur render target

    unsigned int blurVProgram;          // +0x54  GL program handle (vertical blur)
    int blurVAttribPosition;            // +0x58  a_position
    int blurVUniformWorldMatrix;        // +0x5c  u_WorldMatrix
    unsigned int blurVAttribTexCoord;   // +0x60  a_texCoord
    int blurVUniformTexture;            // +0x64  s_texture
    int blurVUniformTexSize;            // +0x68  texSize
    FBOContainer *fboBlurV;             // +0x6c  vertical blur render target
    FBOContainer *fboBlack;             // +0x70  cleared black render target

    unsigned int finalProgram;          // +0x74  GL program handle (final composite)
    int finalAttribPosition;            // +0x78  a_position
    int finalUniformWorldMatrix;        // +0x7c  u_WorldMatrix
    unsigned int finalAttribTexCoord;   // +0x80  a_texCoord
    int finalUniformTexture;            // +0x84  s_texture
    int finalUniformTextureBloom;       // +0x88  s_texture_bloom

    int lumaAttribPosition;             // +0x8c  a_position (luma program)
    int lumaUniformWorldMatrix;         // +0x90  u_WorldMatrix
    unsigned int lumaAttribTexCoord;    // +0x94  a_texCoord
    int lumaUniformTexture;             // +0x98  s_texture

    BloomShader();
    void Init(Engine *engine) override;
    void InternalInit(Engine *engine);
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    void SetInActive() override;
    void RenderEffect(FBOContainer *source, Engine *engine) override;
    void RenderEffect(FBOContainer *source, FBOContainer *&target, Engine *engine) override;
};

} // namespace AbyssEngine

#endif
