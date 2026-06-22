#ifndef GOF2_NOTEXVTXCOLORSHADER_H
#define GOF2_NOTEXVTXCOLORSHADER_H
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

    class NoTexVtxColorShader : public ShaderBaseStruct {
    public:
        int aPosition; // attribute a_position
        int aColor; // attribute a_color
        int uMvpMatrix; // uniform u_mvp
        int uColor; // uniform u_color

        NoTexVtxColorShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
} // namespace AbyssEngine

#endif
