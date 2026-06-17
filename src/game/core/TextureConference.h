#ifndef GOF2_TEXTURECONFERENCE_H
#define GOF2_TEXTURECONFERENCE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// Engine's real type lives in namespace AbyssEngine; the global alias keeps bare `Engine` working.
namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::TextureConference — GLES2 scrolling-texture shader program. Derives from
// ShaderBaseStruct; caches its attribute/uniform locations after Init and, in UpdateMeshData,
// uploads the MVP matrix plus a time-driven scrolling offset accumulated frame to frame.
class TextureConference : public ShaderBaseStruct {
public:
    static int ShaderIndex;

    int       aPosition;     // +0x20  a_position
    int       aTexCoord;     // +0x24  a_texCoord
    int       sTexture;      // +0x28  u_texture (sampler)
    int       uMvpMatrix;    // +0x2c  u_mvp
    int       uColor;        // +0x30  u_color
    int       uOffset;       // +0x34  u_offset
    long long animTime;      // +0x38  scrolling-animation accumulator (ms)

    TextureConference();

    void SetInActive() override;
    void UpdateMeshData(Mesh *mesh, Engine *engine) override;
    void Init(Engine *engine) override;
};

} // namespace AbyssEngine

#endif
