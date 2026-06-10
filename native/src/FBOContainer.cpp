#include "gof2/FBOContainer.h"
#include "gof2/Engine.h"




// ---- Activate_7fa26.cpp ----
namespace AbyssEngine {

void FBOContainer::Activate() {
    glBindTexture(0xde1, this->field_0x4);
}

} // namespace AbyssEngine

// ---- BeginCapture_7f9f8.cpp ----
namespace AbyssEngine {

void FBOContainer::BeginCapture() {
    glBindFramebuffer(0x8d40, this->field_0x0);
    glViewport(0, 0, this->field_0xc, this->field_0x10);
}

} // namespace AbyssEngine

// ---- Create_7f8d4.cpp ----
namespace AbyssEngine {

void FBOContainer::Create(int width, int height, bool a, bool linear) {
    this->field_0xc = width;
    this->field_0x10 = height;
    glGenFramebuffers(1, (GLuint *)((char *)this + 0x0));
    glBindFramebuffer(0x8d40, this->field_0x0);
    glGenTextures(1, (GLuint *)((char *)this + 0x4));
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
    glGenRenderbuffers(1, (GLuint *)((char *)this + 0x8));
    glBindRenderbuffer(0x8d41, this->field_0x8);
    glRenderbufferStorage(0x8d41, 0x81a5, this->field_0xc,
                          this->field_0x10);
    glFramebufferRenderbuffer(0x8d40, 0x8d00, 0x8d41, this->field_0x8);
    glCheckFramebufferStatus(0x8d40);
    this->field_0x18 = 1;
    Engine *engine = this->field_0x14;
    glBindFramebuffer(0x8d40, engine->field_0x40c);
}

} // namespace AbyssEngine

// ---- Release_7f868.cpp ----
namespace AbyssEngine {

void FBOContainer::Release() {
    if (this->field_0x18 == 0) {
        return;
    }
    glDeleteFramebuffers(1, (GLuint *)((char *)this + 0x0));
    this->field_0x0 = 0;
    glDeleteTextures(1, (GLuint *)((char *)this + 0x4));
    this->field_0x4 = 0;
    glDeleteRenderbuffers(1, (GLuint *)((char *)this + 0x8));
    this->field_0x8 = 0;
    this->field_0x18 = 0;
    glDeleteRenderbuffers(1, (GLuint *)((char *)this + 0x30));
    glDeleteRenderbuffers(1, (GLuint *)((char *)this + 0x34));
    glBindFramebuffer(0x8d40, 0);
    return glDeleteRenderbuffers(1, (GLuint *)((char *)this + 0x2c));
}

} // namespace AbyssEngine

// ---- FBOContainer_7f7f4.cpp ----
namespace AbyssEngine {

// AbyssEngine::FBOContainer::FBOContainer(AbyssEngine::Engine*, AbyssEngine::String)
FBOContainer::FBOContainer(Engine *engine, String name)
{
    char *self = (char *)this;

    // Construct the embedded String at +0x1c.
    new ((void *)(self + 0x1c)) String();

    *(unsigned char *)(self + 0x18) = 0;
    *(unsigned char *)(self + 0x28) = 0;

    // Zero framebuffer / texture / renderbuffer / width and clear height.
    *(int *)(self + 0x0) = 0;
    *(int *)(self + 0x4) = 0;
    *(int *)(self + 0x8) = 0;
    *(int *)(self + 0xc) = 0;
    *(int *)(self + 0x10) = 0;

    // Copy the supplied name into the embedded String.
    *(String *)(self + 0x1c) = name;

    *(Engine **)(self + 0x14) = engine;
}

} // namespace AbyssEngine

// ---- EndCapture_7fa18.cpp ----
namespace AbyssEngine {

void FBOContainer::EndCapture() {
    Engine *engine = this->field_0x14;
    glBindFramebuffer(0x8d40, engine->field_0x40c);
}

} // namespace AbyssEngine

// ---- _FBOContainer_7f842.cpp ----
namespace AbyssEngine {

FBOContainer::~FBOContainer() {
    Release();
    ((String *)((char *)this + 0x1c))->~String();
}

} // namespace AbyssEngine
