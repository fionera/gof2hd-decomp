#ifndef GOF2_TEXTUREVTXCOLORSHADER_H
#define GOF2_TEXTUREVTXCOLORSHADER_H
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

    class TextureVtxColorShader : public ShaderBaseStruct {
    public:
        int fogProgram; // GL program handle (fog variant, index 1)
        // Uniform / attribute locations, indexed by program (0 = normal, 1 = fog).
        int loc_a_position[2]; // a_position
        int loc_a_texCoord[2]; // a_texCoord
        int loc_a_VertexColor[2]; // a_VertexColor
        int loc_u_WorldMatrix[2]; // u_WorldMatrix
        int loc_glColor[2]; // glColor
        int loc_s_texture[2]; // s_texture
        int loc_u_UVAnimation[2]; // u_UVAnimation
        int loc_u_UvMatrix[2]; // u_UvMatrix
        int loc_u_fogColor[2]; // u_fogColor
        int loc_u_fogMaxDist[2]; // u_fogMaxDist
        int loc_u_fogMinDist[2]; // u_fogMinDist
        int loc_u_EnableFog[2]; // u_EnableFog
        int loc_u_eyeposmodel[2]; // u_eyeposmodel
        int loc_u_DarkenValue[2]; // u_DarkenValue

        static int ShaderIndex;

        TextureVtxColorShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void ConnectShaderComponents(uint32_t program, int index);

        void UseShader(bool useExtra) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
} // namespace AbyssEngine

#endif
