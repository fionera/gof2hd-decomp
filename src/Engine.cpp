#include "gof2/Engine.h"
#include "gof2/ApplicationManager.h"
#include "gof2/String.h"
#include "gof2/Mesh.h"
#include "gof2/ShaderBaseStruct.h"
#include <arm_neon.h>

// ShaderBaseStruct lives in AbyssEngine (complete type with field_0x4).
// The bare ::ShaderBaseStruct from fwd.h stays incomplete and is only used by
// pointer; member access casts to the complete type via this alias.
typedef AbyssEngine::ShaderBaseStruct ShaderBaseStructFull;
// Mesh is defined in the AbyssEngine namespace (Mesh.h); the complete type is
// needed for field access. ::Mesh (fwd.h) is only an incomplete forward decl.
typedef AbyssEngine::Mesh MeshFull;

extern "C" void FBOContainer_ActivateRender2Texture(FBOContainer *self);
extern "C" void PaintCanvas_Resume(void *canvas);
extern "C" void PaintCanvas_Suspend(void *canvas);
extern "C" uint8_t g_Engine_useShaders;
extern "C" void ShaderUpdateRimColor();
extern "C" void glMatrixMode(unsigned int mode);
extern "C" void MatrixGetGL(const Matrix *matrix, float *out);
extern "C" void glLoadMatrixf(const float *matrix);
extern "C" void glLoadIdentity();
extern "C" void glScalef(float x, float y, float z);
extern "C" void FBOContainer_ActivateTexture(FBOContainer *self);
extern "C" void glColorMask(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);
extern "C" void glDepthFunc(unsigned int func);
extern "C" void glBindFramebuffer(unsigned int target, unsigned int framebuffer);
extern "C" void glViewport(int x, int y, int width, int height);
extern "C" void glClearColor(float red, float green, float blue, float alpha);
extern "C" void glClear(unsigned int mask);
extern "C" void FBOContainer_DeactivateRender2Texture(FBOContainer *self);
extern "C" uint32_t NFC_isPad();
extern "C" uint32_t NFC_getWidth();
extern "C" uint32_t NFC_getHeight();
extern "C" void glEnable(unsigned int cap);
extern "C" void glDepthMask(unsigned int flag);
extern "C" void glDisable(unsigned int cap);
extern "C" void ShaderBaseStruct_Update(ShaderBaseStruct *self);
extern "C" const char *glGetString(unsigned int name);
extern "C" void glMaterialf(unsigned int face, unsigned int pname, float value);
extern "C" void glActiveTexture(unsigned int texture);
extern "C" void glBindTexture(unsigned int target, unsigned int texture);
extern "C" void AEMath_VectorNormalize(Vector *result, const Vector *value);
extern "C" uint8_t g_Engine_shaderDrew;
extern "C" int g_Engine_defaultShader;
extern "C" void glVertexPointer(int size, unsigned int type, int stride, const void *ptr);
extern "C" void glTexCoordPointer(int size, unsigned int type, int stride, const void *ptr);
extern "C" void glNormalPointer(unsigned int type, int stride, const void *ptr);
extern "C" void glColorPointer(int size, unsigned int type, int stride, const void *ptr);
extern "C" void glDrawElements(unsigned int mode, int count, unsigned int type, const void *indices);
extern "C" void glDrawArrays(unsigned int mode, int first, int count);
extern "C" void glGetIntegerv(unsigned int name, void *out);
extern "C" void glBindBuffer(unsigned int target, unsigned int buffer);
extern "C" void ShaderUpdateMaterialColor();
extern "C" void glColor4f(float red, float green, float blue, float alpha);
extern "C" void ApplicationManager_dtor(ApplicationManager *self);
extern "C" void operator_delete(void *ptr);
extern "C" void AEFile_Release();
extern "C" void ArrayReleaseClasses_ShaderBaseStruct_ptr(void *array);
extern "C" void FBOContainer_dtor(FBOContainer *self);
extern "C" void MeshRelease(Engine *self, void *meshSlot);
extern "C" void Engine_ReleaseGL(Engine *self);
extern "C" void Array_ShaderBaseStruct_ptr_dtor(void *array);
extern "C" void Array_int_dtor(void *array);
extern "C" void MeshCreate(Engine *self, int vertices, int faces, int flags, void *outMesh);
extern "C" String *g_Engine_vendorString;
extern "C" String *g_Engine_rendererString;
extern "C" int g_Engine_cloakShader;
extern "C" void ShaderBaseStruct_GetShaderName(String *result, ShaderBaseStruct *self);
// String_GetAEChar declared in ShaderBaseStruct.h (returns void*)
extern "C" void ArrayAdd_ShaderBaseStruct_ptr(ShaderBaseStruct *item, void *array);
extern "C" void ArrayAdd_int(int item, void *array);
extern "C" float g_Engine_texEnv;
extern "C" uint8_t g_Engine_texEnvDirty;
extern "C" void glTexEnvf(unsigned int target, unsigned int pname, float value);
extern "C" void glEnableClientState(unsigned int array);
extern "C" void glDisableClientState(unsigned int array);
extern "C" int g_Engine_lineShader;
extern "C" int g_Engine_altShader;
extern "C" uint8_t g_Engine_shaderDirty;
extern "C" int g_Engine_currentShader;
extern "C" uint8_t g_Engine_postEffectFlag;
extern "C" int g_Engine_postEffectBW;
extern "C" int g_Engine_shaderPostA;
extern "C" int g_Engine_shaderPostB;
extern "C" int g_Engine_shaderPostC;
extern "C" void glMaterialfv(unsigned int face, unsigned int pname, const void *params);
extern "C" void glLightModelfv(unsigned int pname, const void *params);
extern "C" void FBOContainer_ctor(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_Create(FBOContainer *self, int width, int height, bool depth, bool color);
extern "C" int g_Engine_postEffectBlur;
extern "C" int g_Engine_postEffectCounter;
extern "C" int g_Engine_postEffectPending;
extern "C" void FileInterfaceAndroid_ctor(void *self);
extern "C" void AEFile_SetInterface(void *fileInterface);
extern "C" void esMatrixMultiply(void *out, const void *lhs, const void *rhs);
extern "C" void PaintCanvas_Initialize(void *canvas, bool value);
extern "C" void glLineWidth(float width);
extern "C" void glCullFace(unsigned int mode);
extern "C" uint8_t g_Engine_supportsFBO;
extern "C" void glLightfv(unsigned int light, unsigned int pname, const void *params);
extern "C" int g_Engine_activeShader;
extern "C" void Array_int_ctor(void *array);
extern "C" void Array_ShaderBaseStruct_ptr_ctor(void *array);
extern "C" void glGetError();
extern "C" void ShaderCtor_0(void *);
extern "C" void ShaderCtor_1(void *);
extern "C" void ShaderCtor_2(void *);
extern "C" void ShaderCtor_3(void *);
extern "C" void ShaderCtor_4(void *);
extern "C" void AEMath_MatrixInverseRotateVector(Vector *out, const Matrix *matrix, const Vector *value);
extern "C" void AEMath_MatrixInverseTransformVector(Vector *out, const Matrix *matrix, const Vector *value);

// ---- GetAccelValue_86510.cpp ----
double * Engine::GetAccelValue() {
    Engine *self = this;
    double x = self->field_0x4b0;
    double y;
    if (*(int *)(*self->field_0x30 + 0x30) == 1) {
        x = -x;
        y = -self->field_0x4b8;
    } else {
        y = self->field_0x4b8;
    }
    self->field_0x4c8 = x;
    self->field_0x4d0 = y;
    self->field_0x4d8 = self->field_0x4c0;
    return (double *)((char *)self + 0x4c8);
}

// ---- ActivateRender2FracFBO_6dcf6.cpp ----
void Engine::ActivateRender2FracFBO() {
    Engine *self = this;
    FBOContainer *fbo = self->field_0x418;
    if (fbo != 0) {
        return FBOContainer_ActivateRender2Texture(fbo);
    }
}

// ---- Resume_845ee.cpp ----
uint32_t Engine::Resume() {
    Engine *self = this;
    PaintCanvas_Resume(*self->field_0x30);
    for (int index = 0; index != 0x14; index += 1) {
        *(int *)((char *)self + 0x7c + index * 4) = -1;
    }
    return 1;
}

// ---- Suspend_845de.cpp ----
uint32_t Engine::Suspend() {
    Engine *self = this;
    PaintCanvas_Suspend(*self->field_0x30);
    return 1;
}

// ---- GetDisplayWidth_84530.cpp ----
uint32_t Engine::GetDisplayWidth() {
    Engine *self = this;
    return self->field_0x368;
}

// ---- GetGravValue_864c0.cpp ----
double * Engine::GetGravValue() {
    Engine *self = this;
    double x = self->field_0x4e0;
    double y;
    if (*(int *)(*self->field_0x30 + 0x30) == 1) {
        x = -x;
        y = -self->field_0x4e8;
    } else {
        y = self->field_0x4e8;
    }
    self->field_0x4f8 = x;
    self->field_0x500 = y;
    self->field_0x508 = self->field_0x4f0;
    return (double *)((char *)self + 0x4f8);
}

// ---- GetDisplayHeight_84536.cpp ----
uint32_t Engine::GetDisplayHeight() {
    Engine *self = this;
    return self->field_0x36c;
}

// ---- LightSetRimColor_85dd0.cpp ----
void Engine::LightSetRimColor(float red, float green, float blue) {
    Engine *self = this;
    if (g_Engine_useShaders == 0) {
        return;
    }
    self->field_0x320 = red;
    self->field_0x324 = green;
    self->field_0x328 = blue;
    return ShaderUpdateRimColor();
}

// ---- IsPostEffectActivated_868dc.cpp ----
bool Engine::IsPostEffectActivated() {
    Engine *self = this;
    return self->field_0x410 != 0;
}

// ---- SetUVMatrix_850f0.cpp ----
void Engine::SetUVMatrix(const uint32_t *matrix) {
    Engine *self = this;
    if (g_Engine_useShaders == 0) {
        glMatrixMode(0x1702);
        MatrixGetGL((const Matrix *)matrix, (float *)((char *)self + 0x428));
        glLoadMatrixf((float *)((char *)self + 0x428));
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

    self->field_0x1c4 = m0;
    self->field_0x1c8 = m4;
    self->field_0x1cc = m8;
    self->field_0x1d0 = 0;
    self->field_0x1d4 = m1;
    self->field_0x1d8 = m5;
    self->field_0x1dc = m9;
    self->field_0x1e0 = 0;
    self->field_0x1e4 = m2;
    self->field_0x1e8 = m6;
    self->field_0x1ec = m10;
    self->field_0x1f0 = 0;
    self->field_0x1f4 = m3;
    self->field_0x1f8 = m7;
    self->field_0x1fc = m11;
    self->field_0x200 = 0x3f800000;
}

// ---- ActivateRender2TextureFBO_6dccc.cpp ----
void Engine::ActivateRender2TextureFBO() {
    Engine *self = this;
    FBOContainer *fbo = self->field_0x414;
    if (fbo != 0) {
        return FBOContainer_ActivateRender2Texture(fbo);
    }
}

// ---- LightSetMaterialColorAlpha_856b4.cpp ----
typedef void Materialfv(unsigned int face, unsigned int pname, const void *params);
static Materialfv * volatile g_Engine_glMaterialfv;

void Engine::LightSetMaterialColorAlpha(float alpha) {
    Engine *self = this;
    if (self->field_0x424 == 0) {
        return;
    }

    self->field_0x2b4 = alpha;
    self->field_0x488 = alpha;
    Materialfv *materialfv = g_Engine_glMaterialfv;
    materialfv(0x408, 0x1200, (char *)self + 0x2a8);
    self->field_0x2c4 = self->field_0x488;
    materialfv(0x408, 0x1202, (char *)self + 0x2b8);
    self->field_0x2a4 = self->field_0x488;
    return materialfv(0x408, 0x1201, (char *)self + 0x298);
}

// ---- SetAccelValue_86582.cpp ----
void Engine::SetAccelValue(double x, double y, double z) {
    Engine *self = this;
    self->field_0x4b0 = x;
    self->field_0x4b8 = y;
    self->field_0x4c0 = z;
}

// ---- ResetUVMatrix_85184.cpp ----
void Engine::ResetUVMatrix() {
    Engine *self = this;
    if (g_Engine_useShaders != 0) {
        uint32_t one = 0x3f800000;
        uint32x4_t zero = vdupq_n_u32(0);
        self->field_0x1c4 = one;
        self->field_0x1d8 = one;
        self->field_0x1ec = one;
        self->field_0x200 = one;
        vst1q_u32((uint32_t *)((char *)self + 0x1c8), zero);
        vst1q_u32((uint32_t *)((char *)self + 0x1dc), zero);
        vst1q_u32((uint32_t *)((char *)self + 0x1f0), zero);
        return;
    }

    glMatrixMode(0x1702);
    glLoadIdentity();
    glScalef(1.0f, -1.0f, 1.0f);
    return glMatrixMode(0x1700);
}

// ---- ActivateTextureFBO_6dcbe.cpp ----
void Engine::ActivateTextureFBO() {
    Engine *self = this;
    FBOContainer *fbo = self->field_0x414;
    if (fbo != 0) {
        return FBOContainer_ActivateTexture(fbo);
    }
}

// ---- GlowEndGlow_869f4.cpp ----
void Engine::GlowEndGlow() {
    Engine *self = this;
    if (g_Engine_useShaders == 0) {
        return;
    }
    self->field_0x41c = 0;
    glColorMask(1, 1, 1, 1);
    return glDepthFunc(0x201);
}

// ---- ActivateViewBuffer_6dd14.cpp ----
void Engine::ActivateViewBuffer() {
    Engine *self = this;
    glBindFramebuffer(0x8d40, self->field_0x40c);
    return glViewport(0, 0, self->field_0x370, self->field_0x374);
}

// ---- GlowEnableGlow_869c8.cpp ----
void Engine::GlowEnableGlow() {
    Engine *self = this;
    if (self->field_0x41c != 0) {
        return;
    }
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(0x4000);
    self->field_0x41c = 1;
}

// ---- SetOnDestroyApp_845d8.cpp ----
typedef void DestroyCallback(Engine *);

void Engine::SetOnDestroyApp(DestroyCallback *callback) {
    Engine *self = this;
    self->field_0x484 = callback;
}

// ---- SetGravValue_86560.cpp ----
void Engine::SetGravValue(double x, double y, double z) {
    Engine *self = this;
    self->field_0x4e0 = x;
    self->field_0x4e8 = y;
    self->field_0x4f0 = z;
}

// ---- SwapBuffer_6ddb4.cpp ----
void Engine::SwapBuffer() {
    Engine *self = this;
    uint32_t index = 0;
    uint32_t zero = 0;
    while (index < self->field_0x3d8) {
        *(uint32_t *)(self->field_0x3dc + index * 4) = zero;
        index += 1;
    }
}

// ---- ReloadShaders_865c8.cpp ----
typedef void ShaderUnload(ShaderBaseStruct *);
typedef void ShaderInit(ShaderBaseStruct *, Engine *);

void Engine::ReloadShaders() {
    Engine *self = this;
    uint32_t index = 0;
    while (index < self->field_0x510) {
        ShaderBaseStruct *shader =
            *(ShaderBaseStruct **)(self->field_0x514 + index * 4);
        void **vtable = *(void ***)shader;
        ((ShaderUnload *)vtable[0x24 / 4])(shader);

        shader = *(ShaderBaseStruct **)(self->field_0x514 + index * 4);
        vtable = *(void ***)shader;
        ((ShaderInit *)vtable[0x08 / 4])(shader, self);
        index += 1;
    }
}

// ---- DeactivateRender2TextureFBO_6dcda.cpp ----
void Engine::DeactivateRender2TextureFBO() {
    Engine *self = this;
    FBOContainer *fbo = self->field_0x414;
    if (fbo != 0) {
        return FBOContainer_DeactivateRender2Texture(fbo);
    }
}

// ---- GetDeviceInfo_6ddd6.cpp ----
void Engine::GetDeviceInfo() {
    Engine *self = this;
    self->field_0x8 = NFC_isPad();
    self->field_0x0 = NFC_getWidth();
    self->field_0x4 = NFC_getHeight();
}

// ---- CopyFBO_6dd38.cpp ----
void Engine::CopyFBO() {
    Engine *self = this;
    if (g_Engine_useShaders == 0) {
        return;
    }

    if (((Engine *)(self))->IsPostEffectActivated()) {
        ((Engine *)(self))->DeactivateRender2TextureFBO();
        ((Engine *)(self))->DrawCloakFBO(self->field_0x414);
        ((Engine *)(self))->ActivateRender2TextureFBO();
    } else {
        ((Engine *)(self))->DeactivateRender2FracFBO();
        ((Engine *)(self))->DrawCloakFBO(self->field_0x418);
        ((Engine *)(self))->ActivateViewBuffer();
    }

    glEnable(0xb71);
    glDepthMask(1);
    glDisable(0xbe2);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    return glClear(0x100);
}

// ---- HasVibration_8453c.cpp ----
bool Engine::HasVibration() {
    Engine *self = this;
    if (self->field_0x480) {
        return self->field_0x2c != 0;
    }
    return false;
}

// ---- LightSetLightCount_8571c.cpp ----
void Engine::LightSetLightCount(int count) {
    Engine *self = this;
    if (count >= 8) {
        count = 8;
    }
    count &= ~(count >> 31);
    self->field_0x32c = count;
}

// ---- SetAddData_84b04.cpp ----
void Engine::SetAddData(void *data, int size) {
    Engine *self = this;
    self->field_0x358 = data;
    self->field_0x35c = size;
}

// ---- ShaderUpdate_8538c.cpp ----
void Engine::ShaderUpdate() {
    Engine *self = this;
    uint32_t index = 0;
    while (index < self->field_0x510) {
        ShaderBaseStruct_Update(
            *(ShaderBaseStruct **)(self->field_0x514 + index * 4));
        index += 1;
    }
}

// ---- IsExtensionSupported_86452.cpp ----
extern "C" bool Engine_IsExtensionSupported(Engine *, const char *extension)
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

// ---- LightSetMaterialColorShininess_85e04.cpp ----
void Engine::LightSetMaterialColorShininess(float shininess) {
    Engine *self = this;
    self->field_0x2c8 = shininess;
    if (g_Engine_useShaders == 0) {
        return glMaterialf(0x408, 0x1601, shininess);
    }
}

// ---- Initialize_84528.cpp ----
typedef void InitializeCallback(Engine *);

void Engine::Initialize(InitializeCallback *callback) {
    Engine *self = this;
    if (callback != 0) {
        callback(self);
    }
}

// ---- ActivateRefractFBO_6dce8.cpp ----
void Engine::ActivateRefractFBO() {
    Engine *self = this;
    FBOContainer *fbo = self->field_0x418;
    if (fbo != 0) {
        return FBOContainer_ActivateTexture(fbo);
    }
}

// ---- LightSetParticleAmbient_85828.cpp ----
void Engine::LightSetParticleAmbient(float red, float green, float blue) {
    Engine *self = this;
    float *ambient = (float *)((char *)self + 0x314);
    *ambient++ = red;
    *ambient++ = green;
    *ambient++ = blue;
}

// ---- DeactivateRender2FracFBO_6dd04.cpp ----
void Engine::DeactivateRender2FracFBO() {
    Engine *self = this;
    FBOContainer *fbo = self->field_0x418;
    if (fbo != 0) {
        return FBOContainer_DeactivateRender2Texture(fbo);
    }
}

// ---- SetPerspMatrix_85478.cpp ----
void Engine::SetPerspMatrix(const uint32_t *matrix) {
    Engine *self = this;
    if (g_Engine_useShaders == 0) {
        return;
    }
    self->field_0x384 = matrix[0];
    self->field_0x388 = matrix[1];
    self->field_0x38c = matrix[2];
    self->field_0x390 = matrix[3];
    self->field_0x394 = matrix[4];
    self->field_0x398 = matrix[5];
    self->field_0x39c = matrix[6];
    self->field_0x3a0 = matrix[7];
    self->field_0x3a4 = matrix[8];
    self->field_0x3a8 = matrix[9];
    self->field_0x3ac = matrix[10];
    self->field_0x3b0 = matrix[11];
    self->field_0x3b4 = matrix[12];
    self->field_0x3b8 = matrix[13];
    self->field_0x3bc = matrix[14];
    self->field_0x3c0 = matrix[15];
}

// ---- SetFrameBufferTexture_84aa0.cpp ----
void Engine::SetFrameBufferTexture(int slot0, int slot1) {
    Engine *self = this;
    char *firstBase = (char *)self + slot0 * 4;
    int firstValue = *(int *)(firstBase + 0x48c);
    if (firstValue != -1) {
        int *first = (int *)(firstBase + 0x48c);
        glActiveTexture(0x84c0);
        glBindTexture(0xde1, *first);
    }

    if (slot1 == -1) {
        return;
    }
    char *secondBase = (char *)self + slot1 * 4;
    int secondValue = *(int *)(secondBase + 0x48c);
    if (secondValue == -1) {
        return;
    }
    int *second = (int *)(secondBase + 0x48c);
    glActiveTexture(0x84c1);
    return glBindTexture(0xde1, *second);
}

// ---- LightSetLightDirection_858d0.cpp ----
void Engine::LightSetLightDirection(float x, float y, float z, unsigned int light) {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    unsigned int index = light - 0x4000;
    if (index < 8) {
        int count = light - 0x3fff;
        int current = self->field_0x32c;
        if (current > count) {
            count = current;
        }
        self->field_0x32c = count;

        Vector input;
        input.x = x;
        input.y = y;
        input.z = z;
        Vector normalized;
        AEMath_VectorNormalize(&normalized, &input);
        *(Vector *)((char *)self + 0x468 + index * 0x0c) = normalized;
        *(uint32_t *)((char *)self + 0x378 + index * 4) = 0;
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- RenderMesh_85f24.cpp ----
void Engine::RenderMesh(MeshFull *mesh) {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    if (mesh == 0 || mesh->field_0x28 == 0) {
        goto done;
    }

    if (g_Engine_useShaders == 0) {
        glVertexPointer(3, 0x1406, 0, mesh->field_0x4);
        ((Engine *)(self))->AEClientState(0x8074, true);
        bool tex = ((uint32_t)*(uint8_t *)mesh << 30) < 0;
        if (tex && (mesh->field_0x30 == 0 ||
                    *(int *)((char *)mesh->field_0x30 + 4) == -1)) {
            glTexCoordPointer(2, 0x1406, 0, mesh->field_0x8);
        }
        ((Engine *)(self))->AEClientState(0x8078, tex);
        bool normals = ((uint32_t)*(uint8_t *)mesh << 29) < 0;
        if (normals) {
            glNormalPointer(0x1406, 0, mesh->field_0x10);
        }
        ((Engine *)(self))->AEClientState(0x8075, normals);
        bool colors = ((uint32_t)*(uint8_t *)mesh << 28) < 0;
        if (colors) {
            glColorPointer(4, 0x1406, 0, (void *)(uintptr_t)mesh->field_0xc);
        }
        ((Engine *)(self))->AEClientState(0x8076, colors);
        if (((uint32_t)*(uint8_t *)mesh << 27) < 0) {
            glDrawElements(4, mesh->field_0x28, 0x1403,
                           mesh->field_0x2c);
        } else {
            glDrawArrays(4, 0, mesh->field_0x2);
        }
        if (tex && mesh->field_0x30 != 0 &&
            *(int *)((char *)mesh->field_0x30 + 4) != -1) {
            ((Engine *)(self))->AEClientState(0x8078, false);
        }
    } else {
        g_Engine_shaderDrew = 0;
        ((Engine *)(self))->ShaderSetActive(g_Engine_defaultShader, mesh);
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
            ((Engine *)(self))->ShaderSetInActive();
        }
    }

done:
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- DrawQuad_868f0.cpp ----
void Engine::DrawQuad(int x, int y, int width, int height) {
    Engine *self = this;
    float fx = (float)x;
    float fy = (float)y;
    float right = (float)(x + width);
    float bottom = (float)(height + y);

    MeshFull *mesh = (MeshFull *)self->field_0x380;
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

// ---- SetColor_855c8.cpp ----
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
    ((Engine *)(self))->LightSetMaterialColorAlpha(alpha);
    return glColor4f(red, green, blue, alpha);
}

// ---- _Engine_8415c.cpp ----
typedef void DestroyCallback(Engine *);
typedef void FileInterfaceRelease(void *);


Engine::~Engine()
{
    DestroyCallback *destroy = this->field_0x484;
    if (destroy != 0) {
        destroy(this);
    }

    ApplicationManager *manager = (ApplicationManager *)this->field_0x30;
    if (manager != 0) {
        ApplicationManager_dtor(manager);
        operator_delete(manager);
    }
    this->field_0x30 = 0;

    void *fileInterface = this->field_0x24;
    if (fileInterface != 0) {
        void **vtable = *(void ***)fileInterface;
        ((FileInterfaceRelease *)vtable[1])(fileInterface);
    }
    this->field_0x24 = 0;

    AEFile_Release();
    void *shaders = (char *)this + 0x510;
    ArrayReleaseClasses_ShaderBaseStruct_ptr(shaders);

    FBOContainer *fbo = this->field_0x414;
    if (fbo != 0) {
        FBOContainer_dtor(fbo);
        operator_delete(fbo);
    }
    this->field_0x414 = 0;

    fbo = this->field_0x418;
    if (fbo != 0) {
        FBOContainer_dtor(fbo);
        operator_delete(fbo);
    }
    this->field_0x418 = 0;

    MeshRelease(this, (char *)this + 0x380);
    Engine_ReleaseGL(this);
    Array_ShaderBaseStruct_ptr_dtor(shaders);
    Array_int_dtor((char *)this + 0x3d8);
    ((String *)((String *)((char *)this + 0x4c)))->dtor();
    ((String *)((String *)((char *)this + 0x3c)))->dtor();
    ((String *)((String *)((char *)this + 0x14)))->dtor();
    ((String *)((String *)this))->dtor();
}

// ---- AfterGLInit_8428c.cpp ----
void Engine::AfterGLInit() {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    ((Engine *)(self))->ResetLightParam();
    MeshCreate(self, 4, 2, 0x13, (char *)self + 0x380);

    uint32_t *indices = *(uint32_t **)(self->field_0x380 + 0x2c);
    indices[0] = 0x20000;
    indices[1] = 1;
    indices[2] = 0x30002;

    char storage[sizeof(String)];
    String *text = (String *)storage;
    ((String *)(text))->ctor_char(glGetString(0x1f00), false);
    ((String *)(g_Engine_vendorString))->assign(text);
    ((String *)(text))->dtor();
    ((String *)(text))->ctor_char(glGetString(0x1f01), false);
    ((String *)(g_Engine_rendererString))->assign(text);
    ((String *)(text))->dtor();

    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- DrawCloakFBO_868b0.cpp ----
typedef void ShaderDrawCloak(ShaderBaseStruct *);

void Engine::DrawCloakFBO(FBOContainer *fbo) {
    Engine *self = this;
    (void)fbo;   // unused in the shader path
    if (g_Engine_useShaders != 0) {
        ShaderBaseStruct *shader =
            *(ShaderBaseStruct **)(self->field_0x514 + g_Engine_cloakShader * 4);
        void **vtable = *(void ***)shader;
        return ((ShaderDrawCloak *)vtable[0x14 / 4])(shader);
    }
}

// ---- ShaderRegister_84a20.cpp ----
typedef void ShaderInitFn(ShaderBaseStruct *, Engine *);

void Engine::ShaderRegister(ShaderBaseStruct *shader) {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    if (shader != 0) {
        char nameStorage[sizeof(String)];
        String *name = (String *)nameStorage;
        ShaderBaseStruct_GetShaderName(name, shader);
        char *text = (char *)((String *)(name))->GetAEChar();
        ((String *)(name))->dtor();

        void **vtable = *(void ***)shader;
        ((ShaderInitFn *)vtable[0x08 / 4])(shader, self);
        ArrayAdd_ShaderBaseStruct_ptr(shader, (char *)self + 0x510);
        ArrayAdd_int(0, (char *)self + 0x3d8);
        operator_delete(text);
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- SetTextureSlot_84ffc.cpp ----
void Engine::SetTextureSlot(uint32_t textureIndex, uint32_t slot) {
    Engine *self = this;
    char *manager = *self->field_0x30;   // external texture-manager (no named struct)
    uint32_t count = *(uint32_t *)(manager + 0x10);
    if (count == 0 || slot >= 8 || textureIndex > count - 1) {
        return;
    }
    uint32_t *bound = (uint32_t *)((char *)self + 0x7c + slot * 4);
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

// ---- AEClientState_86350.cpp ----
void Engine::AEClientState(unsigned int state, bool enable) {
    Engine *self = this;
    uint32_t bits = self->field_0x4a4;
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
    self->field_0x4a4 = bits;
}

// ---- GlowBeginGlow_86988.cpp ----
void Engine::GlowBeginGlow(unsigned int depthFunc) {
    Engine *self = this;
    if (self->field_0x41c != 0) {
        return;
    }
    if (g_Engine_useShaders == 0) {
        return;
    }
    glColorMask(0, 0, 0, 1);
    ((Engine *)(self))->GlowEnableGlow();
    if (self->field_0x41c != 0) {
        return glDepthFunc(depthFunc);
    }
}

// ---- DrawLine2D_85e2c.cpp ----
void Engine::DrawLine2D(int vertexCount, int count, bool strip) {
    Engine *self = this;
    self->field_0x348 = vertexCount;
    ((Engine *)(self))->ShaderSetActive(g_Engine_lineShader, 0);
    unsigned int mode = strip != 0 ? 2 : 1;
    return glDrawArrays(mode, 0, count);
}

// ---- ShaderSetActive_85e60.cpp ----
typedef void ShaderEnable(ShaderBaseStruct *, bool);
typedef void ShaderApply(ShaderBaseStruct *, MeshFull *, Engine *);

void Engine::ShaderSetActive(int shaderIndex, MeshFull *mesh) {
    Engine *self = this;
    while (shaderIndex == -1) {
        shaderIndex = g_Engine_defaultShader;
        if ((((uint32_t)*(uint8_t *)mesh) << 30) >= 0) {
            shaderIndex = g_Engine_altShader;
        }
    }

    bool hasExtra = mesh != 0 && mesh->field_0x85 != 0;
    ShaderBaseStruct *shader =
        *(ShaderBaseStruct **)(self->field_0x514 + shaderIndex * 4);
    if (shader == 0) {
        return;
    }
    g_Engine_shaderDirty = 1;

    void **vtable = *(void ***)shader;
    if (((ShaderBaseStructFull *)shader)->field_0x4 != self->field_0x3e4) {
        ((ShaderEnable *)vtable[0x28 / 4])(shader, hasExtra);
        shader = *(ShaderBaseStruct **)(self->field_0x514 + shaderIndex * 4);
        self->field_0x3e4 = ((ShaderBaseStructFull *)shader)->field_0x4;
        g_Engine_currentShader = shaderIndex;
        vtable = *(void ***)shader;
    }
    ((ShaderEnable *)vtable[0x28 / 4])(shader, hasExtra);
    shader = *(ShaderBaseStruct **)(self->field_0x514 + shaderIndex * 4);
    vtable = *(void ***)shader;
    ((ShaderApply *)vtable[0x0c / 4])(shader, mesh, self);
    if (mesh != 0) {
        uint32_t triangles = __aeabi_uidiv(mesh->field_0x28, 3);
        uint32_t *slot = (uint32_t *)(self->field_0x3dc + shaderIndex * 4);
        *slot += triangles;
    }
}

// ---- DoPostEffect_8675c.cpp ----
typedef void ShaderPostDraw(ShaderBaseStruct *, void *);
typedef void ShaderPostDrawSwap(ShaderBaseStruct *, void *, void **, Engine *);

void Engine::DoPostEffect() {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    uint32_t flags = self->field_0x410;
    void *current = self->field_0x414;
    void *other = self->field_0x418;
    if (g_Engine_useShaders != 0) {
        void *slot = other;
        if ((flags & 2) != 0) {
            ShaderBaseStruct *shader =
                *(ShaderBaseStruct **)(self->field_0x514 + g_Engine_shaderPostA * 4);
            void **vtable = *(void ***)shader;
            if ((flags & ~2u) == 0) {
                ((ShaderPostDraw *)vtable[0x14 / 4])(shader, current);
                flags = 0;
            } else {
                ((ShaderPostDrawSwap *)vtable[0x1c / 4])(shader, current, &other, self);
                slot = other;
                other = current;
            }
        }
        if ((self->field_0x410 & 1) != 0) {
            ShaderBaseStruct *shader =
                *(ShaderBaseStruct **)(self->field_0x514 + g_Engine_shaderPostB * 4);
            void **vtable = *(void ***)shader;
            if ((flags & ~1u) == 0) {
                ((ShaderPostDraw *)vtable[0x14 / 4])(shader, slot);
                flags = 0;
            } else {
                ((ShaderPostDrawSwap *)vtable[0x1c / 4])(shader, slot, &other, self);
                slot = other;
            }
        }
        if ((self->field_0x410 & 4) != 0) {
            ShaderBaseStruct *shader =
                *(ShaderBaseStruct **)(self->field_0x514 + g_Engine_shaderPostC * 4);
            void **vtable = *(void ***)shader;
            if ((flags & ~4u) == 0) {
                ((ShaderPostDraw *)vtable[0x18 / 4])(shader, slot);
            } else {
                ((ShaderPostDrawSwap *)vtable[0x20 / 4])(shader, slot, &other, self);
            }
        }
        if (g_Engine_postEffectFlag == 1) {
            ((Engine *)(self))->SetPostEffect(g_Engine_postEffectBW, false);
        }
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- LightSetMaterialColorSpecular_85d44.cpp ----
void Engine::LightSetMaterialColorSpecular(float red, float green, float blue) {
    Engine *self = this;
    self->field_0x2b8 = red;
    self->field_0x2bc = green;
    self->field_0x2c0 = blue;
    self->field_0x2c4 = self->field_0x488;

    if (g_Engine_useShaders == 0) {
        return glMaterialfv(0x408, 0x1202, (char *)self + 0x2b8);
    }

    int count = self->field_0x32c;
    int sourceOffset = 0;
    int destOffset = 0;
    for (int index = 0; index < count; index += 1) {
        char *source = (char *)self + sourceOffset;
        char *dest = (char *)self + destOffset;
        sourceOffset += 0x10;
        destOffset += 0x0c;
        *(float *)(dest + 0x2e4) = *(float *)(source + 0x248) * red;
        *(float *)(dest + 0x2e8) = *(float *)(source + 0x24c) * green;
        *(float *)(dest + 0x2ec) = *(float *)(source + 0x250) * blue;
    }
    return ShaderUpdateMaterialColor();
}

// ---- LightSetGlobalSceneColorAmbient_85830.cpp ----
void Engine::LightSetGlobalSceneColorAmbient(float red, float green, float blue) {
    Engine *self = this;
    self->field_0x288 = red;
    self->field_0x28c = green;
    self->field_0x290 = blue;
    self->field_0x294 = 0x3f800000;

    if (g_Engine_useShaders == 0) {
        return glLightModelfv(0xb53, (char *)self + 0x288);
    }

    int count = self->field_0x32c;
    int sourceOffset = 0;
    int destOffset = 0;
    for (int index = 0; index < count; index += 1) {
        char *source = (char *)self + sourceOffset;
        char *dest = (char *)self + destOffset;
        sourceOffset += 0x10;
        destOffset += 0x0c;
        *(float *)(dest + 0x2cc) =
            (*(float *)(source + 0x268) + red) * self->field_0x2a8;
        *(float *)(dest + 0x2d0) =
            (*(float *)(source + 0x26c) + green) * self->field_0x2ac;
        *(float *)(dest + 0x2d4) =
            (*(float *)(source + 0x270) + blue) * self->field_0x2b0;
    }
    return ShaderUpdateMaterialColor();
}

// ---- SetPostEffect_865fc.cpp ----
void Engine::SetPostEffect(uint32_t effect, bool enable) {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    if (self->field_0x414 == 0 && enable) {
        FBOContainer *fbo = (FBOContainer *)operator new(0x38);
        char nameStorage[sizeof(String)];
        String *name = (String *)nameStorage;
        ((String *)(name))->ctor_char("posteffect", false);
        FBOContainer_ctor(fbo, self, name);
        self->field_0x414 = fbo;
        ((String *)(name))->dtor();
        int width;
        int height;
        if (*(int *)(*self->field_0x30 + 0x30) == 2) {
            width = self->field_0x368;
            height = self->field_0x36c;
        } else {
            width = self->field_0x36c;
            height = self->field_0x368;
        }
        FBOContainer_Create(self->field_0x414, width, height, false, true);
    }

    uint32_t flags = self->field_0x410;
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
    self->field_0x410 = flags;
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- LightSetMaterialColorDiffuse_85c14.cpp ----
void Engine::LightSetMaterialColorDiffuse(float red, float green, float blue) {
    Engine *self = this;
    self->field_0x298 = red;
    self->field_0x29c = green;
    self->field_0x2a0 = blue;
    self->field_0x2a4 = self->field_0x488;

    if (g_Engine_useShaders == 0) {
        return glMaterialfv(0x408, 0x1201, (char *)self + 0x298);
    }

    int lightCount = self->field_0x32c;
    int sourceOffset = 0;
    int destOffset = 0;
    for (int i = 0; i < lightCount; i += 1) {
        char *source = (char *)self + sourceOffset;
        char *dest = (char *)self + destOffset;
        sourceOffset += 0x10;
        destOffset += 0x0c;
        *(float *)(dest + 0x2fc) = *(float *)(source + 0x228) * red;
        *(float *)(dest + 0x300) = *(float *)(source + 0x22c) * green;
        *(float *)(dest + 0x304) = *(float *)(source + 0x230) * blue;
    }
    return ShaderUpdateMaterialColor();
}

// ---- initFileInterface_6daf4.cpp ----
void Engine::initFileInterface() {
    Engine *self = this;
    void *fileInterface = operator new(0x38);
    FileInterfaceAndroid_ctor(fileInterface);
    self->field_0x24 = fileInterface;
    return AEFile_SetInterface(fileInterface);
}

// ---- SetOrthoMatrix_854e8.cpp ----
void Engine::SetOrthoMatrix(const uint32_t *projection, const uint32_t *view, bool multiply) {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    if (g_Engine_useShaders != 0) {
        for (int i = 0; i < 16; i += 1) {
            *(uint32_t *)((char *)self + 0x384 + i * 4) = projection[i];
        }
        if (multiply) {
            uint32_t local[16];
            __aeabi_memcpy(local, view, 0x40);
            esMatrixMultiply((char *)self + 0x384, local, (char *)self + 0x384);
        }
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- InitGL_6db20.cpp ----
int Engine::InitGL(bool shaders, int width, int height) {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    self->field_0x418 = 0;
    self->field_0x368 = width;
    self->field_0x36c = height;
    self->field_0x370 = width;
    self->field_0x374 = height;

    void *fileInterface = operator new(0x38);
    FileInterfaceAndroid_ctor(fileInterface);
    self->field_0x24 = fileInterface;
    AEFile_SetInterface(fileInterface);

    self->field_0x10 = 0;
    self->field_0x2c = 0;
    self->field_0x480 = 0;
    g_Engine_useShaders = shaders;
    self->field_0x40c = 0;

    ((Engine *)(self))->ResetLightParam();
    glViewport(0, 0, self->field_0x374, self->field_0x370);
    if (g_Engine_useShaders != 0) {
        ((Engine *)(self))->ShaderInit();
    } else {
        glEnable(0x803a);
        glDisable(0xb50);
        glLineWidth(1.0f);
    }

    Vector value;
    value.x = 0.0f;
    value.y = 1.0f;
    value.z = 0.0f;
    self->field_0x468 = value;
    self->field_0x378 = 0;
    self->field_0x474 = value;
    self->field_0x37c = 0;

    glEnable(0xb71);
    ((Engine *)(self))->GlEnable(0xde1, true);
    glDisable(0xbe2);
    glCullFace(0x405);
    glEnable(0xb44);
    ((Engine *)(self))->AfterGLInit();
    PaintCanvas_Initialize(*self->field_0x30, false);
    self->field_0xc = 0;
    glGetIntegerv(0xd33, (char *)self + 0x0c);

    if (g_Engine_useShaders != 0 && g_Engine_supportsFBO != 0) {
        FBOContainer *fbo = (FBOContainer *)operator new(0x38);
        char nameStorage[sizeof(String)];
        String *name = (String *)nameStorage;
        ((String *)(name))->ctor_char("refract", false);
        FBOContainer_ctor(fbo, self, name);
        self->field_0x418 = fbo;
        ((String *)(name))->dtor();
        FBOContainer_Create(fbo, self->field_0x368,
                            self->field_0x36c, false, true);
    }

    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return 1;
    }
    __stack_chk_fail();
}

// ---- ClearBuffer_84558.cpp ----
extern "C" void Engine_ClearBuffer(Engine *, uint32_t color)
{
    const double scale = 255.0;
    double red = (double)(color >> 24) / scale;
    double green = (double)((color >> 16) & 0xff) / scale;
    double blue = (double)((color >> 8) & 0xff) / scale;
    double alpha = (double)(color & 0xff) / scale;
    glClearColor((float)red, (float)green, (float)blue, (float)alpha);
    return glClear(0x4100);
}

// ---- LightSetLightPosition_8595c.cpp ----
void Engine::LightSetLightPosition(float x, float y, float z, unsigned int light) {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    unsigned int index = light - 0x4000;
    if (index < 8) {
        int count = light - 0x3fff;
        int current = self->field_0x32c;
        if (current > count) {
            count = current;
        }
        self->field_0x32c = count;

        Vector value;
        value.x = x;
        value.y = y;
        value.z = z;
        *(Vector *)((char *)self + 0x468 + index * 0x0c) = value;
        *(uint32_t *)((char *)self + 0x378 + index * 4) = 0x3f800000;
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- LightSetLightColorAmbient_85a94.cpp ----
void Engine::LightSetLightColorAmbient(float red, float green, float blue, unsigned int light) {
    Engine *self = this;
    unsigned int index = light - 0x4000;
    if (index > 7) {
        return;
    }
    int count = light - 0x3fff;
    int current = self->field_0x32c;
    if (current > count) {
        count = current;
    }
    self->field_0x32c = count;

    char *src = (char *)self + index * 0x10;
    *(float *)(src + 0x268) = red;
    *(float *)(src + 0x26c) = green;
    *(float *)(src + 0x270) = blue;
    *(uint32_t *)(src + 0x274) = 0x3f800000;
    if (g_Engine_useShaders == 0) {
        return glLightfv(light, 0x1200, src + 0x268);
    }
    char *dst = (char *)self + index * 0x0c;
    *(float *)(dst + 0x2cc) =
        (self->field_0x288 + red) * self->field_0x2a8;
    *(float *)(dst + 0x2d0) =
        (self->field_0x28c + *(float *)(src + 0x26c)) *
        self->field_0x2ac;
    *(float *)(dst + 0x2d4) =
        (self->field_0x290 + *(float *)(src + 0x270)) *
        self->field_0x2b0;
    return ShaderUpdateMaterialColor();
}

// ---- ShaderSetInActive_86334.cpp ----
typedef void ShaderInactive(ShaderBaseStruct *);

void Engine::ShaderSetInActive() {
    Engine *self = this;
    ShaderBaseStruct *shader =
        *(ShaderBaseStruct **)(self->field_0x514 + g_Engine_activeShader * 4);
    void **vtable = *(void ***)shader;
    return ((ShaderInactive *)vtable[0x10 / 4])(shader);
}

// ---- LightSetLightColorDiffuse_859e0.cpp ----
void Engine::LightSetLightColorDiffuse(float red, float green, float blue, unsigned int light) {
    Engine *self = this;
    unsigned int index = light - 0x4000;
    if (index > 7) {
        return;
    }
    int count = light - 0x3fff;
    int current = self->field_0x32c;
    if (current > count) {
        count = current;
    }
    self->field_0x32c = count;

    char *src = (char *)self + index * 0x10;
    *(float *)(src + 0x228) = red;
    *(float *)(src + 0x22c) = green;
    *(float *)(src + 0x230) = blue;
    *(uint32_t *)(src + 0x234) = 0x3f800000;
    if (g_Engine_useShaders == 0) {
        return glLightfv(light, 0x1201, src + 0x228);
    }
    char *dst = (char *)self + index * 0x0c;
    *(float *)(dst + 0x2fc) = self->field_0x298 * red;
    *(float *)(dst + 0x300) = *(float *)(src + 0x22c) * self->field_0x29c;
    *(float *)(dst + 0x304) = *(float *)(src + 0x230) * self->field_0x2a0;
    return ShaderUpdateMaterialColor();
}

// ---- Engine_83eec.cpp ----
void Engine::Engine() {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    ((String *)((String *)self))->ctor();
    ((String *)((String *)((char *)self + 0x14)))->ctor();
    ((String *)((String *)((char *)self + 0x3c)))->ctor();
    ((String *)((String *)((char *)self + 0x4c)))->ctor();
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
    Array_int_ctor((char *)self + 0x3d8);
    self->field_0x478 = 0;
    self->field_0x400 = 0;
    self->field_0x468 = up;
    self->field_0x3f0 = up;
    Array_ShaderBaseStruct_ptr_ctor((char *)self + 0x510);
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
    for (int i = 0x1f; i != 0x33; i += 1) {
        *(int *)((char *)self + i * 4) = -1;
    }
    self->field_0x48c = -1;
    self->field_0x490 = -1;
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
    void *manager = operator new(0xc0);
    ((ApplicationManager *)((ApplicationManager *)manager))->ctor(self);
    self->field_0x30 = (char **)manager;
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
    ((Engine *)(self))->initFileInterface();
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- SetTextures_84c44.cpp ----
void Engine::SetTextures(uint32_t first, uint32_t second) {
    Engine *self = this;
    char *manager = *self->field_0x30;   // external texture-manager (no named struct)
    uint32_t count = *(uint32_t *)(manager + 0x10);
    if (count == 0 || first > count - 1) {
        return;
    }
    ((Engine *)(self))->SetTextureSlot(first, 0);
    if (second > count - 1) {
        if (self->field_0x80 != -1) {
            if (g_Engine_useShaders == 0) {
                glActiveTexture(0x84c1);
                glDisable(0xde1);
                glActiveTexture(0x84c0);
            }
            self->field_0x80 = -1;
        }
        return;
    }
    uint32_t texture = **(uint32_t **)(*(char **)(manager + 0x14) + second * 4);
    if (self->field_0x80 != texture) {
        glActiveTexture(0x84c1);
        ((Engine *)(self))->GlEnable(0xde1, true);
        unsigned int target = (g_Engine_useShaders != 0 &&
                               (self->field_0x420 & 0x80008) != 0)
                                  ? 0x8513
                                  : 0xde1;
        glBindTexture(target, texture);
        self->field_0x80 = texture;
    }
}

// ---- ShaderInit_846ac.cpp ----
typedef void ShaderCtor(void *);


uint32_t Engine::ShaderInit() {
    Engine *self = this;
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
        ((Engine *)(self))->ShaderRegister((ShaderBaseStruct *)shader);
    }
    glGetError();
    return 1;
}

// ---- SetEyePosition_850c0.cpp ----
extern "C" void Vector_assign(Vector *dst, const Vector *src);   // 0x6eb3c

uint64_t Engine::SetEyePosition(uint32_t x, uint32_t y, uint32_t z) {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    uint32_t buf[3];
    buf[0] = x;
    buf[1] = y;
    buf[2] = z;
    Vector_assign((Vector *)((char *)self + 0x3fc), (const Vector *)buf);
    uint32_t saved = (uint32_t)(__UINTPTR_TYPE__)cookie;
    uint32_t current = (uint32_t)(__UINTPTR_TYPE__)*(void *volatile *)&__stack_chk_guard;
    uint32_t guardDelta = current - saved;
    if (guardDelta == 0) {
        return (uint64_t)buf[0] | ((uint64_t)buf[1] << 32);
    }
    __stack_chk_fail(guardDelta);
}

// ---- SetModelMatrix_851f4.cpp ----
void Engine::SetModelMatrix(const uint32_t *matrix) {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
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
        __aeabi_memcpy((char *)self + 0x144, gl, 0x40);
        Vector tmp;
        if (self->field_0x378 == 0.0f) {
            AEMath_MatrixInverseRotateVector(&tmp, (const Matrix *)matrix,
                                             (const Vector *)((char *)self + 0x468));
            AEMath_VectorNormalize(&tmp, &tmp);
            self->field_0x330 = tmp;
        } else {
            self->field_0x330 = self->field_0x468;
        }
        if (self->field_0x32c > 1) {
            if (self->field_0x37c == 0.0f) {
                AEMath_MatrixInverseRotateVector(&tmp, (const Matrix *)matrix,
                                                 (const Vector *)((char *)self + 0x474));
                AEMath_VectorNormalize(&tmp, &tmp);
                self->field_0x33c = tmp;
            } else {
                self->field_0x33c = self->field_0x474;
            }
        }
        ((Engine *)(self))->ShaderUpdate();
        AEMath_MatrixInverseTransformVector(&tmp, (const Matrix *)matrix,
                                            (const Vector *)((char *)self + 0x3fc));
        *(Vector *)&self->field_0x34c = tmp;
        self->field_0x34c /= *(float *)(matrix + 12);
        self->field_0x350 /= *(float *)(matrix + 13);
        self->field_0x354 /= *(float *)(matrix + 14);
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- LightSetLight_85788.cpp ----
void Engine::LightSetLight(unsigned int light) {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    uint32_t values[4] = {0, 0, 0, 0};
    unsigned int index = light - 0x4000;
    if (index < 8) {
        int count = light - 0x3fff;
        int current = self->field_0x32c;
        if (current > count) {
            count = current;
        }
        self->field_0x32c = count;

        char *vector = (char *)self + 0x468 + index * 0x0c;
        values[0] = *(uint32_t *)(vector + 0x0);
        values[1] = *(uint32_t *)(vector + 0x4);
        values[2] = *(uint32_t *)(vector + 0x8);
        values[3] = *(uint32_t *)((char *)self + 0x378 + index * 4);
        if (g_Engine_useShaders == 0) {
            glLightfv(light, 0x1203, values);
        }
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- SetTexturesExt_84b0c.cpp ----
void Engine::SetTexturesExt(uint32_t first, uint32_t second, uint32_t third, ...) {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    char *manager = *self->field_0x30;   // external texture-manager (no named struct)
    if (*(uint32_t *)(manager + 0x10) != 0) {
        uint32_t values[3] = {first, second, third};
        uint32_t slot = 0;
        uint32_t *p = values;
        while (*p != 0xffffffff) {
            ((Engine *)(self))->SetTextureSlot(*p, slot);
            slot += 1;
            p += 1;
        }
        for (uint32_t i = slot; i < 0x14; i += 1) {
            *(uint32_t *)((char *)self + 0x7c + i * 4) = 0xffffffff;
        }
        glActiveTexture(0x84c0);
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- SetWorldViewMatrix_853b0.cpp ----
void Engine::SetWorldViewMatrix(const uint32_t *matrix) {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    if (g_Engine_useShaders != 0) {
        uint32_t gl[16] = {
            matrix[0], matrix[4], matrix[8], 0,
            matrix[1], matrix[5], matrix[9], 0,
            matrix[2], matrix[6], matrix[10], 0,
            matrix[3], matrix[7], matrix[11], 0x3f800000,
        };
        __aeabi_memcpy((char *)self + 0x184, gl, 0x40);
        esMatrixMultiply((char *)self + 0x104, gl, (char *)self + 0x384);
    } else {
        MatrixGetGL((const Matrix *)matrix, (float *)((char *)self + 0x428));
        uint32_t saved0 = (uint32_t)(__UINTPTR_TYPE__)cookie;
        uint32_t current0 = (uint32_t)(__UINTPTR_TYPE__)*(void *volatile *)&__stack_chk_guard;
        uint32_t delta0 = current0 - saved0;
        if (delta0 == 0) {
            return glLoadMatrixf((float *)((char *)self + 0x428));
        }
        __stack_chk_fail(delta0);
    }
    uint32_t saved1 = (uint32_t)(__UINTPTR_TYPE__)cookie;
    uint32_t current1 = (uint32_t)(__UINTPTR_TYPE__)*(void *volatile *)&__stack_chk_guard;
    uint32_t delta1 = current1 - saved1;
    if (delta1 == 0) {
        return;
    }
    __stack_chk_fail(delta1);
}

// ---- ResetLightParam_84348.cpp ----
void Engine::ResetLightParam() {
    Engine *self = this;
    uint32_t volatile cookie = __stack_chk_guard;
    self->field_0x488 = 0x3f800000;
    self->field_0x32c = 1;
    self->field_0x298 = 0.8f;
    self->field_0x29c = 0.8f;
    self->field_0x2a0 = 0.8f;
    self->field_0x2a4 = 0x3f800000;
    self->field_0x2a8 = 0.2f;
    self->field_0x2ac = 0.2f;
    self->field_0x2b0 = 0.2f;
    self->field_0x2b4 = 0x3f800000;
    self->field_0x2b8 = 0;
    self->field_0x2c0 = 0x3f800000ULL;
    self->field_0x2c8 = 0;
    self->field_0x268 = 0;
    self->field_0x270 = 0;
    self->field_0x274 = 0x3f800000;

    Vector up;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    self->field_0x468 = up;
    self->field_0x474 = up;
    self->field_0x378 = 0;
    self->field_0x37c = 0;

    if (g_Engine_useShaders == 0) {
        glLightfv(0x4000, 0x1200, (char *)self + 0x268);
        glLightfv(0x4000, 0x1201, (char *)self + 0x228);
        glLightfv(0x4000, 0x1202, (char *)self + 0x248);
        glMaterialfv(0x408, 0x1200, (char *)self + 0x2a8);
        glMaterialfv(0x408, 0x1201, (char *)self + 0x298);
        glMaterialfv(0x408, 0x1202, (char *)self + 0x2b8);
        glMaterialf(0x408, 0x1601, self->field_0x2c8);
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}

// ---- LightSetLightColorSpecular_85b60.cpp ----
void Engine::LightSetLightColorSpecular(float red, float green, float blue, unsigned int light) {
    Engine *self = this;
    unsigned int index = light - 0x4000;
    if (index > 7) {
        return;
    }
    int count = light - 0x3fff;
    int current = self->field_0x32c;
    if (current > count) {
        count = current;
    }
    self->field_0x32c = count;

    char *src = (char *)self + index * 0x10;
    *(float *)(src + 0x248) = red;
    *(float *)(src + 0x24c) = green;
    *(float *)(src + 0x250) = blue;
    *(uint32_t *)(src + 0x254) = 0x3f800000;
    if (g_Engine_useShaders == 0) {
        return glLightfv(light, 0x1202, src + 0x248);
    }
    char *dst = (char *)self + index * 0x0c;
    *(float *)(dst + 0x2e4) = self->field_0x2b8 * red;
    *(float *)(dst + 0x2e8) = *(float *)(src + 0x24c) * self->field_0x2bc;
    *(float *)(dst + 0x2ec) = *(float *)(src + 0x250) * self->field_0x2c0;
    return ShaderUpdateMaterialColor();
}

// ---- GlEnable_84d7c.cpp ----
void Engine::GlEnable(unsigned int cap, bool enable) {
    Engine *self = this;
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
    uint32_t flags = self->field_0x420;
    flags = enable ? (flags | bit) : (flags & ~bit);
    self->field_0x420 = flags;
}

// ---- LightSetMaterialColorAmbient_85ca0.cpp ----
void Engine::LightSetMaterialColorAmbient(float red, float green, float blue) {
    Engine *self = this;
    self->field_0x2a8 = red;
    self->field_0x2ac = green;
    self->field_0x2b0 = blue;
    self->field_0x2b4 = self->field_0x488;

    if (g_Engine_useShaders == 0) {
        return glMaterialfv(0x408, 0x1200, (char *)self + 0x2a8);
    }

    int count = self->field_0x32c;
    int sourceOffset = 0;
    int destOffset = 0;
    for (int index = 0; index < count; index += 1) {
        char *source = (char *)self + sourceOffset;
        char *dest = (char *)self + destOffset;
        sourceOffset += 0x10;
        destOffset += 0x0c;
        *(float *)(dest + 0x2cc) =
            (self->field_0x288 + *(float *)(source + 0x268)) * red;
        *(float *)(dest + 0x2d0) =
            (self->field_0x28c + *(float *)(source + 0x26c)) * green;
        *(float *)(dest + 0x2d4) =
            (self->field_0x290 + *(float *)(source + 0x270)) * blue;
    }
    return ShaderUpdateMaterialColor();
}
