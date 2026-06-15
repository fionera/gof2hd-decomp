#ifndef GOF2_BUMPRIMCUBESHADER_H
#define GOF2_BUMPRIMCUBESHADER_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

namespace AbyssEngine {

class Engine;
class Mesh;

// AbyssEngine::BumpRimCubeShader — GLES2 bump + rim + cubemap shader. Caches its vertex-attribute
// and uniform locations after Init resolves them from the linked program, then streams per-mesh and
// per-frame light/material/fog uniforms in UpdateMeshData.
class BumpRimCubeShader : public ShaderBaseStruct {
public:
    int aPosition;
    int aTexCoord;
    int aNormal;
    int aTangent;
    int aBitangent;
    int uModelViewProjectionMatrix;
    int uModelMatrix;
    int uModelMatrixFull;
    int uLightDirModel0;
    int uLightDirModel1;
    int uEyePosModel;
    int uTexDiffuse;
    int uTexNormal;
    int uTexCube;
    int uGlColor;
    int uAmbientColor0;
    int uDiffuseColor1;
    int uDiffuseColor0;
    int uSpecularColor1;
    int uSpecularColor0;
    int uRimColor;
    int uSpecularPower;
    int uIsGlowMat;
    int uTexBiasDiffuse;
    int uTexBiasNormal;
    int uFogColor;
    int uFogMaxDist;
    int uFogMinDist;
    int uEnableFog;
    int uLodDist;

    static int ShaderIndex;

    BumpRimCubeShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
