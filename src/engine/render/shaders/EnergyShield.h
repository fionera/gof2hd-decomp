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
        int aPosition;
        int aTexCoord;
        int aNormal;
        int uMvpMatrix;
        int uModelViewMatrix;
        int uEyePosModel;
        int sTexture0;
        int sTexture1;
        int field_0x40;
        int uGlColor;
        int uRimColor;
        int field_0x4c;
        int uTime;
        int uVertTime;
        int uTexelSize;
        int sTextureBase;

        static int ShaderIndex;

        EnergyShield();

        void Init(Engine *engine) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void SetInActive() override;
    };
}

#endif
