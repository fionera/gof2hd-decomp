#ifndef GOF2_ENERGYSHIELD_H
#define GOF2_ENERGYSHIELD_H
#include "engine/core/Array.h"
#include "../../core/AEString.h"
#include "engine/render/ShaderBaseStruct.h"
#include <cstddef>

namespace AbyssEngine {
    class Mesh;
}

namespace AbyssEngine {
    class Engine;
}

using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class EnergyShield : public ShaderBaseStruct {
    public:
        int aPositionLoc;
        int aTexCoordLoc;
        int aNormalLoc;
        int uModelViewProjectionMatrixLoc;
        int uModelViewMatrixLoc;
        int uEyeposmodelLoc;
        int sTexture0Loc;
        int sTexture1Loc;
        // Unused in the original: Init never writes 0x40 or 0x4c and no method reads them.
        int field_0x40;
        int uGlColorLoc;
        int uRimColorLoc;
        int field_0x4c;
        int uTimeLoc;
        int uVertTimeLoc;
        int uTexelSizeLoc;
        int sTextureBaseLoc;

        static int ShaderIndex;

        EnergyShield();

        void Init(Engine *engine) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void SetInActive() override;
    };

    // Authoritative layout from the original binary: Init @0x8a28c stores the
    // glGetAttribLocation/glGetUniformLocation results at 0x20..0x5c, UpdateMeshData
    // @0x8a40c and SetInActive @0x8a3e0 read the same slots, and the ctor allocates
    // 0x60 bytes.  The two field_0xNN slots are never written or read.
    static_assert(sizeof(EnergyShield) == 0x60, "EnergyShield size");
    static_assert(offsetof(EnergyShield, aPositionLoc) == 0x20, "a_position");
    static_assert(offsetof(EnergyShield, aTexCoordLoc) == 0x24, "a_texCoord");
    static_assert(offsetof(EnergyShield, aNormalLoc) == 0x28, "a_normal");
    static_assert(offsetof(EnergyShield, uModelViewProjectionMatrixLoc) == 0x2c,
                  "u_ModelViewProjectionMatrix");
    static_assert(offsetof(EnergyShield, uModelViewMatrixLoc) == 0x30, "u_ModelViewMatrix");
    static_assert(offsetof(EnergyShield, uEyeposmodelLoc) == 0x34, "u_eyeposmodel");
    static_assert(offsetof(EnergyShield, sTexture0Loc) == 0x38, "s_texture[0]");
    static_assert(offsetof(EnergyShield, sTexture1Loc) == 0x3c, "s_texture[1]");
    static_assert(offsetof(EnergyShield, field_0x40) == 0x40, "field_0x40");
    static_assert(offsetof(EnergyShield, uGlColorLoc) == 0x44, "glColor");
    static_assert(offsetof(EnergyShield, uRimColorLoc) == 0x48, "u_RimColor");
    static_assert(offsetof(EnergyShield, field_0x4c) == 0x4c, "field_0x4c");
    static_assert(offsetof(EnergyShield, uTimeLoc) == 0x50, "u_time");
    static_assert(offsetof(EnergyShield, uVertTimeLoc) == 0x54, "u_vertTime");
    static_assert(offsetof(EnergyShield, uTexelSizeLoc) == 0x58, "u_texelSize");
    static_assert(offsetof(EnergyShield, sTextureBaseLoc) == 0x5c, "s_texture_base");
}

#endif
