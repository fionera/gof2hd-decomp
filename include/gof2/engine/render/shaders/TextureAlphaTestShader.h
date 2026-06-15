#ifndef GOF2_TEXTUREALPHATESTSHADER_H
#define GOF2_TEXTUREALPHATESTSHADER_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

namespace AbyssEngine {

class Engine;
class Mesh;

// AbyssEngine::TextureAlphaTestShader — GLES2 texture shader with an optional alpha-test variant.
// Derives from ShaderBaseStruct and owns two GL programs: the opaque program (in the base
// ShaderBaseStruct::program, index 0) and the alpha-test program (alphaProgram, index 1). Every
// cached attribute/uniform location is a 2-element array indexed by program (0 = opaque,
// 1 = alpha).
class TextureAlphaTestShader : public ShaderBaseStruct {
public:
    int alphaProgram;          // GL program handle (alpha-test variant, index 1)

    int aPositionLoc[2];       // a_Position
    int aTexCoordLoc[2];       // a_TexCoord
    int uMVPMatrixLoc[2];      // u_MVPMatrix
    int uColorLoc[2];          // u_Color
    int uTextureLoc[2];        // u_Texture
    int uLightPosLoc[2];       // u_LightPos
    int uAmbientLoc[2];        // u_Ambient
    int uDiffuseLoc[2];        // u_Diffuse
    int uSamplerLoc[2];        // u_Sampler
    int uFogColorLoc[2];       // u_FogColor

    // Snapshot of ShaderBaseStruct::shaderIndexIntern taken at construction.
    static int ShaderIndex;

    TextureAlphaTestShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void ConnectShaderComponents(int program, int index);
    void UseShader(bool);
};

} // namespace AbyssEngine

#endif
