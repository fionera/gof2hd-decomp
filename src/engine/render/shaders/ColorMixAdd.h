#ifndef GOF2_COLORMIXADD_H
#define GOF2_COLORMIXADD_H
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

    class ColorMixAdd : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int a0Loc; // attribute a0
        int a1Loc; // attribute a1
        int u1Loc; // uniform u1
        int u2Loc; // uniform u2
        int u0Loc; // uniform u0
        int u4Loc; // uniform u4
        int u3Loc; // uniform u3
        int u5Loc; // uniform u5

        ColorMixAdd();

        void Init(Engine *engine) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void SetInActive() override;
    };
} // namespace AbyssEngine

#endif
