#ifndef GOF2_SPECCUBEMAPPING_H
#define GOF2_SPECCUBEMAPPING_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

extern "C" char _ZTVN11AbyssEngine15SpecCubeMappingE[];

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

class SpecCubeMapping : public ShaderBaseStruct {
public:
    static int ShaderIndex;

    int attribPosition;     // +0x20  attrib a0 (position)
    int attribNormal;       // +0x24  attrib a1 (normal)
    int attribTexCoord;     // +0x28  attrib a2 (texcoord)
    int mvpMatrixLoc;       // +0x2c  uniform u0  (world-view-proj matrix)
    int normalMatrixLoc;    // +0x30  uniform u1  (normal matrix)
    int uCameraPosition;    // +0x34  uniform u2  (light color / camera position)
    union {
        int uLightDirection; // +0x38  uniform u3  (light direction xyz + w)
        int field_0x38;
    };
    int samplerLoc1;        // +0x3c  uniform u5  (sampler unit 1)
    int samplerLoc0;        // +0x40  uniform u4  (sampler unit 0)
    union {
        int uParam6;        // +0x44  uniform u6  (per-frame scalar)
        int field_0x44;
    };
    union {
        int uLightAmbient;  // +0x48  uniform u7  (ambient light color)
        int field_0x48;
    };
    union {
        int uParam8;        // +0x4c  uniform u8  (per-frame color)
        int field_0x4c;
    };
    union {
        int uLightDiffuse;  // +0x50  uniform u9  (diffuse light color)
        int field_0x50;
    };
    int uShininess;         // +0x54  uniform u10 (material shininess)
    union {
        int uColor;         // +0x58  uniform u11 (global color)
        int field_0x58;
    };

    SpecCubeMapping();
    void Init(Engine *);
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void SetInActive();
};

} // namespace AbyssEngine
#endif
