#ifndef GOF2_SIMPLESHADER_H
#define GOF2_SIMPLESHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// Galaxy on Fire 2 - AbyssEngine::SimpleShader (GLES2 solid-color shader).
// Renders meshes with a single uniform color modulated by a world matrix.

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

class SimpleShader : public ShaderBaseStruct {
public:
    int aPosition;      // attribute a_position
    int uWorldMatrix;   // uniform u_WorldMatrix
    int uColor;         // uniform u_color

    SimpleShader();

    void Init(Engine *engine) override;
    void SetInActive() override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
};

} // namespace AbyssEngine

#endif
