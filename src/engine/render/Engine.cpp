// Engine is referenced at global scope by the engine glue (externs.h), so it must be
// visible before that header is parsed.
class Engine;

#include "gof2/externs.h"
#include "gof2/platform/gl.h"
#include "gof2/engine/math/AEMath.h"
#include "gof2/engine/render/FBOContainer.h"
#include "gof2/engine/render/Engine.h"
Engine* gEngine = nullptr;            // canonical Engine singleton
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/engine/core/NFC.h"
#include "gof2/engine/file/AEFile.h"
#include "gof2/game/core/String.h"
#include "gof2/engine/render/Mesh.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/engine/render/ShaderBaseStruct.h"
#include <arm_neon.h>

using AbyssEngine::FBOContainer;
using AbyssEngine::ShaderBaseStruct;
using AbyssEngine::AEMath::MatrixGetGL;
using AbyssEngine::AEMath::VectorNormalize;
using AbyssEngine::AEMath::MatrixInverseRotateVector;
using AbyssEngine::AEMath::MatrixInverseTransformVector;

// ARM EABI runtime helpers emitted as explicit calls (host-build externs).
extern "C" void *__aeabi_memcpy(void *dest, const void *src, unsigned long n);
extern "C" unsigned int __aeabi_uidiv(unsigned int num, unsigned int den);

// Fixed-function (GLES1) entry points not covered by platform/gl.h.
extern "C" void glMatrixMode(unsigned int mode);
extern "C" void glLoadMatrixf(const float *matrix);
extern "C" void glLoadIdentity();
extern "C" void glScalef(float x, float y, float z);
extern "C" void glColorMask(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);
extern "C" void glDepthFunc(unsigned int func);
extern "C" const char *glGetString(unsigned int name);
extern "C" void glMaterialf(unsigned int face, unsigned int pname, float value);
extern "C" void glVertexPointer(int size, unsigned int type, int stride, const void *ptr);
extern "C" void glTexCoordPointer(int size, unsigned int type, int stride, const void *ptr);
extern "C" void glNormalPointer(unsigned int type, int stride, const void *ptr);
extern "C" void glColorPointer(int size, unsigned int type, int stride, const void *ptr);
extern "C" void glDrawElements(unsigned int mode, int count, unsigned int type, const void *indices);
extern "C" void glDrawArrays(unsigned int mode, int first, int count);
extern "C" void glGetIntegerv(unsigned int name, void *out);
extern "C" void glColor4f(float red, float green, float blue, float alpha);
extern "C" void glTexEnvf(unsigned int target, unsigned int pname, float value);
extern "C" void glEnableClientState(unsigned int array);
extern "C" void glDisableClientState(unsigned int array);
extern "C" void glMaterialfv(unsigned int face, unsigned int pname, const void *params);
extern "C" void glLightModelfv(unsigned int pname, const void *params);
extern "C" void glLineWidth(float width);
extern "C" void glCullFace(unsigned int mode);
extern "C" void glLightfv(unsigned int light, unsigned int pname, const void *params);
extern "C" void glGetError();

// Engine free-functions / host glue resolved by a later externs pass.
extern "C" void FBOContainer_ActivateRender2Texture(FBOContainer *self);
extern "C" void FBOContainer_ActivateTexture(FBOContainer *self);
extern "C" void FBOContainer_DeactivateRender2Texture(FBOContainer *self);
extern "C" void ShaderUpdateRimColor();
extern "C" void ShaderUpdateMaterialColor();
extern "C" void FileInterfaceAndroid_ctor(void *self);
extern "C" void ShaderCtor_0(void *);
extern "C" void ShaderCtor_1(void *);
extern "C" void ShaderCtor_2(void *);
extern "C" void ShaderCtor_3(void *);
extern "C" void ShaderCtor_4(void *);
extern "C" String *g_Engine_vendorString;
extern "C" String *g_Engine_rendererString;
void MeshRelease(Engine *self, void *meshSlot);
void MeshCreate(Engine *self, int vertices, int faces, int flags, void *outMesh);
void esMatrixMultiply(void *out, const void *lhs, const void *rhs);

double * Engine::GetAccelValue() {
    double x = this->field_0x4b0;
    double y;
    if (*(int *)(*this->field_0x30 + 0x30) == 1) {
        x = -x;
        y = -this->field_0x4b8;
    } else {
        y = this->field_0x4b8;
    }
    this->field_0x4c8 = x;
    this->field_0x4d0 = y;
    this->field_0x4d8 = this->field_0x4c0;
    return (double *)&this->field_0x4c8;
}

void Engine::ActivateRender2FracFBO() {
    FBOContainer *fbo = this->field_0x418;
    if (fbo != 0) {
        return FBOContainer_ActivateRender2Texture(fbo);
    }
}

uint32_t Engine::Resume() {
    ((PaintCanvas*)*this->field_0x30)->Resume();
    for (int index = 0; index != 0x14; index += 1) {
        this->boundTextures[index] = -1;
    }
    return 1;
}

uint32_t Engine::Suspend() {
    ((PaintCanvas*)*this->field_0x30)->Suspend();
    return 1;
}

uint32_t Engine::GetDisplayWidth() {
    return this->field_0x368;
}

double * Engine::GetGravValue() {
    double x = this->field_0x4e0;
    double y;
    if (*(int *)(*this->field_0x30 + 0x30) == 1) {
        x = -x;
        y = -this->field_0x4e8;
    } else {
        y = this->field_0x4e8;
    }
    this->field_0x4f8 = x;
    this->field_0x500 = y;
    this->field_0x508 = this->field_0x4f0;
    return (double *)&this->field_0x4f8;
}

uint32_t Engine::GetDisplayHeight() {
    return this->field_0x36c;
}

void Engine::LightSetRimColor(float red, float green, float blue) {
    if (g_Engine_useShaders == 0) {
        return;
    }
    this->field_0x320 = red;
    this->field_0x324 = green;
    this->field_0x328 = blue;
    return ShaderUpdateRimColor();
}

bool Engine::IsPostEffectActivated() {
    return this->field_0x410 != 0;
}

void Engine::SetUVMatrix(const uint32_t *matrix) {
    if (g_Engine_useShaders == 0) {
        glMatrixMode(0x1702);
        MatrixGetGL(*(const Matrix *)matrix, this->uvMatrixGL);
        glLoadMatrixf(this->uvMatrixGL);
        return glMatrixMode(0x1700);
    }

    uint32_t m0 = matrix[0];
    uint32_t m1 = matrix[1];
    uint32_t m2 = matrix[2];
    uint32_t m3 = matrix[3];
    uint32_t m4 = matrix[4];
    uint32_t m5 = matrix[5];
    uint32_t m6 = matrix[6];
    uint32_t m7 = matrix[7];
    uint32_t m8 = matrix[8];
    uint32_t m9 = matrix[9];
    uint32_t m10 = matrix[10];
    uint32_t m11 = matrix[11];

    uint32_t *uv = (uint32_t *)this->uvMatrix;
    uv[0] = m0;
    uv[1] = m4;
    uv[2] = m8;
    uv[3] = 0;
    uv[4] = m1;
    uv[5] = m5;
    uv[6] = m9;
    uv[7] = 0;
    uv[8] = m2;
    uv[9] = m6;
    uv[10] = m10;
    uv[11] = 0;
    uv[12] = m3;
    uv[13] = m7;
    uv[14] = m11;
    uv[15] = 0x3f800000;
}

void Engine::ActivateRender2TextureFBO() {
    FBOContainer *fbo = this->field_0x414;
    if (fbo != 0) {
        return FBOContainer_ActivateRender2Texture(fbo);
    }
}

typedef void Materialfv(unsigned int face, unsigned int pname, const void *params);
static Materialfv * volatile g_Engine_glMaterialfv;

void Engine::LightSetMaterialColorAlpha(float alpha) {
    if (this->field_0x424 == 0) {
        return;
    }

    this->field_0x2b4 = alpha;
    this->field_0x488 = alpha;
    Materialfv *materialfv = g_Engine_glMaterialfv;
    materialfv(0x408, 0x1200, &this->field_0x2a8);
    this->field_0x2c4 = this->field_0x488;
    materialfv(0x408, 0x1202, &this->field_0x2b8);
    this->field_0x2a4 = this->field_0x488;
    return materialfv(0x408, 0x1201, &this->field_0x298);
}

void Engine::SetAccelValue(double x, double y, double z) {
    this->field_0x4b0 = x;
    this->field_0x4b8 = y;
    this->field_0x4c0 = z;
}

void Engine::ResetUVMatrix() {
    if (g_Engine_useShaders != 0) {
        uint32_t one = 0x3f800000;
        uint32x4_t zero = vdupq_n_u32(0);
        uint32_t *uv = (uint32_t *)this->uvMatrix;
        uv[0] = one;
        uv[5] = one;
        uv[10] = one;
        uv[15] = one;
        vst1q_u32(uv + 1, zero);
        vst1q_u32(uv + 6, zero);
        vst1q_u32(uv + 11, zero);
        return;
    }

    glMatrixMode(0x1702);
    glLoadIdentity();
    glScalef(1.0f, -1.0f, 1.0f);
    return glMatrixMode(0x1700);
}

void Engine::ActivateTextureFBO() {
    FBOContainer *fbo = this->field_0x414;
    if (fbo != 0) {
        return FBOContainer_ActivateTexture(fbo);
    }
}

void Engine::GlowEndGlow() {
    if (g_Engine_useShaders == 0) {
        return;
    }
    this->field_0x41c = 0;
    glColorMask(1, 1, 1, 1);
    return glDepthFunc(0x201);
}

void Engine::ActivateViewBuffer() {
    glBindFramebuffer(0x8d40, this->field_0x40c);
    return glViewport(0, 0, this->field_0x370, this->field_0x374);
}

void Engine::GlowEnableGlow() {
    if (this->field_0x41c != 0) {
        return;
    }
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(0x4000);
    this->field_0x41c = 1;
}

void Engine::SetOnDestroyApp(DestroyCallback *callback) {
    this->field_0x484 = callback;
}

void Engine::SetGravValue(double x, double y, double z) {
    this->field_0x4e0 = x;
    this->field_0x4e8 = y;
    this->field_0x4f0 = z;
}

void Engine::SwapBuffer() {
    uint32_t index = 0;
    while (index < this->triangleCounts->size()) {
        (*this->triangleCounts)[index] = 0;
        index += 1;
    }
}

typedef void ShaderUnload(ShaderBaseStruct *);
typedef void ShaderInitReloadFn(ShaderBaseStruct *, Engine *);

void Engine::ReloadShaders() {
    uint32_t index = 0;
    while (index < this->shaders->size()) {
        ShaderBaseStruct *shader = (*this->shaders)[index];
        void **vtable = *(void ***)shader;
        ((ShaderUnload *)vtable[0x24 / 4])(shader);

        shader = (*this->shaders)[index];
        vtable = *(void ***)shader;
        ((ShaderInitReloadFn *)vtable[0x08 / 4])(shader, this);
        index += 1;
    }
}

void Engine::DeactivateRender2TextureFBO() {
    FBOContainer *fbo = this->field_0x414;
    if (fbo != 0) {
        return FBOContainer_DeactivateRender2Texture(fbo);
    }
}

void Engine::GetDeviceInfo() {
    this->field_0x8 = NFC().isPad();
    this->field_0x0 = NFC().getWidth();
    this->field_0x4 = NFC().getHeight();
}

void Engine::CopyFBO() {
    if (g_Engine_useShaders == 0) {
        return;
    }

    if (this->IsPostEffectActivated()) {
        this->DeactivateRender2TextureFBO();
        this->DrawCloakFBO(this->field_0x414);
        this->ActivateRender2TextureFBO();
    } else {
        this->DeactivateRender2FracFBO();
        this->DrawCloakFBO(this->field_0x418);
        this->ActivateViewBuffer();
    }

    glEnable(0xb71);
    glDepthMask(1);
    glDisable(0xbe2);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    return glClear(0x100);
}

bool Engine::HasVibration() {
    if (this->field_0x480) {
        return this->field_0x2c != 0;
    }
    return false;
}

void Engine::LightSetLightCount(int count) {
    if (count >= 8) {
        count = 8;
    }
    count &= ~(count >> 31);
    this->field_0x32c = count;
}

void Engine::SetAddData(void *data, int size) {
    this->field_0x358 = data;
    this->field_0x35c = size;
}

void Engine::ShaderUpdate() {
    uint32_t index = 0;
    while (index < this->shaders->size()) {
        (*this->shaders)[index]->Update();
        index += 1;
    }
}

bool Engine::IsExtensionSupported(const char *extension)
{
    const char *extensions = glGetString(0x1f03);

    uint32_t allLength = 0;
    while (extensions[allLength] != 0) {
        allLength += 1;
    }

    uint32_t extLength = 0;
    while (extension[extLength] != 0) {
        extLength += 1;
    }

    uint32_t index = 0;
    while (index < allLength) {
        if (extensions[index] == extension[0]) {
            const char *cur = extensions + index;
            uint32_t offset = 0;
            while (offset < extLength && index + offset < allLength &&
                   extension[offset] == cur[offset]) {
                offset += 1;
                if (offset == extLength) {
                    return true;
                }
            }
        }
        index += 1;
    }
    return false;
}

void Engine::LightSetMaterialColorShininess(float shininess) {
    this->field_0x2c8 = shininess;
    if (g_Engine_useShaders == 0) {
        return glMaterialf(0x408, 0x1601, shininess);
    }
}

void Engine::Initialize(InitializeCallback *callback) {
    if (callback != 0) {
        callback(this);
    }
}

void Engine::ActivateRefractFBO() {
    FBOContainer *fbo = this->field_0x418;
    if (fbo != 0) {
        return FBOContainer_ActivateTexture(fbo);
    }
}

void Engine::LightSetParticleAmbient(float red, float green, float blue) {
    this->particleAmbient.x = red;
    this->particleAmbient.y = green;
    this->particleAmbient.z = blue;
}

void Engine::DeactivateRender2FracFBO() {
    FBOContainer *fbo = this->field_0x418;
    if (fbo != 0) {
        return FBOContainer_DeactivateRender2Texture(fbo);
    }
}

void Engine::SetPerspMatrix(const uint32_t *matrix) {
    if (g_Engine_useShaders == 0) {
        return;
    }
    uint32_t *proj = (uint32_t *)this->projMatrix;
    for (int i = 0; i < 16; i += 1) {
        proj[i] = matrix[i];
    }
}

void Engine::SetFrameBufferTexture(int slot0, int slot1) {
    int firstValue = this->frameBufferTextures[slot0];
    if (firstValue != -1) {
        glActiveTexture(0x84c0);
        glBindTexture(0xde1, firstValue);
    }

    if (slot1 == -1) {
        return;
    }
    int secondValue = this->frameBufferTextures[slot1];
    if (secondValue == -1) {
        return;
    }
    glActiveTexture(0x84c1);
    return glBindTexture(0xde1, secondValue);
}

void Engine::LightSetLightDirection(float x, float y, float z, unsigned int light) {
    unsigned int index = light - 0x4000;
    if (index < 8) {
        int count = light - 0x3fff;
        int current = this->field_0x32c;
        if (current > count) {
            count = current;
        }
        this->field_0x32c = count;

        Vector input;
        input.x = x;
        input.y = y;
        input.z = z;
        Vector normalized = AbyssEngine::AEMath::VectorNormalize(input);
        (&this->field_0x468)[index] = normalized;
        this->lightDirty[index] = 0.0f;
    }
    return;
}

void Engine::RenderMesh(MeshFull *mesh) {
    if (mesh == 0 || mesh->field_0x28 == 0) {
        goto done;
    }

    if (g_Engine_useShaders == 0) {
        glVertexPointer(3, 0x1406, 0, mesh->field_0x4);
        this->AEClientState(0x8074, true);
        bool tex = ((uint32_t)*(uint8_t *)mesh << 30) < 0;
        if (tex && (mesh->field_0x30 == 0 ||
                    *(int *)((char *)mesh->field_0x30 + 4) == -1)) {
            glTexCoordPointer(2, 0x1406, 0, mesh->field_0x8);
        }
        this->AEClientState(0x8078, tex);
        bool normals = ((uint32_t)*(uint8_t *)mesh << 29) < 0;
        if (normals) {
            glNormalPointer(0x1406, 0, mesh->field_0x10);
        }
        this->AEClientState(0x8075, normals);
        bool colors = ((uint32_t)*(uint8_t *)mesh << 28) < 0;
        if (colors) {
            glColorPointer(4, 0x1406, 0, (void *)(uintptr_t)mesh->field_0xc);
        }
        this->AEClientState(0x8076, colors);
        if (((uint32_t)*(uint8_t *)mesh << 27) < 0) {
            glDrawElements(4, mesh->field_0x28, 0x1403,
                           mesh->field_0x2c);
        } else {
            glDrawArrays(4, 0, mesh->field_0x2);
        }
        if (tex && mesh->field_0x30 != 0 &&
            *(int *)((char *)mesh->field_0x30 + 4) != -1) {
            this->AEClientState(0x8078, false);
        }
    } else {
        g_Engine_shaderDrew = 0;
        this->ShaderSetActive(g_Engine_defaultShader, mesh);
        if (g_Engine_shaderDrew != 0) {
            int oldBuffer = 0;
            glGetIntegerv(0x8ca6, &oldBuffer);
            if (((uint32_t)*(uint8_t *)mesh << 27) < 0) {
                if (mesh->field_0x5c == 0) {
                    glDrawElements(4, mesh->field_0x28, 0x1403,
                                   mesh->field_0x2c);
                } else {
                    glBindBuffer(0x8893, mesh->field_0x64);
                    glDrawElements(4, mesh->field_0x28, 0x1403, 0);
                    glBindBuffer(0x8892, 0);
                    glBindBuffer(0x8893, 0);
                }
            } else {
                glDrawArrays(4, 0, mesh->field_0x2);
            }
            this->ShaderSetInActive();
        }
    }

done:
    return;
}

void Engine::DrawQuad(int x, int y, int width, int height) {
    float fx = (float)x;
    float fy = (float)y;
    float right = (float)(x + width);
    float bottom = (float)(height + y);

    MeshFull *mesh = (MeshFull *)this->field_0x380;
    float *positions = (float *)mesh->field_0x4;
    positions[0] = fx;
    positions[1] = fy;
    positions[3] = right;
    positions[4] = fy;
    positions[6] = right;
    positions[7] = bottom;
    positions[9] = fx;
    positions[10] = bottom;

    static const float uvs[8] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };
    float *uv = (float *)mesh->field_0x8;
    float32x4_t uv0 = vld1q_f32(uvs);
    float32x4_t uv1 = vld1q_f32(uvs + 4);
    vst1q_f32(uv, uv0);
    vst1q_f32(uv + 4, uv1);

    return glDrawElements(4, mesh->field_0x28, 0x1403,
                          mesh->field_0x2c);
}

void Engine::SetColor(float red, float green, float blue, float alpha) {
    Engine *self = this;
    if (self->field_0xd0 == red &&
        self->field_0xd4 == green &&
        self->field_0xd8 == blue &&
        self->field_0xdc == alpha) {
        return;
    }
    self->field_0xd4 = green;
    self->field_0xd0 = red;
    self->field_0xd8 = blue;
    self->field_0xdc = alpha;
    self->field_0xe0 =
        (int)(green * 255.0f) * 0x10000 + (int)(red * 255.0f) * 0x1000000 +
        (int)(blue * 255.0f) * 0x100 + (int)(alpha * 255.0f);
    if (g_Engine_useShaders != 0) {
        return ShaderUpdateMaterialColor();
    }
    self->LightSetMaterialColorAlpha(alpha);
    return glColor4f(red, green, blue, alpha);
}

typedef void FileInterfaceRelease(void *);

Engine::~Engine()
{
    DestroyCallback *destroy = this->field_0x484;
    if (destroy != 0) {
        destroy(this);
    }

    delete (ApplicationManager *)this->field_0x30;
    this->field_0x30 = 0;

    void *fileInterface = this->field_0x24;
    if (fileInterface != 0) {
        void **vtable = *(void ***)fileInterface;
        ((FileInterfaceRelease *)vtable[1])(fileInterface);
    }
    this->field_0x24 = 0;

    AEFile::Release();
    for (uint32_t i = 0; i < this->shaders->size(); ++i) {
        delete (*this->shaders)[i];
    }
    this->shaders->clear();

    delete this->field_0x414;
    this->field_0x414 = 0;

    delete this->field_0x418;
    this->field_0x418 = 0;

    MeshRelease(this, &this->field_0x380);
    this->ReleaseGL();
    delete this->shaders;
    delete this->triangleCounts;
    // str_0x14 / str_0x3c / str_0x4c are real String members: auto-destructed.
}

// Releases GL-side resources owned by the engine. Empty in this build (GL teardown is
// handled by the driver / deferred), but kept as a real method invoked from ~Engine().
void Engine::ReleaseGL() {
}

void Engine::AfterGLInit() {
    this->ResetLightParam();
    MeshCreate(this, 4, 2, 0x13, &this->field_0x380);

    uint32_t *indices = *(uint32_t **)(this->field_0x380 + 0x2c);
    indices[0] = 0x20000;
    indices[1] = 1;
    indices[2] = 0x30002;

    String vendor(glGetString(0x1f00));
    g_Engine_vendorString->assign(&vendor);
    String renderer(glGetString(0x1f01));
    g_Engine_rendererString->assign(&renderer);
}

typedef void ShaderDrawCloak(ShaderBaseStruct *);

void Engine::DrawCloakFBO(FBOContainer *fbo) {
    (void)fbo;   // unused in the shader path
    if (g_Engine_useShaders != 0) {
        ShaderBaseStruct *shader = (*this->shaders)[g_Engine_cloakShader];
        void **vtable = *(void ***)shader;
        return ((ShaderDrawCloak *)vtable[0x14 / 4])(shader);
    }
}

typedef void ShaderInitFn(ShaderBaseStruct *, Engine *);

void Engine::ShaderRegister(ShaderBaseStruct *shader) {
    if (shader != 0) {
        String name = shader->GetShaderName();
        char *text = name.GetAEChar();

        void **vtable = *(void ***)shader;
        ((ShaderInitFn *)vtable[0x08 / 4])(shader, this);
        this->shaders->push_back(shader);
        this->triangleCounts->push_back(0);
        ::operator delete(text);
    }
}

void Engine::SetTextureSlot(uint32_t textureIndex, uint32_t slot) {
    char *manager = *this->field_0x30;   // external texture-manager (no named struct)
    uint32_t count = *(uint32_t *)(manager + 0x10);
    if (count == 0 || slot >= 8 || textureIndex > count - 1) {
        return;
    }
    uint32_t *bound = (uint32_t *)&this->boundTextures[slot];
    void *textureEntry = *(void **)(*(char **)(manager + 0x14) + textureIndex * 4);
    uint32_t texture = **(uint32_t **)(&textureEntry);
    if (*bound == texture) {
        return;
    }
    glActiveTexture(slot + 0x84c0);
    float env = *(float *)((char *)textureEntry + 0x10);
    if (g_Engine_texEnv != env) {
        g_Engine_texEnv = env;
        if (g_Engine_useShaders == 0) {
            glTexEnvf(0x8500, 0x8501, env);
            textureEntry = *(void **)(*(char **)(manager + 0x14) + textureIndex * 4);
        } else if (g_Engine_texEnvDirty != 0) {
            g_Engine_texEnvDirty = 0;
        }
    }
    glBindTexture(*(uint8_t *)((char *)textureEntry + 0x14) == 0 ? 0xde1 : 0x8513, texture);
    *bound = texture;
}

void Engine::AEClientState(unsigned int state, bool enable) {
    uint32_t bits = this->field_0x4a4;
    uint32_t mask = 0;
    switch (state) {
    case 0x8074: mask = 2; break;
    case 0x8075: mask = 1; break;
    case 0x8076: mask = 8; break;
    case 0x8078: mask = 4; break;
    default: return;
    }
    if (enable) {
        if ((bits & mask) != 0) {
            return;
        }
        glEnableClientState(state);
        bits |= mask;
    } else {
        if ((bits & mask) == 0) {
            return;
        }
        glDisableClientState(state);
        bits &= ~mask;
    }
    this->field_0x4a4 = bits;
}

void Engine::GlowBeginGlow(unsigned int depthFunc) {
    if (this->field_0x41c != 0) {
        return;
    }
    if (g_Engine_useShaders == 0) {
        return;
    }
    glColorMask(0, 0, 0, 1);
    this->GlowEnableGlow();
    if (this->field_0x41c != 0) {
        return glDepthFunc(depthFunc);
    }
}

void Engine::DrawLine2D(int vertexCount, int count, bool strip) {
    this->field_0x348 = vertexCount;
    this->ShaderSetActive(g_Engine_lineShader, 0);
    unsigned int mode = strip != 0 ? 2 : 1;
    return glDrawArrays(mode, 0, count);
}

typedef void ShaderEnable(ShaderBaseStruct *, bool);
typedef void ShaderApply(ShaderBaseStruct *, MeshFull *, Engine *);

void Engine::ShaderSetActive(int shaderIndex, MeshFull *mesh) {
    while (shaderIndex == -1) {
        shaderIndex = g_Engine_defaultShader;
        if ((((uint32_t)*(uint8_t *)mesh) << 30) >= 0) {
            shaderIndex = g_Engine_altShader;
        }
    }

    bool hasExtra = mesh != 0 && mesh->field_0x85 != 0;
    ShaderBaseStruct *shader = (*this->shaders)[shaderIndex];
    if (shader == 0) {
        return;
    }
    g_Engine_shaderDirty = 1;

    void **vtable = *(void ***)shader;
    if (((ShaderBaseStruct *)shader)->program != this->field_0x3e4) {
        ((ShaderEnable *)vtable[0x28 / 4])(shader, hasExtra);
        shader = (*this->shaders)[shaderIndex];
        this->field_0x3e4 = ((ShaderBaseStruct *)shader)->program;
        g_Engine_currentShader = shaderIndex;
        vtable = *(void ***)shader;
    }
    ((ShaderEnable *)vtable[0x28 / 4])(shader, hasExtra);
    shader = (*this->shaders)[shaderIndex];
    vtable = *(void ***)shader;
    ((ShaderApply *)vtable[0x0c / 4])(shader, mesh, this);
    if (mesh != 0) {
        uint32_t triangles = __aeabi_uidiv(mesh->field_0x28, 3);
        (*this->triangleCounts)[shaderIndex] += triangles;
    }
}

typedef void ShaderPostDraw(ShaderBaseStruct *, void *);
typedef void ShaderPostDrawSwap(ShaderBaseStruct *, void *, void **, Engine *);

void Engine::DoPostEffect() {
    uint32_t flags = this->field_0x410;
    void *current = this->field_0x414;
    void *other = this->field_0x418;
    if (g_Engine_useShaders != 0) {
        void *slot = other;
        if ((flags & 2) != 0) {
            ShaderBaseStruct *shader = (*this->shaders)[g_Engine_shaderPostA];
            void **vtable = *(void ***)shader;
            if ((flags & ~2u) == 0) {
                ((ShaderPostDraw *)vtable[0x14 / 4])(shader, current);
                flags = 0;
            } else {
                ((ShaderPostDrawSwap *)vtable[0x1c / 4])(shader, current, &other, this);
                slot = other;
                other = current;
            }
        }
        if ((this->field_0x410 & 1) != 0) {
            ShaderBaseStruct *shader = (*this->shaders)[g_Engine_shaderPostB];
            void **vtable = *(void ***)shader;
            if ((flags & ~1u) == 0) {
                ((ShaderPostDraw *)vtable[0x14 / 4])(shader, slot);
                flags = 0;
            } else {
                ((ShaderPostDrawSwap *)vtable[0x1c / 4])(shader, slot, &other, this);
                slot = other;
            }
        }
        if ((this->field_0x410 & 4) != 0) {
            ShaderBaseStruct *shader = (*this->shaders)[g_Engine_shaderPostC];
            void **vtable = *(void ***)shader;
            if ((flags & ~4u) == 0) {
                ((ShaderPostDraw *)vtable[0x18 / 4])(shader, slot);
            } else {
                ((ShaderPostDrawSwap *)vtable[0x20 / 4])(shader, slot, &other, this);
            }
        }
        if (g_Engine_postEffectFlag == 1) {
            this->SetPostEffect(g_Engine_postEffectBW, false);
        }
    }
    return;
}

void Engine::LightSetMaterialColorSpecular(float red, float green, float blue) {
    this->field_0x2b8 = red;
    this->field_0x2bc = green;
    this->field_0x2c0 = blue;
    this->field_0x2c4 = this->field_0x488;

    if (g_Engine_useShaders == 0) {
        return glMaterialfv(0x408, 0x1202, &this->field_0x2b8);
    }

    int count = this->field_0x32c;
    for (int index = 0; index < count; index += 1) {
        const LightColor &src = (&this->lightSpecular)[index];
        Vector &dst = (&this->lightSpecularShaded)[index];
        dst.x = src.r * red;
        dst.y = src.g * green;
        dst.z = src.b * blue;
    }
    return ShaderUpdateMaterialColor();
}

void Engine::LightSetGlobalSceneColorAmbient(float red, float green, float blue) {
    this->field_0x288 = red;
    this->field_0x28c = green;
    this->field_0x290 = blue;
    this->field_0x294 = 0x3f800000;

    if (g_Engine_useShaders == 0) {
        return glLightModelfv(0xb53, &this->field_0x288);
    }

    int count = this->field_0x32c;
    for (int index = 0; index < count; index += 1) {
        const LightColor &src = (&this->lightAmbient)[index];
        Vector &dst = (&this->lightAmbientShaded)[index];
        dst.x = (src.r + red) * this->field_0x2a8;
        dst.y = (src.g + green) * this->field_0x2ac;
        dst.z = (src.b + blue) * this->field_0x2b0;
    }
    return ShaderUpdateMaterialColor();
}

void Engine::SetPostEffect(uint32_t effect, bool enable) {
    if (this->field_0x414 == 0 && enable) {
        this->field_0x414 = new FBOContainer(this, String("posteffect"));
        int width;
        int height;
        if (*(int *)(*this->field_0x30 + 0x30) == 2) {
            width = this->field_0x368;
            height = this->field_0x36c;
        } else {
            width = this->field_0x36c;
            height = this->field_0x368;
        }
        this->field_0x414->Create(width, height, false, true);
    }

    uint32_t flags = this->field_0x410;
    if (effect == (uint32_t)g_Engine_postEffectBW) {
        if (enable) {
            if (g_Engine_postEffectCounter > 0) {
                g_Engine_postEffectCounter -= 1;
            } else {
                flags |= 4;
            }
        } else if (g_Engine_postEffectFlag == 1) {
            flags &= ~4u;
            g_Engine_postEffectFlag = 0;
            g_Engine_postEffectPending = 1;
        } else {
            g_Engine_postEffectFlag = g_Engine_postEffectCounter < 1;
        }
    } else if (effect == (uint32_t)g_Engine_postEffectBlur) {
        g_Engine_postEffectFlag = enable;
        flags = enable ? (flags | 2) : (flags & ~2u);
    } else if (effect == 0x1400000) {
        flags = enable ? (flags | 1) : (flags & ~1u);
    }
    this->field_0x410 = flags;
    return;
}

void Engine::LightSetMaterialColorDiffuse(float red, float green, float blue) {
    this->field_0x298 = red;
    this->field_0x29c = green;
    this->field_0x2a0 = blue;
    this->field_0x2a4 = this->field_0x488;

    if (g_Engine_useShaders == 0) {
        return glMaterialfv(0x408, 0x1201, &this->field_0x298);
    }

    int lightCount = this->field_0x32c;
    for (int i = 0; i < lightCount; i += 1) {
        const LightColor &src = (&this->lightDiffuse)[i];
        Vector &dst = (&this->lightDiffuseShaded)[i];
        dst.x = src.r * red;
        dst.y = src.g * green;
        dst.z = src.b * blue;
    }
    return ShaderUpdateMaterialColor();
}

void Engine::initFileInterface() {
    void *fileInterface = operator new(0x38);
    FileInterfaceAndroid_ctor(fileInterface);
    this->field_0x24 = fileInterface;
    return AEFile::SetInterface((FileInterface *)fileInterface);
}

void Engine::SetOrthoMatrix(const uint32_t *projection, const uint32_t *view, bool multiply) {
    if (g_Engine_useShaders != 0) {
        uint32_t *proj = (uint32_t *)this->projMatrix;
        for (int i = 0; i < 16; i += 1) {
            proj[i] = projection[i];
        }
        if (multiply) {
            uint32_t local[16];
            __aeabi_memcpy(local, view, 0x40);
            esMatrixMultiply(this->projMatrix, local, this->projMatrix);
        }
    }
    return;
}

int Engine::InitGL(bool shaders, int width, int height) {
    this->field_0x418 = 0;
    this->field_0x368 = width;
    this->field_0x36c = height;
    this->field_0x370 = width;
    this->field_0x374 = height;

    void *fileInterface = operator new(0x38);
    FileInterfaceAndroid_ctor(fileInterface);
    this->field_0x24 = fileInterface;
    AEFile::SetInterface((FileInterface *)fileInterface);

    this->field_0x10 = 0;
    this->field_0x2c = 0;
    this->field_0x480 = 0;
    g_Engine_useShaders = shaders;
    this->field_0x40c = 0;

    this->ResetLightParam();
    glViewport(0, 0, this->field_0x374, this->field_0x370);
    if (g_Engine_useShaders != 0) {
        this->ShaderInit();
    } else {
        glEnable(0x803a);
        glDisable(0xb50);
        glLineWidth(1.0f);
    }

    Vector value;
    value.x = 0.0f;
    value.y = 1.0f;
    value.z = 0.0f;
    this->field_0x468 = value;
    this->lightDirty[0] = 0.0f;
    this->field_0x474 = value;
    this->lightDirty[1] = 0.0f;

    glEnable(0xb71);
    this->GlEnable(0xde1, true);
    glDisable(0xbe2);
    glCullFace(0x405);
    glEnable(0xb44);
    this->AfterGLInit();
    ((PaintCanvas*)*this->field_0x30)->Initialize(false);
    this->field_0xc = 0;
    glGetIntegerv(0xd33, &this->field_0xc);

    if (g_Engine_useShaders != 0 && g_Engine_supportsFBO != 0) {
        FBOContainer *fbo = new FBOContainer(this, String("refract"));
        this->field_0x418 = fbo;
        fbo->Create(this->field_0x368, this->field_0x36c, false, true);
    }

    return 1;
}

void Engine::ClearBuffer(uint32_t color)
{
    const double scale = 255.0;
    double red = (double)(color >> 24) / scale;
    double green = (double)((color >> 16) & 0xff) / scale;
    double blue = (double)((color >> 8) & 0xff) / scale;
    double alpha = (double)(color & 0xff) / scale;
    glClearColor((float)red, (float)green, (float)blue, (float)alpha);
    return glClear(0x4100);
}

void Engine::LightSetLightPosition(float x, float y, float z, unsigned int light) {
    unsigned int index = light - 0x4000;
    if (index < 8) {
        int count = light - 0x3fff;
        int current = this->field_0x32c;
        if (current > count) {
            count = current;
        }
        this->field_0x32c = count;

        Vector value;
        value.x = x;
        value.y = y;
        value.z = z;
        (&this->field_0x468)[index] = value;
        this->lightDirty[index] = 1.0f;
    }
    return;
}

void Engine::LightSetLightColorAmbient(float red, float green, float blue, unsigned int light) {
    unsigned int index = light - 0x4000;
    if (index > 7) {
        return;
    }
    int count = light - 0x3fff;
    int current = this->field_0x32c;
    if (current > count) {
        count = current;
    }
    this->field_0x32c = count;

    LightColor &src = (&this->lightAmbient)[index];
    src.r = red;
    src.g = green;
    src.b = blue;
    src.a = 1.0f;
    if (g_Engine_useShaders == 0) {
        return glLightfv(light, 0x1200, &src.r);
    }
    Vector &dst = (&this->lightAmbientShaded)[index];
    dst.x = (this->field_0x288 + red) * this->field_0x2a8;
    dst.y = (this->field_0x28c + src.g) * this->field_0x2ac;
    dst.z = (this->field_0x290 + src.b) * this->field_0x2b0;
    return ShaderUpdateMaterialColor();
}

typedef void ShaderInactive(ShaderBaseStruct *);

void Engine::ShaderSetInActive() {
    ShaderBaseStruct *shader = (*this->shaders)[g_Engine_activeShader];
    void **vtable = *(void ***)shader;
    return ((ShaderInactive *)vtable[0x10 / 4])(shader);
}

void Engine::LightSetLightColorDiffuse(float red, float green, float blue, unsigned int light) {
    unsigned int index = light - 0x4000;
    if (index > 7) {
        return;
    }
    int count = light - 0x3fff;
    int current = this->field_0x32c;
    if (current > count) {
        count = current;
    }
    this->field_0x32c = count;

    LightColor &src = (&this->lightDiffuse)[index];
    src.r = red;
    src.g = green;
    src.b = blue;
    src.a = 1.0f;
    if (g_Engine_useShaders == 0) {
        return glLightfv(light, 0x1201, &src.r);
    }
    Vector &dst = (&this->lightDiffuseShaded)[index];
    dst.x = this->field_0x298 * red;
    dst.y = src.g * this->field_0x29c;
    dst.z = src.b * this->field_0x2a0;
    return ShaderUpdateMaterialColor();
}

Engine::Engine() {
    Engine *self = this;
    // str_0x14 / str_0x3c / str_0x4c are real String members: auto-constructed.
    self->field_0x340 = 0;
    self->field_0x34c = 0;
    self->field_0x354 = 0;
    self->field_0x3cc = 0;
    self->field_0x3d4 = 0;
    Vector up;
    up.x = 0.0f;
    up.y = 0.0f;
    up.z = 0.0f;
    self->field_0x330 = up;
    self->triangleCounts = new Array<int>();
    self->field_0x478 = 0;
    self->field_0x400 = 0;
    self->field_0x468 = up;
    self->field_0x3f0 = up;
    self->shaders = new Array<ShaderBaseStruct*>();
    self->field_0x380 = 0;
    self->field_0x40c = 0;
    self->field_0x410 = 0;
    self->field_0x3c4 = 0;
    self->field_0x3c8 = 0;
    up.x = 0.5f;
    up.y = 0.0f;
    up.z = 0.0f;
    *(Vector *)&self->field_0x3cc = up;   // vector spans 0x3cc(x,y)+0x3d4(z)
    self->field_0x358 = 0;
    self->field_0x414 = 0;
    self->field_0x418 = 0;
    self->field_0x41c = 0;
    self->field_0x360 = 0;
    self->field_0x4a8 = 0;
    self->field_0x70 = 0;
    self->field_0x100 = 0;
    self->field_0x3e4 = -1;
    for (int i = 0; i != 0x14; i += 1) {
        self->boundTextures[i] = -1;
    }
    self->frameBufferTextures[0] = -1;
    self->frameBufferTextures[1] = -1;
    self->field_0x4a4 = 0;
    self->field_0xfd = 0x100;
    self->field_0x78 = -1;
    self->field_0x420 = 0;
    self->field_0x424 = 0;
    self->field_0x484 = 0;
    self->field_0x320 = 0;
    self->field_0x328 = 0;
    self->field_0x34 = 0;
    self->field_0x4c0 = 0;
    self->field_0x368 = 0;
    self->field_0x370 = 0;
    self->field_0x4b0 = 0;
    self->field_0x4b8 = 0;
    self->field_0x4e0 = 0;
    self->field_0x4e8 = 0;
    self->field_0x4f0 = 0;
    self->field_0x28 = 0x14;
    self->field_0x20 = 1;
    self->field_0x30 = (char **)new ApplicationManager(self);
    self->field_0x3e8 = 0;
    self->field_0x3ec = 0;
    self->field_0xd0 = -1.0f;
    self->field_0xd4 = -1.0f;
    self->field_0xd8 = -1.0f;
    self->field_0xdc = -1.0f;
    up.x = 1.0f;
    up.y = 0.0f;
    up.z = 0.0f;
    self->field_0x3f0 = up;
    self->initFileInterface();
    return;
}

void Engine::SetTextures(uint32_t first, uint32_t second) {
    char *manager = *this->field_0x30;   // external texture-manager (no named struct)
    uint32_t count = *(uint32_t *)(manager + 0x10);
    if (count == 0 || first > count - 1) {
        return;
    }
    this->SetTextureSlot(first, 0);
    if (second > count - 1) {
        if (this->boundTextures[1] != -1) {
            if (g_Engine_useShaders == 0) {
                glActiveTexture(0x84c1);
                glDisable(0xde1);
                glActiveTexture(0x84c0);
            }
            this->boundTextures[1] = -1;
        }
        return;
    }
    uint32_t texture = **(uint32_t **)(*(char **)(manager + 0x14) + second * 4);
    if (this->boundTextures[1] != texture) {
        glActiveTexture(0x84c1);
        this->GlEnable(0xde1, true);
        unsigned int target = (g_Engine_useShaders != 0 &&
                               (this->field_0x420 & 0x80008) != 0)
                                  ? 0x8513
                                  : 0xde1;
        glBindTexture(target, texture);
        this->boundTextures[1] = texture;
    }
}

typedef void ShaderCtor(void *);

uint32_t Engine::ShaderInit() {
    static const uint32_t sizes[] = {
        0x8c, 0x2c, 0x94, 0x2c, 0x30, 0x74, 0x5c, 0x68, 0x44, 0x50,
        0x64, 0x58, 0x58, 0x30, 0x60, 0x84, 0x5c, 0x5c, 0x5c, 0x84,
        0x5c, 0xa0, 0x64, 0x34, 0x70, 0x98, 0x98, 0x5c, 0x5c, 0x60,
        0x58, 0x34, 0xa8, 0x60, 0x40, 0x6c, 0x64, 0x4c, 0x98,
    };
    ShaderCtor *ctors[] = {ShaderCtor_0, ShaderCtor_1, ShaderCtor_2, ShaderCtor_3, ShaderCtor_4};
    for (uint32_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i += 1) {
        void *shader = operator new(sizes[i]);
        ctors[i % 5](shader);
        this->ShaderRegister((ShaderBaseStruct *)shader);
    }
    glGetError();
    return 1;
}

uint64_t Engine::SetEyePosition(uint32_t x, uint32_t y, uint32_t z) {
    uint32_t buf[3];
    buf[0] = x;
    buf[1] = y;
    buf[2] = z;
    this->eyePosition = *(const Vector *)buf;
    return (uint64_t)buf[0] | ((uint64_t)buf[1] << 32);
}

void Engine::SetModelMatrix(const uint32_t *matrix) {
    Engine *self = this;
    if (g_Engine_useShaders != 0) {
        self->field_0x204 = matrix[0];
        self->field_0x208 = matrix[4];
        self->field_0x20c = matrix[8];
        self->field_0x210 = matrix[1];
        self->field_0x214 = matrix[5];
        self->field_0x218 = matrix[9];
        self->field_0x21c = matrix[2];
        self->field_0x220 = matrix[6];
        self->field_0x224 = matrix[10];
        uint32_t gl[16] = {
            matrix[0], matrix[4], matrix[8], 0,
            matrix[1], matrix[5], matrix[9], 0,
            matrix[2], matrix[6], matrix[10], 0,
            matrix[3], matrix[7], matrix[11], 0x3f800000,
        };
        __aeabi_memcpy(self->modelMatrixGL, gl, 0x40);
        Vector tmp;
        if (self->lightDirty[0] == 0.0f) {
            tmp = AbyssEngine::AEMath::MatrixInverseRotateVector(
                *(const Matrix *)matrix, self->field_0x468);
            tmp = AbyssEngine::AEMath::VectorNormalize(tmp);
            self->field_0x330 = tmp;
        } else {
            self->field_0x330 = self->field_0x468;
        }
        if (self->field_0x32c > 1) {
            if (self->lightDirty[1] == 0.0f) {
                tmp = AbyssEngine::AEMath::MatrixInverseRotateVector(
                    *(const Matrix *)matrix, self->field_0x474);
                tmp = AbyssEngine::AEMath::VectorNormalize(tmp);
                self->field_0x33c = tmp;
            } else {
                self->field_0x33c = self->field_0x474;
            }
        }
        self->ShaderUpdate();
        tmp = AbyssEngine::AEMath::MatrixInverseTransformVector(
            *(const Matrix *)matrix, self->eyePosition);
        *(Vector *)&self->field_0x34c = tmp;
        self->field_0x34c /= *(float *)(matrix + 12);
        self->field_0x350 /= *(float *)(matrix + 13);
        self->field_0x354 /= *(float *)(matrix + 14);
    }
    return;
}

void Engine::LightSetLight(unsigned int light) {
    uint32_t values[4] = {0, 0, 0, 0};
    unsigned int index = light - 0x4000;
    if (index < 8) {
        int count = light - 0x3fff;
        int current = this->field_0x32c;
        if (current > count) {
            count = current;
        }
        this->field_0x32c = count;

        const Vector &dir = (&this->field_0x468)[index];
        values[0] = *(const uint32_t *)&dir.x;
        values[1] = *(const uint32_t *)&dir.y;
        values[2] = *(const uint32_t *)&dir.z;
        values[3] = *(const uint32_t *)&this->lightDirty[index];
        if (g_Engine_useShaders == 0) {
            glLightfv(light, 0x1203, values);
        }
    }
    return;
}

void Engine::SetTexturesExt(uint32_t first, uint32_t second, uint32_t third, ...) {
    char *manager = *this->field_0x30;   // external texture-manager (no named struct)
    if (*(uint32_t *)(manager + 0x10) != 0) {
        uint32_t values[3] = {first, second, third};
        uint32_t slot = 0;
        uint32_t *p = values;
        while (*p != 0xffffffff) {
            this->SetTextureSlot(*p, slot);
            slot += 1;
            p += 1;
        }
        for (uint32_t i = slot; i < 0x14; i += 1) {
            this->boundTextures[i] = -1;
        }
        glActiveTexture(0x84c0);
    }
    return;
}

void Engine::SetWorldViewMatrix(const uint32_t *matrix) {
    if (g_Engine_useShaders != 0) {
        uint32_t gl[16] = {
            matrix[0], matrix[4], matrix[8], 0,
            matrix[1], matrix[5], matrix[9], 0,
            matrix[2], matrix[6], matrix[10], 0,
            matrix[3], matrix[7], matrix[11], 0x3f800000,
        };
        __aeabi_memcpy(this->worldViewMatrixGL, gl, 0x40);
        esMatrixMultiply(this->worldViewProjMatrix, gl, this->projMatrix);
    } else {
        MatrixGetGL(*(const Matrix *)matrix, this->uvMatrixGL);
        return glLoadMatrixf(this->uvMatrixGL);
    }
    return;
}

void Engine::ResetLightParam() {
    this->field_0x488 = 0x3f800000;
    this->field_0x32c = 1;
    this->field_0x298 = 0.8f;
    this->field_0x29c = 0.8f;
    this->field_0x2a0 = 0.8f;
    this->field_0x2a4 = 0x3f800000;
    this->field_0x2a8 = 0.2f;
    this->field_0x2ac = 0.2f;
    this->field_0x2b0 = 0.2f;
    this->field_0x2b4 = 0x3f800000;
    this->field_0x2b8 = 0;
    this->field_0x2c0 = 0x3f800000ULL;
    this->field_0x2c8 = 0;
    this->lightAmbient.r = 0.0f;
    this->lightAmbient.g = 0.0f;
    this->lightAmbient.b = 0.0f;
    this->lightAmbient.a = 1.0f;

    Vector up;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    this->field_0x468 = up;
    this->field_0x474 = up;
    this->lightDirty[0] = 0.0f;
    this->lightDirty[1] = 0.0f;

    if (g_Engine_useShaders == 0) {
        glLightfv(0x4000, 0x1200, &this->lightAmbient.r);
        glLightfv(0x4000, 0x1201, &this->lightDiffuse.r);
        glLightfv(0x4000, 0x1202, &this->lightSpecular.r);
        glMaterialfv(0x408, 0x1200, &this->field_0x2a8);
        glMaterialfv(0x408, 0x1201, &this->field_0x298);
        glMaterialfv(0x408, 0x1202, &this->field_0x2b8);
        glMaterialf(0x408, 0x1601, this->field_0x2c8);
    }
    return;
}

void Engine::LightSetLightColorSpecular(float red, float green, float blue, unsigned int light) {
    unsigned int index = light - 0x4000;
    if (index > 7) {
        return;
    }
    int count = light - 0x3fff;
    int current = this->field_0x32c;
    if (current > count) {
        count = current;
    }
    this->field_0x32c = count;

    LightColor &src = (&this->lightSpecular)[index];
    src.r = red;
    src.g = green;
    src.b = blue;
    src.a = 1.0f;
    if (g_Engine_useShaders == 0) {
        return glLightfv(light, 0x1202, &src.r);
    }
    Vector &dst = (&this->lightSpecularShaded)[index];
    dst.x = this->field_0x2b8 * red;
    dst.y = src.g * this->field_0x2bc;
    dst.z = src.b * this->field_0x2c0;
    return ShaderUpdateMaterialColor();
}

void Engine::GlEnable(unsigned int cap, bool enable) {
    if (g_Engine_useShaders == 0) {
        unsigned int glCap = cap == 0x1000000 ? 0xbc0 : cap;
        if (enable) {
            return glEnable(glCap);
        }
        return glDisable(glCap);
    }

    uint32_t bit = 0;
    if (cap == 0xde1) {
        bit = 1;
    } else if (cap == 0x1000000) {
        bit = 2;
    } else if (cap >= 0x1100000 && cap <= 0x1100024) {
        static const uint32_t bits[] = {
            4, 8, 0x10, 0x20, 0x80, 0x40, 0x100, 0x200, 0x400, 0x1000,
            0, 0, 0, 0, 0, 0, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000,
            0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0, 0, 0, 0, 0,
            0, 0x800000, 0x1000000, 0x2000000, 0x4000000, 0x8000000,
        };
        bit = bits[cap - 0x1100000];
    }
    if (bit == 0) {
        return;
    }
    uint32_t flags = this->field_0x420;
    flags = enable ? (flags | bit) : (flags & ~bit);
    this->field_0x420 = flags;
}

// Engine::LightEnable(bool) toggles fixed-function lighting on the engine. In the shipped
// binary this is a single long-branch veneer (B.W) into the GL backend whose target lies
// outside the analyzed range, so the body cannot be statically recovered; forward to the
// engine's own symbol to preserve the exact runtime behaviour.
extern "C" void _ZN11AbyssEngine6Engine11LightEnableEb(Engine *self, bool enabled);
void Engine::LightEnable(bool enabled) {
    _ZN11AbyssEngine6Engine11LightEnableEb(this, enabled);
}

void Engine::LightSetMaterialColorAmbient(float red, float green, float blue) {
    this->field_0x2a8 = red;
    this->field_0x2ac = green;
    this->field_0x2b0 = blue;
    this->field_0x2b4 = this->field_0x488;

    if (g_Engine_useShaders == 0) {
        return glMaterialfv(0x408, 0x1200, &this->field_0x2a8);
    }

    int count = this->field_0x32c;
    for (int index = 0; index < count; index += 1) {
        const LightColor &src = (&this->lightAmbient)[index];
        Vector &dst = (&this->lightAmbientShaded)[index];
        dst.x = (this->field_0x288 + src.r) * red;
        dst.y = (this->field_0x28c + src.g) * green;
        dst.z = (this->field_0x290 + src.b) * blue;
    }
    return ShaderUpdateMaterialColor();
}
