#ifndef GOF2_TEXTURECONFERENCE_H
#define GOF2_TEXTURECONFERENCE_H
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

    class TextureConference : public ShaderBaseStruct {
    public:
        static int ShaderIndex;

        int aPosition; // +0x20  a_position
        int aTexCoord; // +0x24  a_texCoord
        int sTexture; // +0x28  u_texture (sampler)
        int uMvpMatrix; // +0x2c  u_mvp
        int uColor; // +0x30  u_color
        int uOffset; // +0x34  u_offset
        long long animTime; // +0x38  scrolling-animation accumulator (ms)

        TextureConference();

        void SetInActive() override;

        void UpdateMeshData(Mesh *mesh, Engine *engine) override;

        void Init(Engine *engine) override;
    };
} // namespace AbyssEngine

#endif
