#ifndef GOF2_VERTEXCOLORSHADER_H
#define GOF2_VERTEXCOLORSHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::VertexColorShader — GLES2 shader for vertex-coloured, lit geometry.
// Derives from ShaderBaseStruct (which owns the GL program handle) and caches the
// program's attribute and uniform locations after Init.
class VertexColorShader : public ShaderBaseStruct {
public:
    int aPosition;          // attribute "position"
    int aTexCoord;          // attribute "texcoord"
    int aNormal;            // attribute "normal"
    int aTangent;           // attribute "tangent"
    int aBiNormal;          // attribute "binormal"
    int aColor;             // attribute "color"

    int uWorldViewProj;     // uniform "worldViewProj"
    int uNormalMatrix;      // uniform "normalMatrix"
    int uLightDir;          // uniform "lightDir"
    int uLightColor;        // uniform "lightColor"
    int uAmbientColor;      // uniform "ambientColor"
    int uMaterialDiffuse;   // uniform "materialDiffuse"
    int uMaterialAmbient;   // uniform "materialAmbient"
    int uMaterialSpecular;  // uniform "materialSpecular"
    int uMaterialShininess; // uniform "materialShininess"

    VertexColorShader();
    void Init(Engine *engine) override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    void SetInActive() override;
};

} // namespace AbyssEngine

#endif
