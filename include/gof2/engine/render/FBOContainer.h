#ifndef GOF2_FBOCONTAINER_H
#define GOF2_FBOCONTAINER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef void GLvoid;

extern "C" {
void glGenFramebuffers(GLsizei n, GLuint *framebuffers);
void glBindFramebuffer(GLenum target, GLuint framebuffer);
void glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
void glGenTextures(GLsizei n, GLuint *textures);
void glBindTexture(GLenum target, GLuint texture);
void glDeleteTextures(GLsizei n, const GLuint *textures);
void glPixelStorei(GLenum pname, GLint param);
void glTexParameteri(GLenum target, GLenum pname, GLint param);
void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width,
                  GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture,
                            GLint level);
void glGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
void glBindRenderbuffer(GLenum target, GLuint renderbuffer);
void glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget,
                               GLuint renderbuffer);
GLenum glCheckFramebufferStatus(GLenum target);
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
}

namespace AbyssEngine {

// Minimal view of Engine used by FBOContainer (full definition lives in Engine.h,
// owned by another batch). Only the framebuffer-binding field at +0x40c is touched here.
struct Engine;

// AbyssEngine::FBOContainer — wraps a GL framebuffer object (color texture + depth
// renderbuffer) for off-screen render-to-texture capture.
// Layout (byte offsets):
//   0x00 GLuint framebuffer
//   0x04 GLuint texture
//   0x08 GLuint renderbuffer
//   0x0c GLsizei width
//   0x10 GLsizei height
//   0x14 Engine* engine
//   0x18 bool created
//   0x1c String name
//   0x2c .. 0x34 extra GL ids
class FBOContainer {
public:
    GLuint field_0x0;                   // +0x0  framebuffer id
    GLuint field_0x4;                   // +0x4  color texture id
    GLuint field_0x8;                   // +0x8  depth renderbuffer id
    GLsizei field_0xc;                  // +0xc  width
    GLsizei field_0x10;                 // +0x10 height
    Engine* field_0x14;                 // +0x14 owning engine
    unsigned char field_0x18;           // +0x18 created flag
    String field_0x1c;                  // +0x1c debug name
    unsigned char field_0x28;           // +0x28
    GLuint field_0x2c;                  // +0x2c extra renderbuffer id
    GLuint field_0x30;                  // +0x30 extra renderbuffer id
    GLuint field_0x34;                  // +0x34 extra renderbuffer id

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
