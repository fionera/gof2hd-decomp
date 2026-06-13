#include "gof2/engine/render/FBOContainer.h"

// FBOContainer only needs the Engine framebuffer-binding field at +0x40c.
// The full Engine definition is owned by another batch (Engine.h); to keep this
// translation unit self-contained we model just that field here. The layout offset
// matches Engine::field_0x40c.
namespace AbyssEngine {
struct Engine {
    char _pad[0x40c];
    uint32_t field_0x40c;
};
}

namespace AbyssEngine {

void FBOContainer::Activate() {
    glBindTexture(0xde1, this->field_0x4);
}

void FBOContainer::BeginCapture() {
    glBindFramebuffer(0x8d40, this->field_0x0);
    glViewport(0, 0, this->field_0xc, this->field_0x10);
}

void FBOContainer::Create(int width, int height, bool a, bool linear) {
    this->field_0xc = width;
    this->field_0x10 = height;
    glGenFramebuffers(1, &this->field_0x0);
    glBindFramebuffer(0x8d40, this->field_0x0);
    glGenTextures(1, &this->field_0x4);
    glBindTexture(0xde1, this->field_0x4);
    glPixelStorei(0xcf5, 1);
    glTexParameteri(0xde1, 0x2802, 0x812f);
    glTexParameteri(0xde1, 0x2803, 0x812f);
    GLint filter;
    if (linear) {
        glTexParameteri(0xde1, 0x2800, 0x2601);
        filter = 0x2601;
    } else {
        glTexParameteri(0xde1, 0x2800, 0x2600);
        filter = 0x2600;
    }
    glTexParameteri(0xde1, 0x2801, filter);
    glTexImage2D(0xde1, 0, 0x1908, this->field_0xc,
                 this->field_0x10, 0, 0x1908, 0x1401, 0);
    glFramebufferTexture2D(0x8d40, 0x8ce0, 0xde1, this->field_0x4, 0);
    glGenRenderbuffers(1, &this->field_0x8);
    glBindRenderbuffer(0x8d41, this->field_0x8);
    glRenderbufferStorage(0x8d41, 0x81a5, this->field_0xc,
                          this->field_0x10);
    glFramebufferRenderbuffer(0x8d40, 0x8d00, 0x8d41, this->field_0x8);
    glCheckFramebufferStatus(0x8d40);
    this->field_0x18 = 1;
    Engine *engine = this->field_0x14;
    glBindFramebuffer(0x8d40, engine->field_0x40c);
}

void FBOContainer::Release() {
    if (this->field_0x18 == 0) {
        return;
    }
    glDeleteFramebuffers(1, &this->field_0x0);
    this->field_0x0 = 0;
    glDeleteTextures(1, &this->field_0x4);
    this->field_0x4 = 0;
    glDeleteRenderbuffers(1, &this->field_0x8);
    this->field_0x8 = 0;
    this->field_0x18 = 0;
    glDeleteRenderbuffers(1, &this->field_0x30);
    glDeleteRenderbuffers(1, &this->field_0x34);
    glBindFramebuffer(0x8d40, 0);
    glDeleteRenderbuffers(1, &this->field_0x2c);
}

// AbyssEngine::FBOContainer::FBOContainer(AbyssEngine::Engine*, AbyssEngine::String)
FBOContainer::FBOContainer(Engine *engine, String name)
{
    this->field_0x18 = 0;
    this->field_0x28 = 0;

    // Zero framebuffer / texture / renderbuffer / width and clear height.
    this->field_0x0 = 0;
    this->field_0x4 = 0;
    this->field_0x8 = 0;
    this->field_0xc = 0;
    this->field_0x10 = 0;

    // Copy the supplied name into the embedded String.
    this->field_0x1c = name;

    this->field_0x14 = engine;
}

void FBOContainer::EndCapture() {
    Engine *engine = this->field_0x14;
    glBindFramebuffer(0x8d40, engine->field_0x40c);
}

FBOContainer::~FBOContainer() {
    Release();
}

// ---- Engine-facing render-to-texture entry points ----
// These are the FBOContainer-side operations the Engine's FBO helpers
// (ActivateRender2FracFBO / ActivateTextureFBO / DeactivateRender2FracFBO)
// tail-call into once they have fetched the active FBOContainer.

// Bind this framebuffer and set the viewport to its dimensions so that
// subsequent drawing is captured into the attached color texture.
void FBOContainer::ActivateRender2Texture() {
    glBindFramebuffer(0x8d40, this->field_0x0);
    glViewport(0, 0, this->field_0xc, this->field_0x10);
}

// Bind this FBO's color texture as the current 2D texture so it can be
// sampled while drawing the post-processing pass.
void FBOContainer::ActivateTexture() {
    glBindTexture(0xde1, this->field_0x4);
}

// Restore the engine's default framebuffer, ending render-to-texture.
void FBOContainer::DeactivateRender2Texture() {
    Engine *engine = this->field_0x14;
    glBindFramebuffer(0x8d40, engine->field_0x40c);
}

// The GlowPPShader builds four identically-shaped offscreen targets; the
// decompiler emitted a distinct construct/Create fragment per call site.
// They are byte-for-byte the same as the primary constructor / Create, so
// each variant simply forwards to that shared logic.
void FBOContainer::initFrom(Engine *engine, String name) {
    this->field_0x18 = 0;
    this->field_0x28 = 0;
    this->field_0x0 = 0;
    this->field_0x4 = 0;
    this->field_0x8 = 0;
    this->field_0xc = 0;
    this->field_0x10 = 0;
    this->field_0x1c = name;
    this->field_0x14 = engine;
}

void FBOContainer::Create0(int width, int height, bool a, bool linear) { Create(width, height, a, linear); }
void FBOContainer::Create1(int width, int height, bool a, bool linear) { Create(width, height, a, linear); }
void FBOContainer::Create2(int width, int height, bool a, bool linear) { Create(width, height, a, linear); }
void FBOContainer::Create3(int width, int height, bool a, bool linear) { Create(width, height, a, linear); }

} // namespace AbyssEngine
