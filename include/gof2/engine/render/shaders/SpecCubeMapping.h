#ifndef GOF2_SPECCUBEMAPPING_H
#define GOF2_SPECCUBEMAPPING_H
#include "gof2/common.h"
#include "gof2/engine/render/ShaderBaseStruct.h"

extern "C" char _ZTVN11AbyssEngine15SpecCubeMappingE[];

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

class SpecCubeMapping : public ShaderBaseStruct {
public:
    static int ShaderIndex;

    int attribPosition;     // attrib a0
    int attribNormal;       // attrib a1
    int attribTexCoord;     // attrib a2
    int mvpMatrixLoc;       // uniform u0
    int normalMatrixLoc;    // uniform u1
    int uCameraPosition;    // uniform u2
    int field_0x38;         // uniform u3
    int samplerLoc1;        // uniform u5
    int samplerLoc0;        // uniform u4
    int field_0x44;         // uniform u6
    int field_0x48;         // uniform u7
    int field_0x4c;         // uniform u8
    int field_0x50;         // uniform u9
    int uShininess;         // uniform u10
    int field_0x58;         // uniform u11

    SpecCubeMapping();
    void Init(Engine *);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine
#endif
