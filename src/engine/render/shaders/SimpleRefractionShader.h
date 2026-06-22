#ifndef GOF2_SIMPLEREFRACTIONSHADER_H
#define GOF2_SIMPLEREFRACTIONSHADER_H
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

    class SimpleRefractionShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int aPositionLoc; // attribute a_position
        int aNormalLoc; // attribute a_normal
        int aTangentLoc; // attribute a_tangent
        int aTexCoordLoc; // attribute a_texCoord
        int uM0Loc; // uniform u_m0
        int uM1Loc; // uniform u_m1
        int uTex0Loc; // uniform u_tex0
        int uTex1Loc; // uniform u_tex1
        int uM4Loc; // uniform u_m4
        int uM3Loc; // uniform u_m3
        int uM2Loc; // uniform u_m2
        int uRefractLoc; // uniform u_refract
        int uM5Loc; // uniform u_m5
        int uM6Loc; // uniform u_m6

        SimpleRefractionShader();

        void Init(::Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, ::Engine *engine) override;
    };
} // namespace AbyssEngine

#endif
