#ifndef GOF2_GENERICSHADER2_H
#define GOF2_GENERICSHADER2_H
#include "common.h"
#include "engine/render/ShaderBaseStruct.h"

// Galaxy on Fire 2 - AbyssEngine::GenericShader2 (GLES2 bump/lit shader).
// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

class GenericShader2 : public ShaderBaseStruct {
public:
    int aPosition;
    int aNormal;
    int aTangent;
    int aBinormal;
    int aTexCoord;
    int uM0;
    int uM1;
    int uM2;
    int uM3;
    int uM4;
    int uM5;
    int uM6;
    int uM7;
    int uM8;

    static int ShaderIndex;

    GenericShader2();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
