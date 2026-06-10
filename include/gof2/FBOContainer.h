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

struct Engine;



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


} // namespace AbyssEngine

struct FBOContainer {
    GLuint field_0x0;                   // +0x0
    GLuint field_0x4;                   // +0x4
    GLuint field_0x8;                   // +0x8
    GLsizei field_0xc;                  // +0xc
    GLsizei field_0x10;                 // +0x10
    Engine* field_0x14;                 // +0x14
    unsigned char field_0x18;           // +0x18
};
#endif
