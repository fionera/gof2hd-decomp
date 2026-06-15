#ifndef GOF2_BUMPSHADERV3_H
#define GOF2_BUMPSHADERV3_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

namespace AbyssEngine {

class Engine;
class Mesh;

// AbyssEngine::BumpShaderV3 — GLES2 bump-mapping shader (BumpShaderV3.vsh/.fsh).
// Resolves the vertex attributes and per-light/material uniforms, then streams mesh
// geometry through them each frame.
class BumpShaderV3 : public ShaderBaseStruct {
public:
    // ---- vertex attribute locations ----
    int aPosition;
    int aTexCoord;
    int aNormal;
    int aTangent;
    int aBitangent;

    // ---- uniform locations ----
    int uModelViewProjectionMatrix;
    int uModelMatrix;
    int uLightDirModel0;
    int uLightDirModel1;
    int uEyePosModel;
    int uTexDiffuse;
    int uTexNormal;
    int uTexSpecular;
    int uGlColor;
    int uAmbientColor0;
    int uDiffuseColor0;
    int uAmbientColor1;
    int uSpecularColor0;
    int uDiffuseColor1;
    int uSpecularColor1;
    int uSpecularPower;
    int uRimColor;
    int uTexBiasDiffuse;
    int uTexBiasNormal;
    int uIsGlowMat;

    BumpShaderV3();

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);

    static int ShaderIndex;
};

} // namespace AbyssEngine

#endif
