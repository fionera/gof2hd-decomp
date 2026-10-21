#ifndef GOF2_ENERGYSHIELD_H
#define GOF2_ENERGYSHIELD_H
#include "engine/core/Array.h"
#include "../../core/AEString.h"
#include "engine/render/ShaderBaseStruct.h"

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
}

#endif
