#ifndef GOF2_TEXTURELIGHTSHADER_H
#define GOF2_TEXTURELIGHTSHADER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// AbyssEngine::TextureLightShader — GLES2 lit-texture shader (derives from ShaderBaseStruct).
// Owns the cached attribute/uniform location handles for a single program: vertex attributes
// (position/texcoord/normal), the model/view/projection matrices, and a three-light Phong
// lighting block (per-light direction + color, shininess, specular, ambient).
// Engine is the global ::Engine type (defined in Engine.h); referenced by pointer only.
struct Engine;

namespace AbyssEngine {

class Mesh;

class TextureLightShader : public ShaderBaseStruct {
public:
    int aPosition;          // vertex position attribute
    int aTexCoord;          // vertex texcoord attribute
    int aNormal;            // vertex normal attribute
    int uMvpMatrix;         // model-view-projection matrix
    int uModelViewMatrix;   // model-view matrix
    int uNormalMatrix;      // normal matrix
    int uColor0;            // material color 0 (vec4)
    int uColor1;            // material color 1 (vec4)
    int uAmbientColor;      // ambient color (vec4)
    int uTexture;           // diffuse texture sampler
    int uLight0Direction;
    int uLight0Color;
    int uLight1Direction;
    int uLight1Color;
    int uLight2Direction;
    int uLight2Color;
    int uShininess;
    int uSpecularColor;
    int uModelMatrix;
    int uHasScaleAnimation;

    TextureLightShader();
    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
};

} // namespace AbyssEngine

#endif
