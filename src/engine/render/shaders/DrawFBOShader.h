#ifndef GOF2_DRAWFBOSHADER_H
#define GOF2_DRAWFBOSHADER_H
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
    class FBOContainer;

    class DrawFBOShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int positionLoc; // attribute "position"
        int worldViewMatrixLoc; // uniform "worldViewMatrix"
        int texCoordLoc; // attribute "texCoord"
        int textureLoc; // uniform "texture"

        DrawFBOShader();

        ~DrawFBOShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void RenderEffect(FBOContainer *fbo, Engine *engine) override;
    };
} // namespace AbyssEngine

#endif
