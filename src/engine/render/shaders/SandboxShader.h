#ifndef GOF2_SANDBOXSHADER_H
#define GOF2_SANDBOXSHADER_H
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

    class SandboxShader : public ShaderBaseStruct {
    public:
        // attribute locations
        int attrPosition; // a_position
        int attrNormal; // a_normal
        int attrTangent; // a_tangent
        int attrBinormal; // a_binormal
        int attrTexCoord; // a_texCoord
        // uniform locations
        int uniformA; // u_a
        int uniformB; // u_b
        int uniformC; // u_c
        int uniformF; // u_f
        int uniformD; // u_d
        int uniformE; // u_e
        int uniformG; // u_g

        SandboxShader();

        void Init(Engine *engine) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void SetInActive() override;
    };
} // namespace AbyssEngine
#endif
