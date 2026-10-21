#ifndef GOF2_BUMPMAPPING_H
#define GOF2_BUMPMAPPING_H
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
    class BumpMapping : public ShaderBaseStruct {
    public:
        int aPositionLoc;
        int aNormalLoc;
        int aTangentLoc;
        int aBitangentLoc;
        int aTexCoordLoc;
        int uWorldMatrixLoc;
        int uLightposmodelLoc;
        int sTextureBaseLoc;
        int sTextureNormalLoc;

        static int ShaderIndex;

        BumpMapping();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
}

#endif
