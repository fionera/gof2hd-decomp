#ifndef GOF2_VERTEXCOLORALPHATEXTURESHADER_H
#define GOF2_VERTEXCOLORALPHATEXTURESHADER_H
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
    struct Mesh;

    class VertexColorAlphaTextureShader : public ShaderBaseStruct {
    public:
        int attrib1; // a1
        int attrib2; // a2
        int attrib3; // a3
        int attrib4; // a4
        int attrib5; // a5
        int attrib0; // a0
        int uniform0; // u0
        int uniform1; // u1
        int uniform2; // u2
        int uniform3; // u3
        int uniform4; // u4
        int uniform5; // u5
        int uniform6; // u6
        int uniform7; // u7
        int uniform8; // u8

        VertexColorAlphaTextureShader();

        void Init(Engine *engine) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void SetInActive() override;
    };
} // namespace AbyssEngine

#endif
