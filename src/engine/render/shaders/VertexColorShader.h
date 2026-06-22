#ifndef GOF2_VERTEXCOLORSHADER_H
#define GOF2_VERTEXCOLORSHADER_H
#include "engine/core/Array.h"
#include "../../core/AEString.h"
#include "fieldaccess.h"

#include "engine/render/ShaderBaseStruct.h"


using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class VertexColorShader : public ShaderBaseStruct {
    public:
        int aPosition;
        int aTexCoord;
        int aNormal;
        int aTangent;
        int aBiNormal;
        int aColor;

        int uWorldViewProj;
        int uNormalMatrix;
        int uLightDir;
        int uLightColor;
        int uAmbientColor;
        int uMaterialDiffuse;
        int uMaterialAmbient;
        int uMaterialSpecular;
        int uMaterialShininess;

        VertexColorShader();

        void Init(Engine *engine) override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void SetInActive() override;
    };
}

#endif
