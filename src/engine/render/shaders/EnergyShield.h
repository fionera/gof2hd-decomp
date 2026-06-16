#ifndef GOF2_ENERGYSHIELD_H
#define GOF2_ENERGYSHIELD_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

// EnergyShield — refraction shield shader. Int fields are GL uniform/attribute locations.
class EnergyShield : public ShaderBaseStruct {
public:
    // GL locations (the tex0/tex1 pair is accessed dynamically as &uTex0 + i).
    int aPosition;             // a_position
    int aTexCoord;             // a_texCoord
    int uM0;                   // u_m0
    int uM1;                   // u_m1
    int uM2;                   // u_m2
    int uM3;                   // u_m3
    int uTex0;                 // u_tex0
    int uTex1;                 // u_tex1
    int uM4;                   // u_m4
    int uM5;                   // u_m5
    int uM7;                   // u_m7
    int uRefract;              // u_refract
    int uM6;                   // u_m6
    int uM8;                   // u_m8

    static int ShaderIndex;

    EnergyShield();
    void Init(Engine *engine) override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    void SetInActive() override;
};

} // namespace AbyssEngine

extern "C" {
extern char EnergyShield_vtable[];
}

#endif
