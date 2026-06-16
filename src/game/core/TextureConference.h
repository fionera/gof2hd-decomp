#ifndef GOF2_TEXTURECONFERENCE_H
#define GOF2_TEXTURECONFERENCE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/ShaderBaseStruct.h"

// Engine is a top-level (global-scope) class; the renderer root is not in AbyssEngine.
class Engine;

namespace AbyssEngine {

class Mesh;

// AbyssEngine::TextureConference — GLES2 scrolling-texture shader program. Derives from
// ShaderBaseStruct; caches its attribute/uniform locations after Init and, in UpdateMeshData,
// uploads the MVP matrix plus a time-driven scrolling offset accumulated frame to frame.
class TextureConference : public ShaderBaseStruct {
public:
    static int ShaderIndex;

    int       aPosition;     // a_position
    int       aTexCoord;     // a_texCoord
    int       uColor;        // u_color
    int       uMvpMatrix;    // u_mvp
    int       uOffset;       // u_offset
    int       sTexture;      // u_texture (sampler)
    long long animTime;      // scrolling-animation accumulator (ms)

    TextureConference();

    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void Init(Engine *engine);
};

} // namespace AbyssEngine

#endif
