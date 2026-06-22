#ifndef GOF2_BUMPMAPPING_H
#define GOF2_BUMPMAPPING_H
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

    class BumpMapping : public ShaderBaseStruct {
    public:
        int a0Loc; // attribute a_position
        int a1Loc; // attribute a_normal
        int a2Loc; // attribute a_tangent
        int a3Loc; // attribute a_binormal
        int a4Loc; // attribute a_texCoord
        int u0Loc; // uniform u_mvpMatrix
        int u1Loc; // uniform u_lightDir
        int u2Loc; // uniform u_texture
        int u3Loc; // uniform u_normalMap

        static int ShaderIndex;

        BumpMapping();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
} // namespace AbyssEngine

#endif
