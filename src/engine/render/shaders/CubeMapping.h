#ifndef GOF2_CUBEMAPPING_H
#define GOF2_CUBEMAPPING_H
#include "common.h"
#include "engine/render/ShaderBaseStruct.h"

// AbyssEngine::CubeMapping — GLES2 cube-mapping shader.
// Derives from ShaderBaseStruct; holds one GL program (the base's ShaderBaseStruct::program)
// plus its cached attribute/uniform location handles.

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

class CubeMapping : public ShaderBaseStruct {
public:
    int aPosition;        // a0 (position)
    int aNormal;          // a1 (normal)
    int aTexCoord;        // a2 (texcoord)
    int uMvp;             // u0 (mvp)
    int uNormalMatrix;    // u1 (normal matrix)
    int uniform2;         // u2
    int uniform3;         // u3
    int uniform5;         // u5
    int uniform4;         // u4
    int uniform6;         // u6
    int uniform7;         // u7
    int uniform8;         // u8
    int uniform9;         // u9
    int uniform10;        // u10
    int uniform11;        // u11

    CubeMapping();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
