

#include <GLES/gl.h>
#include "engine/core/ApplicationManager.h"
#include "engine/core/NFC.h"
#include "engine/file/AEFile.h"
#include "engine/file/FileInterfaceAndroid.h"
#include "engine/render/Engine.h"
#include "engine/render/FBOContainer.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/String.h"
#include <GLES2/gl2.h>

extern "C" { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
void glMatrixMode(GLenum) {
}

void glLoadMatrixf(const GLfloat *) {
}

void glLoadIdentity(void) {
}

void glScalef(GLfloat, GLfloat, GLfloat) {
}

void glColor4f(GLfloat, GLfloat, GLfloat, GLfloat) {
}

void glMaterialf(GLenum, GLenum, GLfloat) {
}

void glMaterialfv(GLenum, GLenum, const GLfloat *) {
}

void glLightfv(GLenum, GLenum, const GLfloat *) {
}

void glLightModelfv(GLenum, const GLfloat *) {
}

void glTexEnvf(GLenum, GLenum, GLfloat) {
}

void glVertexPointer(GLint, GLenum, GLsizei, const GLvoid *) {
}

void glTexCoordPointer(GLint, GLenum, GLsizei, const GLvoid *) {
}

void glNormalPointer(GLenum, GLsizei, const GLvoid *) {
}

void glColorPointer(GLint, GLenum, GLsizei, const GLvoid *) {
}

void glEnableClientState(GLenum) {
}

void glDisableClientState(GLenum) {
}

void glAlphaFunc(GLenum, GLclampf) {
}

void glClientActiveTexture(GLenum) {
}

void glFogf(GLenum, GLfloat) {
}

void glFogfv(GLenum, const GLfloat *) {
}

void glHint(GLenum, GLenum) {
}

void glMultMatrixf(const GLfloat *) {
}

void glShadeModel(GLenum) {
}

void glTexEnvi(GLenum, GLenum, GLint) {
}
}

// Android platform half of AbyssEngine::Engine. The original (cf. iOS Engine_iPhone.mm) keeps these
// in the platform TU next to the GLES1 stubs: in the .so they sit contiguously at 0x6daf2-0x6ddf0
// right after glMaterialf, so they live here rather than in Engine.cpp.
namespace AbyssEngine {

void Engine::SetScreenOrientation(AbyssEngine::LandscapeMode orientation) {
}

void Engine::initFileInterface() {
    FileInterfaceAndroid *fileInterface = new FileInterfaceAndroid();
    this->fileInterface = fileInterface;
    return AEFile::SetInterface((FileInterface *) fileInterface);
}

int Engine::InitGL(bool shaders, int width, int height) {
    this->refractFBO = 0;
    this->displayWidth = width;
    this->displayHeight = height;
    this->viewportWidth = width;
    this->viewportHeight = height;

    FileInterfaceAndroid *fileInterface = new FileInterfaceAndroid();
    this->fileInterface = fileInterface;
    AEFile::SetInterface((FileInterface *) fileInterface);

    this->hasVibration = 0;
    this->vibrationSupported = 0;
    this->lastGlError = 0;
    Engine::enableShader = shaders;
    this->viewFramebuffer = 0;

    this->ResetLightParam();
    glViewport(0, 0, this->viewportHeight, this->viewportWidth);
    if (Engine::enableShader != 0) {
        this->ShaderInit();
    } else {
        glEnable(0x803a);
        glDisable(0xb50);
        glLineWidth(1.0f);
    }

    {
        Vector up;
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        this->field_0x468 = up;
    }
    this->lightDirty[0] = 0.0f;
    {
        Vector up;
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        this->field_0x474 = up;
    }
    this->lightDirty[1] = 0.0f;

    glEnable(0xb71);
    this->GlEnable(0xde1, true);
    glDisable(0xbe2);
    glCullFace(0x405);
    glEnable(0xb44);
    this->AfterGLInit();
    this->appManager->paintCanvas->Initialize(false);
    this->maxTextureSize = 0;
    glGetIntegerv(0xd33, (GLint *) &this->maxTextureSize);

    if (Engine::enableShader != 0 && Engine::EnablePostEffect) {
        this->refractFBO = new FBOContainer(this, String("refractFBO"));
        this->refractFBO->Create(this->displayWidth, this->displayHeight, true, false);
    }

    return 1;
}

void Engine::SetFrameBufferScaleFactor(float factor, int slot) {
}

void Engine::ReleaseGL() {
}

void Engine::PreUpdate() {
}

void Engine::ActivateTextureFBO() {
    FBOContainer *fbo = this->postEffectFBO;
    if (fbo != 0) {
        return fbo->Activate();
    }
}

void Engine::ActivateRender2TextureFBO() {
    FBOContainer *fbo = this->postEffectFBO;
    if (fbo != 0) {
        return fbo->BeginCapture();
    }
}

void Engine::DeactivateRender2TextureFBO() {
    FBOContainer *fbo = this->postEffectFBO;
    if (fbo != 0) {
        return fbo->EndCapture();
    }
}

void Engine::ActivateRefractFBO() {
    FBOContainer *fbo = this->refractFBO;
    if (fbo != 0) {
        return fbo->Activate();
    }
}

void Engine::ActivateRender2FracFBO() {
    FBOContainer *fbo = this->refractFBO;
    if (fbo != 0) {
        return fbo->BeginCapture();
    }
}

void Engine::DeactivateRender2FracFBO() {
    FBOContainer *fbo = this->refractFBO;
    if (fbo != 0) {
        return fbo->EndCapture();
    }
}

void Engine::ActivateFrameBuffer(int slot) {
}

void Engine::ActivateViewBuffer() {
    glBindFramebuffer(0x8d40, this->viewFramebuffer);
    return glViewport(0, 0, this->viewportWidth, this->viewportHeight);
}

void Engine::CopyFBO() {
    if (Engine::enableShader == 0) {
        return;
    }

    if (this->IsPostEffectActivated()) {
        this->DeactivateRender2TextureFBO();
        this->DrawCloakFBO(this->postEffectFBO);
        this->ActivateRender2TextureFBO();
    } else {
        this->DeactivateRender2FracFBO();
        this->DrawCloakFBO(this->refractFBO);
        this->ActivateViewBuffer();
    }

    glEnable(0xb71);
    glDepthMask(1);
    glDisable(0xbe2);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    return glClear(0x100);
}

void Engine::SwapBuffer() {
    uint32_t index = 0;
    while (index < this->triangleCounts.size()) {
        this->triangleCounts[index] = 0;
        index += 1;
    }
}

void Engine::GrabFrameBuffer() {
}

void Engine::SaveImageToPhotosAlbum() {
}

void *Engine::GetJPEGImageData(float quality) { // lint: void_ptr return type fixed by Engine.h declaration
    // lint: void_ptr return type fixed by Engine.h declaration
    return nullptr;
}

DeviceInfo Engine::GetDeviceInfo() {
    DeviceInfo info;
    info.isPad = NFC::isPad();
    info.width = NFC::getWidth();
    info.height = NFC::getHeight();
    return info;
}

}
