#ifndef GOF2_CUBENORMALMAPPING_H
#define GOF2_CUBENORMALMAPPING_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;

namespace AbyssEngine {

class Mesh;

class CubeNormalMapping : public ShaderBaseStruct {
public:
    int attribA0;
    int attribA1;
    int attribA2;
    int attribA3;
    int attribA4;
    int uniformU0;
    int uniformU1;
    int uniformU2;
    int uniformU3;
    int uniformU5;
    int uniformU4;
    int uniformU6;
    int uniformU7;
    int uniformU8;
    int uniformU9;
    int uniformU12;
    int uniformU10;
    int uniformU11;

    CubeNormalMapping();
    void Init(Engine *) override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    void SetInActive() override;
};

} // namespace AbyssEngine
#endif
