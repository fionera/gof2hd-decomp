#ifndef GOF2_BUMPSHADERV4_H
#define GOF2_BUMPSHADERV4_H
#include "common.h"
#include "engine/render/ShaderBaseStruct.h"

extern "C" char _ZTVN11AbyssEngine12BumpShaderV4E[];

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::BumpShaderV4 - GLES2 bump-mapping shader variant. Resolves the
// vertex attribute and uniform locations of BumpShaderV4.vsh/.fsh in Init() and
// feeds per-mesh geometry and per-frame lighting state to the program.
class BumpShaderV4 : public ShaderBaseStruct {
public:
    int aPosition;
    int aTexCoord;
    int aNormal;
    int aTangent;
    int aBitangent;
    int uMvpMatrix;
    int uModelMatrix;
    int uLightDirModel;
    int uEyePosModel;
    int uTexture0;
    int uTexture1;
    int uColor;
    int uAmbientColor;
    int uDiffuseColor;
    int uSpecularColor;

    static int ShaderIndex;

    BumpShaderV4();

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
