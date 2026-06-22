#ifndef GOF2_SANDBOXSHADER_H
#define GOF2_SANDBOXSHADER_H
#include "engine/core/Array.h"
#include "../../core/AEString.h"
#include "fieldaccess.h"

#include "engine/render/ShaderBaseStruct.h"


using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class SandboxShader : public ShaderBaseStruct {
    public:
        int attrPosition;
        int attrNormal;
        int attrTangent;
        int attrBinormal;
        int attrTexCoord;

        int uniformA;
        int uniformB;
        int uniformC;
        int uniformF;
        int uniformD;
        int uniformE;
        int uniformG;

        SandboxShader();

        void Init(Engine *engine) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void SetInActive() override;
    };
}
#endif
