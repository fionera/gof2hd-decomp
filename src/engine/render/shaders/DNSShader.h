#ifndef GOF2_DNSSHADER_H
#define GOF2_DNSSHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

namespace AbyssEngine {
    class Engine;
}

using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class Mesh;

    class DNSShader : public ShaderBaseStruct {
    public:
        int aPositionLoc; // attribute a_position
        int aNormalLoc; // attribute a_normal
        int aTangentLoc; // attribute a_tangent
        int aBinormalLoc; // attribute a_binormal
        int uM0Loc; // uniform u_m0
        int uM1Loc; // uniform u_m1
        int uM2Loc; // uniform u_m2
        int uM3Loc; // uniform u_m3
        int uM4Loc; // uniform u_m4
        int uM5Loc; // uniform u_m5
        int uM6Loc; // uniform u_m6
        int uM7Loc; // uniform u_m7
        int uM8Loc; // uniform u_m8
        int uM9Loc; // uniform u_m9
        int uM10Loc; // uniform u_m10
        int uM11Loc; // uniform u_m11
        int uM12Loc; // uniform u_m12

        DNSShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
} // namespace AbyssEngine

#endif
