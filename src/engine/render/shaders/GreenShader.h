#ifndef GOF2_GREENSHADER_H
#define GOF2_GREENSHADER_H
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

    class GreenShader : public ShaderBaseStruct {
    public:
        int a0Loc; // attribute a0
        int a1Loc; // attribute a1
        int a2Loc; // attribute a2
        int a3Loc; // attribute a3
        int a4Loc; // attribute a4
        int u0Loc; // uniform u0
        int u1Loc; // uniform u1
        int u2Loc; // uniform u2
        int u3Loc; // uniform u3
        int u4Loc; // uniform u4
        int u5Loc; // uniform u5
        int u6Loc; // uniform u6
        int u7Loc; // uniform u7
        int u8Loc; // uniform u8

        GreenShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
} // namespace AbyssEngine

#endif
