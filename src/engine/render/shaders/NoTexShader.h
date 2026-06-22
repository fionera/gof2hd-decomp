#ifndef GOF2_NOTEXSHADER_H
#define GOF2_NOTEXSHADER_H
#include "engine/core/Array.h"
#include "../../core/AEString.h"
#include "fieldaccess.h"

#include "engine/render/ShaderBaseStruct.h"


using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class NoTexShader : public ShaderBaseStruct {
    public:
        int aPosition;
        int uMvpMatrix;
        int uColor;

        NoTexShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
}

#endif
