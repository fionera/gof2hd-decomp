#ifndef GOF2_FBOCONTAINER_H
#define GOF2_FBOCONTAINER_H
#include "gof2/common.h"

// Engine is referenced by pointer only; its real definition lives in
// gof2/engine/render/Engine.h. Note: the engine's Engine type is at global scope
// (matches the original ::Engine), not inside AbyssEngine.
class Engine;

namespace AbyssEngine {

// AbyssEngine::FBOContainer — wraps a GL framebuffer object (color texture + depth
// renderbuffer) for off-screen render-to-texture capture.
class FBOContainer {
public:
    uint32_t framebuffer;          // GL framebuffer object id
    uint32_t texture;              // GL color texture id
    uint32_t renderbuffer;         // GL depth renderbuffer id
    int32_t  width;                // capture width
    int32_t  height;               // capture height
    Engine*  engine;               // owning engine (default framebuffer source)
    uint8_t  created;              // true once Create() has built the GL objects
    String   name;                 // debug name
    uint8_t  field_0x28;           //
    uint32_t extraRenderbuffer0;   // additional renderbuffer ids released on teardown
    uint32_t extraRenderbuffer1;
    uint32_t extraRenderbuffer2;

    FBOContainer(Engine *engine, String name);
    ~FBOContainer();

    void Create(int width, int height, bool a, bool linear);
    void Release();
    void Activate();
    void BeginCapture();
    void EndCapture();

    // Engine-facing render-to-texture entry points (see .cpp).
    void ActivateRender2Texture();
    void ActivateTexture();
    void DeactivateRender2Texture();

    // Per-target construct/Create variants used by GlowPPShader's four FBOs.
    void initFrom(Engine *engine, String name);
    void Create0(int width, int height, bool a, bool linear);
    void Create1(int width, int height, bool a, bool linear);
    void Create2(int width, int height, bool a, bool linear);
    void Create3(int width, int height, bool a, bool linear);
};

} // namespace AbyssEngine

#endif
