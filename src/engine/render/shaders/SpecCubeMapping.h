#ifndef GOF2_SPECCUBEMAPPING_H
#define GOF2_SPECCUBEMAPPING_H
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
    class SpecCubeMapping : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int attribPosition;
        int attribNormal;
        int attribTexCoord;
        int mvpMatrixLoc;
        int normalMatrixLoc;
        int uCameraPosition;

        int uLightDirection;

        int samplerLoc1;
        int samplerLoc0;

        int uParam6;

        int uLightAmbient;

        int uParam8;

        int uLightDiffuse;

        int uShininess;

        int uColor;

        SpecCubeMapping();

        void Init(Engine *) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void SetInActive() override;
    };
}
#endif
