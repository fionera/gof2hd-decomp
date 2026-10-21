#ifndef GOF2_BUMPRIMCUBESHADER_NEW_H
#define GOF2_BUMPRIMCUBESHADER_NEW_H
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

// Rim-lit cube-mapped bump shader (newer variant). Streams per-frame lighting and
// camera state into its GL uniforms each draw.
class BumpRimCubeShader_new : public ShaderBaseStruct {
public:
    int attrib0;
    int attrib1;
    int attrib2;
    int attrib3;
    int attrib4;
    int uniform0;
    int uniform1;
    int uniform2;
    int uniform3;
    int uniform4;
    int uniform5;
    int uniform6;
    int uniform7;
    int uniform8;
    int uniform9;
    int uniform10;
    int uniform11;
    int uniform12;
    int uniform13;
    int uniform14;
    int uniform15;
    int uniform16;
    int uniform17;
    int uniform18;
    int uniform19;
    int uniform20;
    int uniform21;
    int uniform22;
    int uniform23;
    int uniform24;

    static int ShaderIndex;

    BumpRimCubeShader_new();
    void Init(Engine *);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine
#endif
