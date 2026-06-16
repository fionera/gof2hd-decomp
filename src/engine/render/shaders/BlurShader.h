#ifndef GOF2_BLURSHADER_H
#define GOF2_BLURSHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// AbyssEngine::BlurShader — GLES2 radial/box blur post-process shader. Derives from
// ShaderBaseStruct (shared shader storage: GL program handle, name, dirty flag) and caches this
// program's attribute/uniform locations plus its blur tuning floats. RenderEffect draws a
// full-screen quad through the blur program into either the back buffer or a capture FBO.

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;
class FBOContainer;

class BlurShader : public ShaderBaseStruct {
public:
    int aPosition;              // a_position
    int uMvpMatrix;             // u_mvpMatrix
    int aTexCoord;              // a_texCoord
    int sTexture;               // s_texture
    int uTexelSize;             // u_texelSize
    int uBlurAmount;            // u_blurAmount
    int uStrength;              // u_strength
    int uCenter;                // u_center
    unsigned int positionAttrib;   // vertex attrib slot
    unsigned int texCoordAttrib;   // texcoord attrib slot
    float strength;
    float blurScale;

    BlurShader();

    void Init(Engine *engine) override;
    void SetInActive() override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    void RenderEffect(FBOContainer *fbo, FBOContainer **target, Engine *engine,
                      float amount, Vector vector);
};

} // namespace AbyssEngine

#endif
