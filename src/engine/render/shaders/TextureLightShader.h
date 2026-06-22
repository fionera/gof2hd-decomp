#ifndef GOF2_TEXTURELIGHTSHADER_H
#define GOF2_TEXTURELIGHTSHADER_H
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

    class TextureLightShader : public ShaderBaseStruct {
    public:
        int aPosition; // +0x20 vertex position attribute
        int aTexCoord; // +0x24 vertex texcoord attribute
        int aNormal; // +0x28 vertex normal attribute
        int uMvpMatrix; // +0x2c model-view-projection matrix
        int uModelViewMatrix; // +0x30 model-view matrix
        int uNormalMatrix; // +0x34 normal matrix
        int uColor0; // +0x38 material color 0 (vec4)
        int uColor1; // +0x3c material color 1 (vec4)
        int uAmbientColor; // +0x40 ambient color (vec4)
        int uTexture; // +0x44 diffuse texture sampler
        int uLight0Direction; // +0x48
        int uLight0Color; // +0x4c
        int uLight1Direction; // +0x50
        int uLight1Color; // +0x54
        int uLight2Direction; // +0x58
        int uLight2Color; // +0x5c
        int uShininess; // +0x60
        int uSpecularColor; // +0x64
        int uModelMatrix; // +0x68
        int uHasScaleAnimation; // +0x6c

        TextureLightShader();

        void Init(Engine *engine) override;

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    };
} // namespace AbyssEngine

#endif
