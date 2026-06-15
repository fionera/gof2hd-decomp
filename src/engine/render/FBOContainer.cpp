#include "gof2/engine/render/FBOContainer.h"
#include "gof2/engine/render/Engine.h"
#include "gof2/platform/gl.h"

// Framebuffer-object GL entry points not yet declared in gof2/platform/gl.h.
// (Signatures use the same plain-integer style as gl.h; they belong in that
// header once the platform externs pass reaches the FBO functions.)
extern "C" {
void glGenFramebuffers(int n, uint32_t *framebuffers);
void glDeleteFramebuffers(int n, const uint32_t *framebuffers);
void glGenTextures(int n, uint32_t *textures);
void glDeleteTextures(int n, const uint32_t *textures);
void glPixelStorei(uint32_t pname, int param);
void glTexParameteri(uint32_t target, uint32_t pname, int param);
void glTexImage2D(uint32_t target, int level, int internalformat, int width,
                  int height, int border, uint32_t format, uint32_t type, const void *pixels);
void glFramebufferTexture2D(uint32_t target, uint32_t attachment, uint32_t textarget,
                            uint32_t texture, int level);
void glGenRenderbuffers(int n, uint32_t *renderbuffers);
void glBindRenderbuffer(uint32_t target, uint32_t renderbuffer);
void glDeleteRenderbuffers(int n, const uint32_t *renderbuffers);
void glRenderbufferStorage(uint32_t target, uint32_t internalformat, int width, int height);
void glFramebufferRenderbuffer(uint32_t target, uint32_t attachment, uint32_t renderbuffertarget,
                               uint32_t renderbuffer);
uint32_t glCheckFramebufferStatus(uint32_t target);
}

namespace AbyssEngine {

FBOContainer::FBOContainer(Engine *engine, String name) {
    this->created = 0;
    this->field_0x28 = 0;
    this->framebuffer = 0;
    this->texture = 0;
    this->renderbuffer = 0;
    this->width = 0;
    this->height = 0;
    this->name = name;
    this->engine = engine;
}

FBOContainer::~FBOContainer() {
    Release();
}

void FBOContainer::Create(int width, int height, bool a, bool linear) {
    this->width = width;
    this->height = height;
    glGenFramebuffers(1, &this->framebuffer);
    glBindFramebuffer(0x8d40, this->framebuffer);
    glGenTextures(1, &this->texture);
    glBindTexture(0xde1, this->texture);
    glPixelStorei(0xcf5, 1);
    glTexParameteri(0xde1, 0x2802, 0x812f);
    glTexParameteri(0xde1, 0x2803, 0x812f);
    int filter;
    if (linear) {
        glTexParameteri(0xde1, 0x2800, 0x2601);
        filter = 0x2601;
    } else {
        glTexParameteri(0xde1, 0x2800, 0x2600);
        filter = 0x2600;
    }
    glTexParameteri(0xde1, 0x2801, filter);
    glTexImage2D(0xde1, 0, 0x1908, this->width, this->height, 0, 0x1908, 0x1401, 0);
    glFramebufferTexture2D(0x8d40, 0x8ce0, 0xde1, this->texture, 0);
    glGenRenderbuffers(1, &this->renderbuffer);
    glBindRenderbuffer(0x8d41, this->renderbuffer);
    glRenderbufferStorage(0x8d41, 0x81a5, this->width, this->height);
    glFramebufferRenderbuffer(0x8d40, 0x8d00, 0x8d41, this->renderbuffer);
    glCheckFramebufferStatus(0x8d40);
    this->created = 1;
    glBindFramebuffer(0x8d40, this->engine->field_0x40c);
}

void FBOContainer::Release() {
    if (this->created == 0) {
        return;
    }
    glDeleteFramebuffers(1, &this->framebuffer);
    this->framebuffer = 0;
    glDeleteTextures(1, &this->texture);
    this->texture = 0;
    glDeleteRenderbuffers(1, &this->renderbuffer);
    this->renderbuffer = 0;
    this->created = 0;
    glDeleteRenderbuffers(1, &this->extraRenderbuffer1);
    glDeleteRenderbuffers(1, &this->extraRenderbuffer2);
    glBindFramebuffer(0x8d40, 0);
    glDeleteRenderbuffers(1, &this->extraRenderbuffer0);
}

void FBOContainer::Activate() {
    glBindTexture(0xde1, this->texture);
}

void FBOContainer::BeginCapture() {
    glBindFramebuffer(0x8d40, this->framebuffer);
    glViewport(0, 0, this->width, this->height);
}

void FBOContainer::EndCapture() {
    glBindFramebuffer(0x8d40, this->engine->field_0x40c);
}

// ---- Engine-facing render-to-texture entry points ----
// These are the FBOContainer-side operations the Engine's FBO helpers
// (ActivateRender2FracFBO / ActivateTextureFBO / DeactivateRender2FracFBO)
// tail-call into once they have fetched the active FBOContainer.

// Bind this framebuffer and set the viewport to its dimensions so that
// subsequent drawing is captured into the attached color texture.
void FBOContainer::ActivateRender2Texture() {
    glBindFramebuffer(0x8d40, this->framebuffer);
    glViewport(0, 0, this->width, this->height);
}

// Bind this FBO's color texture as the current 2D texture so it can be
// sampled while drawing the post-processing pass.
void FBOContainer::ActivateTexture() {
    glBindTexture(0xde1, this->texture);
}

// Restore the engine's default framebuffer, ending render-to-texture.
void FBOContainer::DeactivateRender2Texture() {
    glBindFramebuffer(0x8d40, this->engine->field_0x40c);
}

// The GlowPPShader builds four identically-shaped offscreen targets; the
// decompiler emitted a distinct construct/Create fragment per call site.
// They are byte-for-byte the same as the primary constructor / Create, so
// each variant simply forwards to that shared logic.
void FBOContainer::initFrom(Engine *engine, String name) {
    this->created = 0;
    this->field_0x28 = 0;
    this->framebuffer = 0;
    this->texture = 0;
    this->renderbuffer = 0;
    this->width = 0;
    this->height = 0;
    this->name = name;
    this->engine = engine;
}

void FBOContainer::Create0(int width, int height, bool a, bool linear) { Create(width, height, a, linear); }
void FBOContainer::Create1(int width, int height, bool a, bool linear) { Create(width, height, a, linear); }
void FBOContainer::Create2(int width, int height, bool a, bool linear) { Create(width, height, a, linear); }
void FBOContainer::Create3(int width, int height, bool a, bool linear) { Create(width, height, a, linear); }

} // namespace AbyssEngine
