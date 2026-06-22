#ifndef GOF2_GLOWSHADER_H
#define GOF2_GLOWSHADER_H
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

    class GlowShader : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int a_positionLoc; // attribute a_position
        int a_texCoordLoc; // attribute a_texCoord
        int u_mvpLoc; // uniform u_mvp
        int u_colorLoc; // uniform u_color
        int u_textureLoc; // uniform u_texture

        GlowShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
} // namespace AbyssEngine

#endif
