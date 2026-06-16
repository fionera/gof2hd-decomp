#ifndef GOF2_CUBEMAPPING_H
#define GOF2_CUBEMAPPING_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
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
    int aPosition;        // +0x20  a0 (position attribute location)
    int aNormal;          // +0x24  a1 (normal attribute location)
    int aTexCoord;        // +0x28  a2 (texcoord attribute location)
    int uMvp;             // +0x2c  u0 (mvp matrix uniform)
    int uNormalMatrix;    // +0x30  u1 (normal matrix uniform)
    int uniform2;         // +0x34  u2
    int uniform3;         // +0x38  u3
    int uniform5;         // +0x3c  u5
    int uniform4;         // +0x40  u4
    int uniform6;         // +0x44  u6
    int uniform7;         // +0x48  u7
    int uniform8;         // +0x4c  u8
    int uniform9;         // +0x50  u9
    int uniform10;        // +0x54  u10
    int uniform11;        // +0x58  u11

    CubeMapping();
    void Init(Engine *engine) override;
    void SetInActive() override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
};

} // namespace AbyssEngine

#endif
