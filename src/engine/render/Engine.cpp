

#include <GLES2/gl2.h>
#include <GLES/gl.h>
extern "C" void glClientActiveTexture(GLenum texture); // lint: extern_c (GLES1 C ABI entry point, defined in GLES1Compat.cpp)
#include "engine/core/AbyssEngine.h"
#include "engine/math/AEMath.h"
#include "engine/render/FBOContainer.h"
#include "engine/render/Engine.h"
AbyssEngine::Engine *gEngine = nullptr;

bool AbyssEngine::Engine::vboSupported = false;
bool AbyssEngine::Engine::clampTextures = false;
bool AbyssEngine::Engine::vfc = false;
float AbyssEngine::Engine::lodBiasDiffuse = 0.0f;
float AbyssEngine::Engine::lodBiasNormal = 0.0f;
unsigned int AbyssEngine::Engine::countryCode = 0;
bool AbyssEngine::Engine::EnablePostEffect = false;
bool AbyssEngine::Engine::CheckForOrientationChange;
bool AbyssEngine::Engine::fogEnabled = false;
bool AbyssEngine::PostEffectFlag;
#include "engine/core/ApplicationManager.h"
#include "engine/core/NFC.h"
#include "engine/file/AEFile.h"
#include "engine/file/FileInterfaceAndroid.h"
#include "game/core/String.h"
#include "engine/render/Mesh.h"
#include "engine/render/Material.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/ShaderBaseStruct.h"
#include "engine/render/shaders/TextureShader.h"
#include "engine/render/shaders/SimpleShader.h"
#include "engine/render/shaders/TextureVtxColorShader.h"
#include "engine/render/shaders/NoTexShader.h"
#include "engine/render/shaders/NoTexVtxColorShader.h"
#include "engine/render/shaders/TextureAlphaTestShader.h"
#include "engine/render/shaders/CubeMapping.h"
#include "engine/render/shaders/CubeNormalMapping.h"
#include "engine/render/shaders/BumpMapping.h"
#include "engine/render/shaders/SandboxShader.h"
#include "engine/render/shaders/GenericShader.h"
#include "engine/render/shaders/GenericShader1.h"
#include "engine/render/shaders/GenericShader2.h"
#include "engine/render/shaders/TexOnlyShader.h"
#include "game/core/BumpShaderV2.h"
#include "game/core/BumpShaderV3.h"
#include "engine/render/shaders/VertexColorShader.h"
#include "engine/render/shaders/VertexColorAlphaTextureShader.h"
#include "engine/render/shaders/PulseShader.h"
#include "engine/render/shaders/BumpShader.h"
#include "game/core/BumpShaderV4.h"
#include "engine/render/shaders/BloomShader.h"
#include "engine/render/shaders/BlurShader.h"
#include "engine/render/shaders/PostBWShader.h"
#include "engine/render/shaders/TextureLightShader.h"
#include "engine/render/shaders/BumpRimCubeShader.h"
#include "game/core/BumpShaderCloak.h"
#include "engine/render/shaders/SpecCubeMapping.h"
#include "engine/render/shaders/SpecCubeAlphaMapping.h"
#include "engine/render/shaders/EnergyShield.h"
#include "engine/render/shaders/SimpleRefractionShader.h"
#include "engine/render/shaders/GlowShader.h"
#include "engine/render/shaders/GlowPPShader.h"
#include "engine/render/shaders/DrawFBOShader.h"
#include "engine/render/shaders/ColorMixAdd.h"
#include "game/core/BumpShaderParticle.h"
#include "engine/render/shaders/DNSShader.h"
#include "game/core/BumpShaderRefract.h"
#include "game/core/BumpRimCubeShader_new.h"
#include <arm_neon.h>
#include <cstdarg>
#include <cstring>

namespace AbyssEngine {
    int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
                   signed char vertexFormat, Mesh **out);
    void MeshRelease(Engine *engine, Mesh **slot);
    void esMatrixMultiply(ESMatrix *out, ESMatrix *a, ESMatrix *b);
}


namespace {

    bool g_Engine_supportsFBO;


    int g_Engine_shaderPostA;
    int g_Engine_shaderPostB;
    int g_Engine_shaderPostC;


    // Post-effect ids (0x1400000 base) and the black/white effect's switch-off bookkeeping.
    const uint32_t kPostEffectBase = 0x1400000;
    const uint32_t kPostEffectBlur = 0x1400001;
    const uint32_t kPostEffectBW = 0x1400002;
    bool g_Engine_postEffectFlag;
    int g_Engine_postEffectCounter;
    int g_Engine_postEffectPending;

    struct TextureEntry {
        uint32_t glTexture;
        unsigned char pad_04[0x10 - 0x04];
        float texEnv;
        unsigned char isCube;
    };
#if __SIZEOF_POINTER__ == 4
    static_assert(__builtin_offsetof(TextureEntry, glTexture) == 0x00, "TextureEntry::glTexture");
    static_assert(__builtin_offsetof(TextureEntry, texEnv) == 0x10, "TextureEntry::texEnv");
    static_assert(__builtin_offsetof(TextureEntry, isCube) == 0x14, "TextureEntry::isCube");
#endif
}

void glError() {
    glGetError();
}

double *Engine::GetAccelValue() {
    double x = this->accelRaw[0];
    double y;
    if (this->appManager->paintCanvas->gameOrientation == 1) {
        x = -x;
        y = -this->accelRaw[1];
    } else {
        y = this->accelRaw[1];
    }
    this->accelValue[1] = y;
    this->accelValue[0] = x;
    this->accelValue[2] = this->accelRaw[2];
    return (double *) this->accelValue;
}

void Engine::ActivateRender2FracFBO() {
    FBOContainer *fbo = this->refractFBO;
    if (fbo != 0) {
        return fbo->BeginCapture();
    }
}

uint32_t Engine::Resume() {
    this->appManager->paintCanvas->Resume();
    for (int index = 0; index != 0x14; index += 1) {
        this->boundTextures[index] = -1;
    }
    return 1;
}

uint32_t Engine::Suspend() {
    this->appManager->paintCanvas->Suspend();
    return 1;
}

uint32_t Engine::GetDisplayWidth() {
    return this->displayWidth;
}

double *Engine::GetGravValue() {
    double x = this->gravRaw[0];
    double y;
    if (this->appManager->paintCanvas->gameOrientation == 1) {
        x = -x;
        y = -this->gravRaw[1];
    } else {
        y = this->gravRaw[1];
    }
    this->gravValue[1] = y;
    this->gravValue[0] = x;
    this->gravValue[2] = this->gravRaw[2];
    return (double *) this->gravValue;
}

uint32_t Engine::GetDisplayHeight() {
    return this->displayHeight;
}

void Engine::LightSetRimColor(float red, float green, float blue) {
    if (Engine::enableShader == 0) {
        return;
    }
    this->rimColor.x = red;
    this->rimColor.y = green;
    this->rimColor.z = blue;
    return this->ShaderUpdate();
}

bool Engine::IsPostEffectActivated() {
    return this->postEffectFlags != 0;
}

void Engine::SetUVMatrix(const Matrix &matrix) {
    if (Engine::enableShader == 0) {
        glMatrixMode(0x1702);
        MatrixGetGL(matrix, this->uvMatrixGL);
        glLoadMatrixf(this->uvMatrixGL);
        return glMatrixMode(0x1700);
    }

    const float *m = matrix.m;
    float m0 = m[0];
    float m1 = m[1];
    float m2 = m[2];
    float m3 = m[3];
    float m4 = m[4];
    float m5 = m[5];
    float m6 = m[6];
    float m7 = m[7];
    float m8 = m[8];
    float m9 = m[9];
    float m10 = m[10];
    float m11 = m[11];

    float *uv = this->uvMatrix;
    uv[0] = m0;
    uv[1] = m4;
    uv[2] = m8;
    uv[3] = 0.0f;
    uv[4] = m1;
    uv[5] = m5;
    uv[6] = m9;
    uv[7] = 0.0f;
    uv[8] = m2;
    uv[9] = m6;
    uv[10] = m10;
    uv[11] = 0.0f;
    uv[12] = m3;
    uv[13] = m7;
    uv[14] = m11;
    uv[15] = 1.0f;
}

void Engine::ActivateRender2TextureFBO() {
    FBOContainer *fbo = this->postEffectFBO;
    if (fbo != 0) {
        return fbo->BeginCapture();
    }
}

void Engine::LightSetMaterialColorAlpha(float alpha) {
    if (this->lightingEnabled == 0) {
        return;
    }

    this->materialAmbient[3] = alpha;
    this->materialAlpha = alpha;
    glMaterialfv(0x408, 0x1200, this->materialAmbient);
    this->materialSpecular[3] = this->materialAlpha;
    glMaterialfv(0x408, 0x1202, this->materialSpecular);
    this->materialDiffuse[3] = this->materialAlpha;
    return glMaterialfv(0x408, 0x1201, this->materialDiffuse);
}

void Engine::SetAccelValue(double x, double y, double z) {
    this->accelRaw[0] = x;
    this->accelRaw[1] = y;
    this->accelRaw[2] = z;
}

void Engine::ResetUVMatrix() {
    if (Engine::enableShader != 0) {
        float *uv = this->uvMatrix;
        uv[0] = 1.0f;
        uv[1] = 0.0f;
        uv[2] = 0.0f;
        uv[3] = 0.0f;
        uv[4] = 0.0f;
        uv[5] = 1.0f;
        uv[6] = 0.0f;
        uv[7] = 0.0f;
        uv[8] = 0.0f;
        uv[9] = 0.0f;
        uv[10] = 1.0f;
        uv[11] = 0.0f;
        uv[12] = 0.0f;
        uv[13] = 0.0f;
        uv[14] = 0.0f;
        uv[15] = 1.0f;
        return;
    }

    glMatrixMode(0x1702);
    glLoadIdentity();
    glScalef(1.0f, -1.0f, 1.0f);
    return glMatrixMode(0x1700);
}

void Engine::ActivateTextureFBO() {
    FBOContainer *fbo = this->postEffectFBO;
    if (fbo != 0) {
        return fbo->Activate();
    }
}

void Engine::GlowEndGlow() {
    if (Engine::EnableGlow == 0) {
        return;
    }
    this->glowActive = 0;
    glColorMask(1, 1, 1, 1);
    return glDepthFunc(0x201);
}

void Engine::ActivateViewBuffer() {
    glBindFramebuffer(0x8d40, this->viewFramebuffer);
    return glViewport(0, 0, this->viewportWidth, this->viewportHeight);
}

void Engine::GlowEnableGlow() {
    if (this->glowActive != 0) {
        return;
    }
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(0x4000);
    this->glowActive = 1;
}

void Engine::SetOnDestroyApp(DestroyCallback *callback) {
    this->onDestroyCallback = callback;
}

void Engine::SetGravValue(double x, double y, double z) {
    this->gravRaw[0] = x;
    this->gravRaw[1] = y;
    this->gravRaw[2] = z;
}

void Engine::SwapBuffer() {
    uint32_t index = 0;
    while (index < this->triangleCounts.size()) {
        this->triangleCounts[index] = 0;
        index += 1;
    }
}

void Engine::ReloadShaders() {
    uint32_t index = 0;
    while (index < this->shaders.size()) {
        ShaderBaseStruct *shader = this->shaders[index];
        shader->DeleteShader();

        shader = this->shaders[index];
        shader->Init(this);
        index += 1;
    }
}

void Engine::DeactivateRender2TextureFBO() {
    FBOContainer *fbo = this->postEffectFBO;
    if (fbo != 0) {
        return fbo->EndCapture();
    }
}

DeviceInfo Engine::GetDeviceInfo() {
    DeviceInfo info;
    info.isPad = NFC().isPad();
    info.width = NFC().getWidth();
    info.height = NFC().getHeight();
    return info;
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

bool Engine::HasVibration() {
    if (this->hasVibration) {
        return this->vibrationSupported != 0;
    }
    return false;
}

void Engine::LightSetLightCount(int count) {
    if (count >= 8) {
        count = 8;
    }
    count &= ~(count >> 31);
    this->lightCount = count;
}

void Engine::SetAddData(void *data, int size) { // lint: void_ptr exported method signature (addData member typed in Engine.h)
    // lint: void_ptr exported method signature (addData member typed in Engine.h)
    this->addData = (unsigned char *) data;
    this->addDataSize = size;
}

void Engine::ShaderUpdate() {
    uint32_t index = 0;
    while (index < this->shaders.size()) {
        this->shaders[index]->Update();
        index += 1;
    }
}

bool Engine::IsExtensionSupported(const char *extension) {
    const char *extensions = (const char *) glGetString(0x1f03);

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
    this->materialShininess = shininess;
    if (Engine::enableShader == 0) {
        return glMaterialf(0x408, 0x1601, shininess);
    }
}

void Engine::Initialize(InitializeCallback *callback) {
    if (callback != 0) {
        callback(this);
    }
}

void Engine::ActivateRefractFBO() {
    FBOContainer *fbo = this->refractFBO;
    if (fbo != 0) {
        return fbo->Activate();
    }
}

void Engine::LightSetParticleAmbient(float red, float green, float blue) {
    this->field_0x314.x = red;  // particle ambient @0x314
    this->field_0x314.y = green;
    this->field_0x314.z = blue;
}

void Engine::DeactivateRender2FracFBO() {
    FBOContainer *fbo = this->refractFBO;
    if (fbo != 0) {
        return fbo->EndCapture();
    }
}

void Engine::SetPerspMatrix(float *matrix) {
    if (Engine::enableShader != 0) {
        float *proj = this->projMatrix;
        proj[0] = matrix[0];
        proj[1] = matrix[1];
        proj[2] = matrix[2];
        proj[3] = matrix[3];
        proj[4] = matrix[4];
        proj[5] = matrix[5];
        proj[6] = matrix[6];
        proj[7] = matrix[7];
        proj[8] = matrix[8];
        proj[9] = matrix[9];
        proj[10] = matrix[10];
        proj[11] = matrix[11];
        proj[12] = matrix[12];
        proj[13] = matrix[13];
        proj[14] = matrix[14];
        proj[15] = matrix[15];
    }
}

void Engine::SetFrameBufferTexture(int slot0, int slot1) {
    if (this->frameBufferTextures[slot0] != -1) {
        glActiveTexture(0x84c0);
        glBindTexture(0xde1, this->frameBufferTextures[slot0]);
    }
    if (slot1 != -1 && this->frameBufferTextures[slot1] != -1) {
        glActiveTexture(0x84c1);
        glBindTexture(0xde1, this->frameBufferTextures[slot1]);
    }
}

void Engine::LightSetLightDirection(float x, float y, float z, unsigned int light) {
    unsigned int index = light - 0x4000;
    if (index < 8) {
        int count = light - 0x3fff;
        int current = this->lightCount;
        if (current > count) {
            count = current;
        }
        this->lightCount = count;

        Vector normalized =
                AbyssEngine::AEMath::VectorNormalize(Vector{x, y, z});
        (&this->field_0x468)[index] = normalized;
        this->lightDirty[index] = 0.0f;
    }
    return;
}

void Engine::RenderMesh(Mesh *mesh) {
    // Triangle statistics: [0]/[1] = indexed 2D/3D (field_0x58 holds two counters),
    // triangleCountA/B = VBO-indexed 2D/3D; drawCallCountA/B = 2D/3D draw calls.
    int *indexedTriangles = (int *) &this->field_0x58;
    if (mesh == 0 || mesh->indexCount == 0) {
        return;
    }

    if (Engine::enableShader != 0) {
        ShaderSet = 0;
        uint32_t flags = this->glEnableFlags;
        if ((flags & 0x1000) != 0) {
            this->ShaderSetActive(GenericShader::ShaderIndex, mesh);
        }         else if ((flags & 0x2000) != 0) {
            this->ShaderSetActive(GenericShader1::ShaderIndex, mesh);
        }         else if ((flags & 0x4000) != 0) {
            this->ShaderSetActive(GenericShader2::ShaderIndex, mesh);
        }         else if ((flags & 0x400) != 0) {
            this->ShaderSetActive(SandboxShader::ShaderIndex, mesh);
        }         else if ((flags & 0x4) != 0) {
            this->ShaderSetActive(BumpMapping::ShaderIndex, mesh);
        }         else if ((flags & 0x10000) != 0) {
            this->ShaderSetActive(BumpShaderV3::ShaderIndex, mesh);
        }         else if ((flags & 0x1000000) != 0) {
            this->ShaderSetActive(BumpShaderParticle::ShaderIndex, mesh);
        }         else if ((flags & 0x40000) != 0) {
            this->ShaderSetActive(BumpShaderCloak::ShaderIndex, mesh);
        }         else if ((flags & 0x8000000) != 0) {
            this->ShaderSetActive(BumpShaderRefract::ShaderIndex, mesh);
        }         else if ((flags & 0x200000) != 0) {
            this->ShaderSetActive(EnergyShield::ShaderIndex, mesh);
        }         else if ((flags & 0x400000) != 0) {
            this->ShaderSetActive(SimpleRefractionShader::ShaderIndex, mesh);
        }         else if ((flags & 0x100000) != 0) {
            this->ShaderSetActive(GlowShader::ShaderIndex, mesh);
        }         else if ((flags & 0x4000000) != 0) {
            this->ShaderSetActive(DNSShader::ShaderIndex, mesh);
        } 
        else if ((flags & 0x20000) != 0) {
            if (Engine::UseAdvancedShader != 0) {
                this->ShaderSetActive(BumpRimCubeShader::ShaderIndex, mesh);
            } else {
                this->ShaderSetActive(BumpRimCubeShader_new::ShaderIndex, mesh);
            }
        } else if ((flags & 0x2000000) != 0) {
            // shader already bound by the caller
        } else if ((flags & 0x8) != 0) {
            this->ShaderSetActive(SpecCubeMapping::ShaderIndex, mesh);
        } else if ((flags & 0x80000) != 0) {
            this->ShaderSetActive(SpecCubeAlphaMapping::ShaderIndex, mesh);
        } else if ((flags & 0x8000) != 0) {
            this->ShaderSetActive(CubeNormalMapping::ShaderIndex, mesh);
        } else if ((flags & 0x2) != 0) {
            this->ShaderSetActive(TextureAlphaTestShader::ShaderIndex, mesh);
        } else if ((flags & 0x1) == 0) {
            this->ShaderSetActive(NoTexShader::ShaderIndex, mesh);
        } else if ((mesh->vertexFormat & 0xa) == 8) {
            this->ShaderSetActive(NoTexVtxColorShader::ShaderIndex, mesh);
        } else if (this->lightingEnabled) {
            this->ShaderSetActive(TextureLightShader::ShaderIndex, mesh);
        } else if ((mesh->vertexFormat & 8) != 0) {
            this->ShaderSetActive(TextureVtxColorShader::ShaderIndex, mesh);
        } else if ((flags & 0x800000) != 0) {
            this->ShaderSetActive(ColorMixAdd::ShaderIndex, mesh);
        } else {
            this->ShaderSetActive(TextureShader::ShaderIndex, mesh);
        }

        if (ShaderSet != 0) {
            int oldBuffer;
            glGetIntegerv(0x8ca6, &oldBuffer);
            if ((mesh->vertexFormat & 0x10) == 0) {
                glDrawArrays(4, 0, mesh->vertexCount);
            } else {
                unsigned int count = mesh->indexCount;
                unsigned int triangles = count / 3;
                if (mesh->uploaded != 0) {
                    if (this->statsEnabled != 0) {
                        this->triangleCountB += triangles;
                        this->drawCallCountB += 1;
                    } else {
                        this->triangleCountA += triangles;
                        this->drawCallCountA += 1;
                    }
                    glBindBuffer(0x8893, mesh->indexVBO);
                    glDrawElements(4, mesh->indexCount, 0x1403, 0);
                    glBindBuffer(0x8892, 0);
                    glBindBuffer(0x8893, 0);
                } else {
                    if (this->statsEnabled != 0) {
                        indexedTriangles[1] += triangles;
                        this->drawCallCountB += 1;
                    } else {
                        indexedTriangles[0] += triangles;
                        this->drawCallCountA += 1;
                    }
                    glDrawElements(4, count, 0x1403, mesh->indices);
                }
            }
            this->shaders[currentUsedShaderIndex]->SetInActive();
        }
        return;
    }

    glVertexPointer(3, 0x1406, 0, mesh->positions);
    this->AEClientState(0x8074, true);
    if ((mesh->vertexFormat & 2) == 0) {
        this->AEClientState(0x8078, false);
    } else if (mesh->material == 0 || ((Material *) mesh->material)->textures[1] == -1) {
        glTexCoordPointer(2, 0x1406, 0, mesh->texCoords);
        this->AEClientState(0x8078, true);
    }
    if ((mesh->vertexFormat & 4) == 0) {
        this->AEClientState(0x8075, false);
    } else {
        glNormalPointer(0x1406, 0, mesh->normals);
        this->AEClientState(0x8075, true);
    }
    if ((mesh->vertexFormat & 8) == 0) {
        this->AEClientState(0x8076, false);
    } else {
        glColorPointer(4, 0x1406, 0, mesh->colors);
        this->AEClientState(0x8076, true);
    }
    if ((mesh->vertexFormat & 0x10) == 0) {
        glDrawArrays(4, 0, mesh->vertexCount);
    } else {
        unsigned int count;
        if (this->field_0xfe != 0) {
            count = mesh->indexCount;
            unsigned int triangles = count / 3;
            if (this->statsEnabled != 0) {
                indexedTriangles[1] += triangles;
                this->drawCallCountB += 1;
            } else {
                indexedTriangles[0] += triangles;
                this->drawCallCountA += 1;
            }
        } else {
            count = mesh->indexCount;
        }
        glDrawElements(4, count, 0x1403, mesh->indices);
    }
    if ((mesh->vertexFormat & 2) != 0 && mesh->material != 0 &&
        ((Material *) mesh->material)->textures[1] != -1) {
        this->AEClientState(0x8078, false);
        return glClientActiveTexture(0x84c0);
    }
}

void Engine::DrawQuad(int x, int y, int width, int height) {
    float fx = (float) x;
    float fy = (float) y;
    float right = (float) (x + width);
    float bottom = (float) (height + y);

    Mesh *mesh = this->quadMesh;
    float *positions = (float *) mesh->positions;
    positions[0] = fx;
    positions[1] = fy;
    positions[3] = right;
    positions[4] = fy;
    positions[6] = right;
    positions[7] = bottom;
    positions[9] = fx;
    positions[10] = bottom;

    float *uv = (float *) mesh->texCoords;
    uv[0] = 0.0f;
    uv[1] = 1.0f;
    uv[2] = 1.0f;
    uv[3] = 1.0f;
    uv[4] = 1.0f;
    uv[5] = 0.0f;
    uv[6] = 0.0f;
    uv[7] = 0.0f;

    return glDrawElements(4, mesh->indexCount, 0x1403,
                          mesh->indices);
}

void Engine::SetColor(float red, float green, float blue, float alpha) {
    if (this->glColor[0] != red || this->glColor[1] != green || this->glColor[2] != blue ||
        this->glColor[3] != alpha) {
        this->glColor[0] = red;
        this->glColor[1] = green;
        this->glColor[2] = blue;
        this->glColor[3] = alpha;
        this->packedColor =
                (int) (green * 255.0f) * 0x10000 + (int) (red * 255.0f) * 0x1000000 +
                (int) (blue * 255.0f) * 0x100 + (int) (alpha * 255.0f);
        if (Engine::enableShader != 0) {
            return this->ShaderUpdate();
        }
        this->LightSetMaterialColorAlpha(alpha);
        return glColor4f(red, green, blue, alpha);
}
}

Engine::~Engine() {
    DestroyCallback *destroy = this->onDestroyCallback;
    if (destroy != 0) {
        destroy(this);
    }

    delete this->appManager;
    this->appManager = nullptr;

    delete (FileInterface *) this->fileInterface;
    this->fileInterface = 0;

    AEFile::Release();
    ArrayReleaseClasses(this->shaders);

    delete this->postEffectFBO;
    this->postEffectFBO = 0;

    delete this->refractFBO;
    this->refractFBO = 0;

    AbyssEngine::MeshRelease(this, &this->quadMesh);
    this->ReleaseGL();
}

void Engine::ReleaseGL() {
}

void Engine::AfterGLInit() {
    this->ResetLightParam();
    AbyssEngine::MeshCreate(this, 4, 2, 0x13, &this->quadMesh);

    unsigned short *indices = (unsigned short *) this->quadMesh->indices;
    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 1;
    indices[3] = 0;
    indices[4] = 3;
    indices[5] = 2;

    Engine::vendor = String((const char *) glGetString(0x1f00));
    Engine::renderer = String((const char *) glGetString(0x1f01));
}

void Engine::DrawCloakFBO(FBOContainer *fbo) {
    if (Engine::enableShader != 0) {
        ShaderBaseStruct *shader = this->shaders[DrawFBOShader::ShaderIndex];
        shader->RenderEffect(fbo, this);
    }
}

void Engine::ShaderRegister(ShaderBaseStruct *shader) {
    if (shader != 0) {
        char *name = shader->GetShaderName().GetAEChar();
        shader->Init(this);
        ArrayAdd(shader, this->shaders);
        ArrayAdd(0, this->triangleCounts);
        delete[] name;
    }
}

void Engine::SetTextureSlot(uint32_t textureIndex, uint32_t slot) {
    uint32_t count = this->appManager->paintCanvas->cubeTextures.count;
    if (count == 0) {
        return;
    }
    if (slot >= 8 || textureIndex > count - 1) {
        return;
    }
    uint32_t *bound = (uint32_t *) &this->boundTextures[slot];
    uint32_t texture = ((TextureEntry *) this->appManager->paintCanvas->cubeTextures.data_[textureIndex])->glTexture;
    if (*bound == texture) {
        return;
    }
    glActiveTexture(slot + 0x84c0);
    float env = ((TextureEntry *) this->appManager->paintCanvas->cubeTextures.data_[textureIndex])->texEnv;
    if (currentLODBias != env) {
        currentLODBias = env;
        if (Engine::enableShader != 0) {
            if (BiasErrorOutputFlag != 0) {
                BiasErrorOutputFlag = 0;
            }
        } else {
            glTexEnvf(0x8500, 0x8501, env);
        }
    }
    glBindTexture(((TextureEntry *) this->appManager->paintCanvas->cubeTextures.data_[textureIndex])->isCube
                          ? 0x8513 : 0xde1,
                  texture);
    *bound = texture;
}

void Engine::AEClientState(unsigned int state, bool enable) {
    if (enable) {
        switch (state) {
            case 0x8074:
                if ((this->clientStateFlagsAE & 2) == 0) {
                    glEnableClientState(0x8074);
                    this->clientStateFlagsAE |= 2;
                }
                break;
            case 0x8075:
                if ((this->clientStateFlagsAE & 1) == 0) {
                    glEnableClientState(0x8075);
                    this->clientStateFlagsAE |= 1;
                }
                break;
            case 0x8076:
                if ((this->clientStateFlagsAE & 8) == 0) {
                    glEnableClientState(0x8076);
                    this->clientStateFlagsAE |= 8;
                }
                break;
            case 0x8078:
                if ((this->clientStateFlagsAE & 4) == 0) {
                    glEnableClientState(0x8078);
                    this->clientStateFlagsAE |= 4;
                }
                break;
        }
    } else {
        switch (state) {
            case 0x8074:
                if ((this->clientStateFlagsAE & 2) != 0) {
                    glDisableClientState(0x8074);
                    this->clientStateFlagsAE &= ~2u;
                }
                break;
            case 0x8075:
                if ((this->clientStateFlagsAE & 1) != 0) {
                    glDisableClientState(0x8075);
                    this->clientStateFlagsAE &= ~1u;
                }
                break;
            case 0x8078:
                if ((this->clientStateFlagsAE & 4) != 0) {
                    glDisableClientState(0x8078);
                    this->clientStateFlagsAE &= ~4u;
                }
                break;
            case 0x8076:
                if ((this->clientStateFlagsAE & 8) != 0) {
                    glDisableClientState(0x8076);
                    this->clientStateFlagsAE &= ~8u;
                }
                break;
        }
    }
}

void Engine::GlowBeginGlow(int depthFunc) {
    if (this->glowActive != 0) {
        return;
    }
    if (Engine::EnableGlow == 0) {
        return;
    }
    glColorMask(0, 0, 0, 1);
    this->GlowEnableGlow();
    if (this->glowActive != 0) {
        return glDepthFunc(depthFunc);
    }
}

void Engine::DrawLine2D(float *verts, int count, bool strip) {
    this->lineVertexBase = (int)(uintptr_t)verts;
    this->ShaderSetActive(NoTexShader::ShaderIndex, 0);
    if (strip) {
        return glDrawArrays(GL_LINE_LOOP, 0, count);
    }
    return glDrawArrays(GL_LINES, 0, count);
}

void Engine::ShaderSetActive(int shaderIndex, Mesh *mesh) {
    while (shaderIndex == -1) {
        shaderIndex = TextureShader::ShaderIndex;
        if ((mesh->vertexFormat & 2) == 0) {
            shaderIndex = SimpleShader::ShaderIndex;
        }
    }

    bool hasExtra = mesh != 0 && mesh->hasAnimation != 0;
    ShaderBaseStruct *shader = this->shaders[shaderIndex];
    if (shader == 0) {
        return;
    }
    ShaderSet = 1;

    if (shader->program != this->currentProgram) {
        shader->UseShader(hasExtra);
        shader = this->shaders[shaderIndex];
        this->currentProgram = shader->program;
        currentUsedShaderIndex = shaderIndex;
    }
    shader->UseShader(hasExtra);
    shader = this->shaders[shaderIndex];
    shader->UpdateMeshData(mesh, this);
    if (mesh != 0) {
        uint32_t triangles = ((unsigned) (mesh->indexCount) / (unsigned) (3));
        this->triangleCounts[shaderIndex] += triangles;
    }
}

void Engine::DoPostEffect() {
    uint32_t flags = this->postEffectFlags;
    FBOContainer *current = this->postEffectFBO;
    FBOContainer *other = this->refractFBO;
    if (Engine::enableShader != 0) {
        FBOContainer *slot = other;
        if ((flags & 2) != 0) {
            ShaderBaseStruct *shader = this->shaders[g_Engine_shaderPostA];
            if ((flags & ~2u) == 0) {
                shader->RenderEffect(current, this);
                flags = 0;
            } else {
                shader->RenderEffect(current, other, this);
                slot = other;
                other = current;
            }
        }
        if ((this->postEffectFlags & 1) != 0) {
            ShaderBaseStruct *shader = this->shaders[g_Engine_shaderPostB];
            if ((flags & ~1u) == 0) {
                shader->RenderEffect(slot, this);
                flags = 0;
            } else {
                shader->RenderEffect(slot, other, this);
                slot = other;
            }
        }
        if ((this->postEffectFlags & 4) != 0) {
            ShaderBaseStruct *shader = this->shaders[g_Engine_shaderPostC];
            if ((flags & ~4u) == 0) {
                shader->RenderEffect(slot, this);
            } else {
                shader->RenderEffect(slot, other, this);
            }
        }
        if (g_Engine_postEffectFlag == 1) {
            this->SetPostEffect(kPostEffectBW, false);
        }
    }
    return;
}

void Engine::LightSetMaterialColorSpecular(float red, float green, float blue) {
    this->materialSpecular[0] = red;
    this->materialSpecular[1] = green;
    this->materialSpecular[2] = blue;
    this->materialSpecular[3] = this->materialAlpha;

    if (Engine::enableShader == 0) {
        return glMaterialfv(0x408, 0x1202, this->materialSpecular);
    }

    int count = this->lightCount;
    for (int index = 0; index < count; index += 1) {
        const LightColor &src = (&this->lightSpecular)[index];
        Vector &dst = (&this->lightDiffuseShaded)[index] /* specular-shaded[2] @0x2e4 */;
        dst.x = src.r * red;
        dst.y = src.g * green;
        dst.z = src.b * blue;
    }
    return this->ShaderUpdate();
}

void Engine::LightSetGlobalSceneColorAmbient(float red, float green, float blue) {
    this->sceneAmbient[0] = red;
    this->sceneAmbient[1] = green;
    this->sceneAmbient[2] = blue;
    this->sceneAmbient[3] = 1.0f;

    if (Engine::enableShader == 0) {
        return glLightModelfv(0xb53, this->sceneAmbient);
    }

    int count = this->lightCount;
    for (int index = 0; index < count; index += 1) {
        const LightColor &src = (&this->lightAmbient)[index];
        Vector &dst = (&this->lightAmbientShaded)[index];
        dst.x = (src.r + red) * this->materialAmbient[0];
        dst.y = (src.g + green) * this->materialAmbient[1];
        dst.z = (src.b + blue) * this->materialAmbient[2];
    }
    return this->ShaderUpdate();
}

void Engine::SetPostEffect(uint32_t effect, bool enable) {
    if (this->postEffectFBO == 0 && enable) {
        this->postEffectFBO = new FBOContainer(this, String("posteffect"));
        int width;
        int height;
        if (this->appManager->paintCanvas->gameOrientation == 2) {
            width = this->displayWidth;
            height = this->displayHeight;
        } else {
            width = this->displayHeight;
            height = this->displayWidth;
        }
        this->postEffectFBO->Create(width, height, true, false);
    }

    if (effect == kPostEffectBW) {
        if (enable) {
            if (g_Engine_postEffectCounter > 0) {
                g_Engine_postEffectCounter -= 1;
            } else {
                this->postEffectFlags |= 4;
            }
        } else if (g_Engine_postEffectFlag == 1) {
            this->postEffectFlags &= ~4u;
            g_Engine_postEffectFlag = 0;
            g_Engine_postEffectPending = 1;
        } else {
            g_Engine_postEffectFlag = g_Engine_postEffectCounter <= 0;
        }
    } else if (effect == kPostEffectBlur) {
        Engine::EnableGlow = enable;
        if (enable) {
            this->postEffectFlags |= 2;
        } else {
            this->postEffectFlags &= ~2u;
        }
    } else if (effect == kPostEffectBase) {
        if (enable) {
            this->postEffectFlags |= 1;
        } else {
            this->postEffectFlags &= ~1u;
        }
    }
}

void Engine::LightSetMaterialColorDiffuse(float red, float green, float blue) {
    this->materialDiffuse[0] = red;
    this->materialDiffuse[1] = green;
    this->materialDiffuse[2] = blue;
    this->materialDiffuse[3] = this->materialAlpha;

    if (Engine::enableShader == 0) {
        return glMaterialfv(0x408, 0x1201, this->materialDiffuse);
    }

    int lightCount = this->lightCount;
    for (int i = 0; i < lightCount; i += 1) {
        const LightColor &src = (&this->lightDiffuse)[i];
        Vector &dst = (&this->field_0x2fc)[i]; /* diffuse-shaded[2] @0x2fc */
        dst.x = src.r * red;
        dst.y = src.g * green;
        dst.z = src.b * blue;
    }
    return this->ShaderUpdate();
}

void Engine::initFileInterface() {
    FileInterfaceAndroid *fileInterface = new(operator new(0x38)) FileInterfaceAndroid();
    this->fileInterface = fileInterface;
    return AEFile::SetInterface((FileInterface *) fileInterface);
}

void Engine::SetOrthoMatrix(float *projection, float *view, bool multiply) {
    if (Engine::enableShader != 0) {
        float *proj = this->projMatrix;
        proj[0] = projection[0];
        proj[1] = projection[1];
        proj[2] = projection[2];
        proj[3] = projection[3];
        proj[4] = projection[4];
        proj[5] = projection[5];
        proj[6] = projection[6];
        proj[7] = projection[7];
        proj[8] = projection[8];
        proj[9] = projection[9];
        proj[10] = projection[10];
        proj[11] = projection[11];
        proj[12] = projection[12];
        proj[13] = projection[13];
        proj[14] = projection[14];
        proj[15] = projection[15];
        if (multiply) {
            float local[16];
            vst1q_f32(&local[0], vld1q_f32(&view[0]));
            vst1q_f32(&local[4], vld1q_f32(&view[4]));
            vst1q_f32(&local[8], vld1q_f32(&view[8]));
            vst1q_f32(&local[12], vld1q_f32(&view[12]));
            AbyssEngine::esMatrixMultiply((AbyssEngine::ESMatrix *) this->projMatrix,
                                          (AbyssEngine::ESMatrix *) local,
                                          (AbyssEngine::ESMatrix *) this->projMatrix);
        }
    }
}

int Engine::InitGL(bool shaders, int width, int height) {
    this->refractFBO = 0;
    this->displayWidth = width;
    this->displayHeight = height;
    this->viewportWidth = width;
    this->viewportHeight = height;

    FileInterfaceAndroid *fileInterface = new(operator new(0x38)) FileInterfaceAndroid();
    this->fileInterface = fileInterface;
    AEFile::SetInterface((FileInterface *) fileInterface);

    this->lastGlError = 0;
    this->vibrationSupported = 0;
    this->hasVibration = 0;
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
    this->appManager->paintCanvas->Initialize(false);
    this->maxTextureSize = 0;
    glGetIntegerv(0xd33, (GLint *) &this->maxTextureSize);

    if (Engine::enableShader != 0 && g_Engine_supportsFBO != 0) {
        FBOContainer *fbo = new FBOContainer(this, String("refract"));
        this->refractFBO = fbo;
        fbo->Create(this->displayWidth, this->displayHeight, false, true);
    }

    return 1;
}

void Engine::ClearBuffer(uint32_t color) {
    const double scale = 255.0;
    double red = (double) (color >> 24) / scale;
    double green = (double) ((color >> 16) & 0xff) / scale;
    double blue = (double) ((color >> 8) & 0xff) / scale;
    double alpha = (double) (color & 0xff) / scale;
    glClearColor((float) red, (float) green, (float) blue, (float) alpha);
    return glClear(0x4100);
}

void Engine::LightSetLightPosition(float x, float y, float z, unsigned int light) {
    unsigned int index = light - 0x4000;
    if (index < 8) {
        int count = light - 0x3fff;
        int current = this->lightCount;
        if (current > count) {
            count = current;
        }
        this->lightCount = count;

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
    int current = this->lightCount;
    if (current > count) {
        count = current;
    }
    this->lightCount = count;

    LightColor &src = (&this->lightAmbient)[index];
    src.r = red;
    src.g = green;
    src.b = blue;
    src.a = 1.0f;
    if (Engine::enableShader == 0) {
        return glLightfv(light, 0x1200, &src.r);
    }
    float *dst = &(&this->lightAmbientShaded)[index].x;
    dst[0] = (this->sceneAmbient[0] + red) * this->materialAmbient[0];
    dst[1] = (this->sceneAmbient[1] + src.g) * this->materialAmbient[1];
    dst[2] = (this->sceneAmbient[2] + src.b) * this->materialAmbient[2];
    return this->ShaderUpdate();
}

void Engine::ShaderSetInActive() {
    ShaderBaseStruct *shader = this->shaders[currentUsedShaderIndex];
    shader->SetInActive();
}

void Engine::LightSetLightColorDiffuse(float red, float green, float blue, unsigned int light) {
    unsigned int index = light - 0x4000;
    if (index > 7) {
        return;
    }
    int count = light - 0x3fff;
    int current = this->lightCount;
    if (current > count) {
        count = current;
    }
    this->lightCount = count;

    LightColor &src = (&this->lightDiffuse)[index];
    src.r = red;
    src.g = green;
    src.b = blue;
    src.a = 1.0f;
    if (Engine::enableShader == 0) {
        return glLightfv(light, 0x1201, &src.r);
    }
    float *dst = &(&this->field_0x2fc)[index].x; /* diffuse-shaded[2] @0x2fc */
    dst[0] = this->materialDiffuse[0] * red;
    dst[1] = src.g * this->materialDiffuse[1];
    dst[2] = src.b * this->materialDiffuse[2];
    return this->ShaderUpdate();
}

Engine::Engine()
        : lightDir(), field_0x33c(), lightColor(), field_0x3cc(), fogColor(), eyePosition(),
          field_0x468(), field_0x474() {
    this->quadMesh = nullptr;
    this->viewFramebuffer = 0;
    this->postEffectFlags = 0;
    this->field_0x3c4 = 0;
    this->field_0x3c8 = 0;
    this->field_0x3cc = Vector{0.5f, 0.3f, 0.0f};
    this->addData = 0;
    this->postEffectFBO = 0;
    this->refractFBO = 0;
    this->glowActive = 0;
    this->autoPilotEngaged = 0;
    this->field_0x4a8 = 0;
    this->textureByteCounter = 0;
    this->field_0x100 = 0;
    this->currentProgram = -1;
    for (int i = 0; i != 0x14; i += 1) {
        this->boundTextures[i] = -1;
    }
    this->frameBufferTextures[0] = -1;
    this->frameBufferTextures[1] = -1;
    this->field_0xff = 0;
    this->clientStateFlagsAE = 0;
    this->statsEnabled = 0;
    this->field_0xfe = 1;
    this->field_0x78 = -1;
    this->displayWidth = 0;
    this->displayHeight = 0;
    this->viewportWidth = 0;
    this->viewportHeight = 0;
    this->field_0x74 = 0;
    this->glEnableFlags = 0;
    this->lightingEnabled = 0;
    this->field_0x48 = 0;
    this->onDestroyCallback = 0;
    this->rimColor.x = 0;
    this->rimColor.y = 0;
    this->rimColor.z = 0;
    this->field_0x34 = 0;
    this->field_0x38 = 0;
    this->accelRaw[0] = 0;
    this->accelRaw[1] = 0;
    this->accelRaw[2] = 0;
    this->gravRaw[0] = 0;
    this->gravRaw[1] = 0;
    this->gravRaw[2] = 0;
    reinterpret_cast<int &>(this->explosionTimeline) = 0x14;  // raw-int placeholder (see Engine.h)
    this->linearFilterFlag = 1;
    this->field_0xcc = 0.7f;
    this->appManager = new ApplicationManager(this);
    this->fogMinDist = 0;
    this->fogMaxDist = 50000.0f;
    this->glColor[0] = -1.0f;
    this->glColor[1] = -1.0f;
    this->glColor[2] = -1.0f;
    this->glColor[3] = -1.0f;
    this->fogColor = Vector{1.0f, 0.0f, 0.0f};
    this->initFileInterface();
}

void Engine::SetTextures(uint32_t first, uint32_t second) {
    uint32_t count = this->appManager->paintCanvas->cubeTextures.count;
    if (count == 0 || first > count - 1) {
        return;
    }
    uint32_t texture =
            ((TextureEntry *) this->appManager->paintCanvas->cubeTextures.data_[first])->glTexture;
    if ((uint32_t) this->boundTextures[0] != texture) {
        glActiveTexture(0x84c0);
        float env = ((TextureEntry *) this->appManager->paintCanvas->cubeTextures.data_[first])->texEnv;
        if (currentLODBias != env) {
            currentLODBias = env;
            if (Engine::enableShader != 0) {
                if (BiasErrorOutputFlag != 0) {
                    BiasErrorOutputFlag = 0;
                }
            } else {
                glTexEnvf(0x8500, 0x8501, env);
            }
        }
        glBindTexture(0xde1, texture);
        this->boundTextures[0] = texture;
    }

    if (second > this->appManager->paintCanvas->cubeTextures.count - 1) {
        if (this->boundTextures[1] != -1) {
            if (Engine::enableShader == 0) {
                glActiveTexture(0x84c1);
                glDisable(0xde1);
                glActiveTexture(0x84c0);
            }
            this->boundTextures[1] = -1;
        }
        return;
    }

    uint32_t texture2 =
            ((TextureEntry *) this->appManager->paintCanvas->cubeTextures.data_[second])->glTexture;
    if ((uint32_t) this->boundTextures[1] != texture2) {
        glActiveTexture(0x84c1);
        this->GlEnable(0xde1, true);
        unsigned int target;
        if (Engine::enableShader != 0 && (this->glEnableFlags & 0x80008) != 0) {
            target = 0x8513;
        } else {
            target = 0xde1;
        }
        glBindTexture(target, texture2);
        this->boundTextures[1] = texture2;
    }
}

uint32_t Engine::ShaderInit() {
#define MAKE_SHADER(Type, size) do { \
        auto *s = new(operator new(size)) AbyssEngine::Type(); \
        this->ShaderRegister(s); \
    } while(0)
    MAKE_SHADER(TextureShader, 0x8c);
    MAKE_SHADER(SimpleShader, 0x2c);
    MAKE_SHADER(TextureVtxColorShader, 0x94);
    MAKE_SHADER(NoTexShader, 0x2c);
    MAKE_SHADER(NoTexVtxColorShader, 0x30);
    MAKE_SHADER(TextureAlphaTestShader, 0x74);
    MAKE_SHADER(CubeMapping, 0x5c);
    MAKE_SHADER(CubeNormalMapping, 0x68);
    MAKE_SHADER(BumpMapping, 0x44);
    MAKE_SHADER(SandboxShader, 0x50);
    MAKE_SHADER(GenericShader, 0x64);
    MAKE_SHADER(GenericShader1, 0x58);
    MAKE_SHADER(GenericShader2, 0x58);
    MAKE_SHADER(TexOnlyShader, 0x30);
    MAKE_SHADER(BumpShaderV2, 0x60);
    MAKE_SHADER(BumpShaderV3, 0x84);
    MAKE_SHADER(VertexColorShader, 0x5c);
    MAKE_SHADER(VertexColorAlphaTextureShader, 0x5c);
    MAKE_SHADER(PulseShader, 0x5c);
    MAKE_SHADER(BumpShader, 0x84);
    MAKE_SHADER(BumpShaderV4, 0x5c);
    MAKE_SHADER(BloomShader, 0xa0);
    MAKE_SHADER(BlurShader, 0x64);
    MAKE_SHADER(PostBWShader, 0x34);
    MAKE_SHADER(TextureLightShader, 0x70);
    MAKE_SHADER(BumpRimCubeShader, 0x98);
    MAKE_SHADER(BumpShaderCloak, 0x98);
    MAKE_SHADER(SpecCubeMapping, 0x5c);
    MAKE_SHADER(SpecCubeAlphaMapping, 0x5c);
    MAKE_SHADER(EnergyShield, 0x60);
    MAKE_SHADER(SimpleRefractionShader, 0x58);
    MAKE_SHADER(GlowShader, 0x34);
    MAKE_SHADER(GlowPPShader, 0xa8);
    MAKE_SHADER(DrawFBOShader, 0x60);
    MAKE_SHADER(ColorMixAdd, 0x40);
    MAKE_SHADER(BumpShaderParticle, 0x6c);
    MAKE_SHADER(DNSShader, 0x64);
    MAKE_SHADER(BumpShaderRefract, 0x4c);
    MAKE_SHADER(BumpRimCubeShader_new, 0x98);
#undef MAKE_SHADER
    glGetError();
    return 1;
}

void Engine::SetEyePosition(float x, float y, float z) {
    this->eyePosition = Vector{x, y, z};
}

void Engine::SetModelMatrix(const Matrix &matrix) {
    Engine * self = this;
    const float *m = matrix;
    if (Engine::enableShader != 0) {
        self->normalMatrix[0] = m[0];
        self->normalMatrix[1] = m[4];
        self->normalMatrix[2] = m[8];
        self->normalMatrix[3] = m[1];
        self->normalMatrix[4] = m[5];
        self->normalMatrix[5] = m[9];
        self->normalMatrix[6] = m[2];
        self->normalMatrix[7] = m[6];
        self->normalMatrix[8] = m[10];
        float gl[16] = {
            m[0], m[4], m[8], 0.0f,
            m[1], m[5], m[9], 0.0f,
            m[2], m[6], m[10], 0.0f,
            m[3], m[7], m[11], 1.0f,
        };
        memcpy(self->modelMatrixGL, gl, 0x40);
        Vector tmp;
        if (self->lightDirty[0] == 0.0f) {
            tmp = AbyssEngine::AEMath::MatrixInverseRotateVector(
                matrix, self->field_0x468);
            tmp = AbyssEngine::AEMath::VectorNormalize(tmp);
            self->lightDir = tmp;
        } else {
            self->lightDir = self->field_0x468;
        }
        if (self->lightCount > 1) {
            if (self->lightDirty[1] == 0.0f) {
                tmp = AbyssEngine::AEMath::MatrixInverseRotateVector(
                    matrix, self->field_0x474);
                tmp = AbyssEngine::AEMath::VectorNormalize(tmp);
                self->field_0x33c = tmp;
            } else {
                self->field_0x33c = self->field_0x474;
            }
        }
        self->ShaderUpdate();
        tmp = AbyssEngine::AEMath::MatrixInverseTransformVector(
            matrix, self->eyePosition);
        self->lightColor = tmp;
        self->lightColor.x /= m[12];
        self->lightColor.y /= m[13];
        self->lightColor.z /= m[14];
    }
    return;
}

void Engine::LightSetLight(unsigned int light) {
    float position[4] = {0.0f, 1.0f, 0.0f, 0.0f};
    unsigned int index = light - 0x4000;
    if (index < 8) {
        int count = light - 0x3fff;
        int current = this->lightCount;
        if (current > count) {
            count = current;
        }
        this->lightCount = count;

        const Vector &dir = (&this->field_0x468)[index];
        position[0] = dir.x;
        position[1] = dir.y;
        position[2] = dir.z;
        position[3] = this->lightDirty[index];
        if (Engine::enableShader == 0) {
            glLightfv(light, 0x1203, position);
        }
    }
    return;
}

void Engine::SetTexturesExt(uint32_t first, ...) {
    if (this->appManager->paintCanvas->cubeTextures.count != 0) {
        va_list args;
        va_start(args, first);
        uint32_t slot = 0;
        for (uint32_t textureIndex = first; textureIndex != 0xffffffff;
             textureIndex = va_arg(args, uint32_t), slot += 1) {
            if (textureIndex >= this->appManager->paintCanvas->cubeTextures.count) {
                continue;
            }
            uint32_t texture =
                    ((TextureEntry *) this->appManager->paintCanvas->cubeTextures.data_[textureIndex])->glTexture;
            if ((uint32_t) this->boundTextures[slot] == texture) {
                continue;
            }
            glActiveTexture(slot + 0x84c0);
            float env = ((TextureEntry *) this->appManager->paintCanvas->cubeTextures.data_[textureIndex])->texEnv;
            if (currentLODBias != env) {
                currentLODBias = env;
                if (Engine::enableShader != 0) {
                    if (BiasErrorOutputFlag != 0) {
                        BiasErrorOutputFlag = 0;
                    }
                } else {
                    glTexEnvf(0x8500, 0x8501, env);
                }
            }
            bool isCube = ((TextureEntry *) this->appManager->paintCanvas->cubeTextures.data_[textureIndex])->isCube != 0;
            glBindTexture(isCube ? 0x8513 : 0xde1, texture);
            this->boundTextures[slot] = texture;
        }
        va_end(args);
        for (uint32_t i = slot; i < 0x14; i += 1) {
            this->boundTextures[i] = -1;
        }
        glActiveTexture(0x84c0);
    }
}

void Engine::PreUpdate() {
}

void Engine::Release() {
}

void Engine::ActivateFrameBuffer(int slot) {
}

void Engine::SetFrameBufferScaleFactor(float factor, int slot) {
}

void Engine::GrabFrameBuffer() {
}

void *Engine::GetJPEGImageData(float quality) { // lint: void_ptr return type fixed by Engine.h declaration
    // lint: void_ptr return type fixed by Engine.h declaration
    return nullptr;
}

void Engine::SaveImageToPhotosAlbum() {
}

void Engine::SetScreenOrientation(AbyssEngine::LandscapeMode orientation) {
}

bool Engine::IsRefractActivated() {
    return true;
}

void Engine::Vibrate(unsigned short duration) {
}

void Engine::SetWorldViewMatrix(const Matrix &matrix) {
    const float *m = matrix.m;
    if (Engine::enableShader != 0) {
        float gl[16] = {
            m[0], m[4], m[8], 0.0f,
            m[1], m[5], m[9], 0.0f,
            m[2], m[6], m[10], 0.0f,
            m[3], m[7], m[11], 1.0f,
        };
        memcpy(this->worldViewMatrixGL, gl, 0x40);
        AbyssEngine::esMatrixMultiply((AbyssEngine::ESMatrix *) this->worldViewProjMatrix, (AbyssEngine::ESMatrix *) gl, (AbyssEngine::ESMatrix *) this->projMatrix);
    } else {
        MatrixGetGL(matrix, this->uvMatrixGL);
        return glLoadMatrixf(this->uvMatrixGL);
    }
    return;
}

void Engine::ResetLightParam() {
    this->materialAlpha = 1.0f;
    this->lightCount = 1;
    this->materialDiffuse[0] = 0.8f;
    this->materialDiffuse[1] = 0.8f;
    this->materialDiffuse[2] = 0.8f;
    this->materialDiffuse[3] = 0x3f800000;
    this->materialAmbient[0] = 0.2f;
    this->materialAmbient[1] = 0.2f;
    this->materialAmbient[2] = 0.2f;
    this->materialAmbient[3] = 0x3f800000;
    this->materialSpecular[0] = 0;
    this->materialSpecular[2] = 0x3f800000ULL;
    this->materialShininess = 0;
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

    if (Engine::enableShader == 0) {
        glLightfv(0x4000, 0x1200, &this->lightAmbient.r);
        glLightfv(0x4000, 0x1201, &this->lightDiffuse.r);
        glLightfv(0x4000, 0x1202, &this->lightSpecular.r);
        glMaterialfv(0x408, 0x1200, this->materialAmbient);
        glMaterialfv(0x408, 0x1201, this->materialDiffuse);
        glMaterialfv(0x408, 0x1202, this->materialSpecular);
        glMaterialf(0x408, 0x1601, this->materialShininess);
    }
    return;
}

void Engine::LightSetLightColorSpecular(float red, float green, float blue, unsigned int light) {
    unsigned int index = light - 0x4000;
    if (index > 7) {
        return;
    }
    int count = light - 0x3fff;
    int current = this->lightCount;
    if (current > count) {
        count = current;
    }
    this->lightCount = count;

    LightColor &src = (&this->lightSpecular)[index];
    src.r = red;
    src.g = green;
    src.b = blue;
    src.a = 1.0f;
    if (Engine::enableShader == 0) {
        return glLightfv(light, 0x1202, &src.r);
    }
    float *dst = &(&this->lightDiffuseShaded)[index].x; /* specular-shaded[2] @0x2e4 */
    dst[0] = this->materialSpecular[0] * red;
    dst[1] = src.g * this->materialSpecular[1];
    dst[2] = src.b * this->materialSpecular[2];
    return this->ShaderUpdate();
}

void Engine::GlEnable(unsigned int cap, bool enable) {
    // Engine-level capability codes (0x1000000 alpha test, 0x11000xx shader features) are tracked
    // as bits in glEnableFlags when shaders are on; the GLES1 path forwards real GL caps only.
    if (Engine::enableShader != 0) {
        switch (cap) {
            case 0x1100000:
                if (enable) {
                    this->glEnableFlags |= 0x4u;
                } else {
                    this->glEnableFlags &= ~0x4u;
                }
                break;
            case 0x1000000:
                if (enable) {
                    this->glEnableFlags |= 0x2u;
                } else {
                    this->glEnableFlags &= ~0x2u;
                }
                break;
            case 0x1100001:
                if (enable) {
                    this->glEnableFlags |= 0x8u;
                } else {
                    this->glEnableFlags &= ~0x8u;
                }
                break;
            case 0x1100002:
                if (enable) {
                    this->glEnableFlags |= 0x10u;
                } else {
                    this->glEnableFlags &= ~0x10u;
                }
                break;
            case 0x1100003:
                if (enable) {
                    this->glEnableFlags |= 0x20u;
                } else {
                    this->glEnableFlags &= ~0x20u;
                }
                break;
            case 0x1100004:
                if (enable) {
                    this->glEnableFlags |= 0x80u;
                } else {
                    this->glEnableFlags &= ~0x80u;
                }
                break;
            case 0x1100005:
                if (enable) {
                    this->glEnableFlags |= 0x40u;
                } else {
                    this->glEnableFlags &= ~0x40u;
                }
                break;
            case 0x1100006:
                if (enable) {
                    this->glEnableFlags |= 0x100u;
                } else {
                    this->glEnableFlags &= ~0x100u;
                }
                break;
            case 0x1100007:
                if (enable) {
                    this->glEnableFlags |= 0x200u;
                } else {
                    this->glEnableFlags &= ~0x200u;
                }
                break;
            case 0x1100008:
                if (enable) {
                    this->glEnableFlags |= 0x400u;
                } else {
                    this->glEnableFlags &= ~0x400u;
                }
                break;
            case 0x1100009:
                if (enable) {
                    this->glEnableFlags |= 0x1000u;
                } else {
                    this->glEnableFlags &= ~0x1000u;
                }
                break;
            case 0x1100010:
                if (enable) {
                    this->glEnableFlags |= 0x2000u;
                } else {
                    this->glEnableFlags &= ~0x2000u;
                }
                break;
            case 0x1100011:
                if (enable) {
                    this->glEnableFlags |= 0x4000u;
                } else {
                    this->glEnableFlags &= ~0x4000u;
                }
                break;
            case 0x1100012:
                if (enable) {
                    this->glEnableFlags |= 0x8000u;
                } else {
                    this->glEnableFlags &= ~0x8000u;
                }
                break;
            case 0x1100013:
                if (enable) {
                    this->glEnableFlags |= 0x10000u;
                } else {
                    this->glEnableFlags &= ~0x10000u;
                }
                break;
            case 0x1100014:
                if (enable) {
                    this->glEnableFlags |= 0x20000u;
                } else {
                    this->glEnableFlags &= ~0x20000u;
                }
                break;
            case 0x1100015:
                if (enable) {
                    this->glEnableFlags |= 0x40000u;
                } else {
                    this->glEnableFlags &= ~0x40000u;
                }
                break;
            case 0x1100016:
                if (enable) {
                    this->glEnableFlags |= 0x80000u;
                } else {
                    this->glEnableFlags &= ~0x80000u;
                }
                break;
            case 0x1100017:
                if (enable) {
                    this->glEnableFlags |= 0x100000u;
                } else {
                    this->glEnableFlags &= ~0x100000u;
                }
                break;
            case 0x1100018:
                if (enable) {
                    this->glEnableFlags |= 0x200000u;
                } else {
                    this->glEnableFlags &= ~0x200000u;
                }
                break;
            case 0x1100019:
                if (enable) {
                    this->glEnableFlags |= 0x400000u;
                } else {
                    this->glEnableFlags &= ~0x400000u;
                }
                break;
            case 0x1100020:
                if (enable) {
                    this->glEnableFlags |= 0x800000u;
                } else {
                    this->glEnableFlags &= ~0x800000u;
                }
                break;
            case 0x1100021:
                if (enable) {
                    this->glEnableFlags |= 0x1000000u;
                } else {
                    this->glEnableFlags &= ~0x1000000u;
                }
                break;
            case 0x1100022:
                if (enable) {
                    this->glEnableFlags |= 0x2000000u;
                } else {
                    this->glEnableFlags &= ~0x2000000u;
                }
                break;
            case 0x1100023:
                if (enable) {
                    this->glEnableFlags |= 0x4000000u;
                } else {
                    this->glEnableFlags &= ~0x4000000u;
                }
                break;
            case 0x1100024:
                if (enable) {
                    this->glEnableFlags |= 0x8000000u;
                } else {
                    this->glEnableFlags &= ~0x8000000u;
                }
                break;
            case 0xde1:
                if (enable) {
                    this->glEnableFlags |= 0x1u;
                } else {
                    this->glEnableFlags &= ~0x1u;
                }
                break;
        }
        return;
    }
    if ((cap >> 20) > 16) {
        return;
    }
    if (cap == 0x1000000) {
        cap = 0xbc0;
    }
    if (enable) {
        return glEnable(cap);
    }
    return glDisable(cap);
}

void Engine::LightEnable(bool enabled) {
    if (this->lightingEnabled == enabled) return;
    this->lightingEnabled = enabled;
    if (!enabled) {
        this->LightSetMaterialColorAlpha(1.0f);
    }
    if (Engine::enableShader) return;
    if (enabled) {
        glEnable(0xb50);  // GL_LIGHTING
        glEnable(0x4000); // GL_LIGHT0
    } else {
        glDisable(0x4000); // GL_LIGHT0
        glDisable(0xb50);  // GL_LIGHTING
    }
}

void Engine::LightSetMaterialColorAmbient(float red, float green, float blue) {
    this->materialAmbient[0] = red;
    this->materialAmbient[1] = green;
    this->materialAmbient[2] = blue;
    this->materialAmbient[3] = this->materialAlpha;

    if (Engine::enableShader == 0) {
        return glMaterialfv(0x408, 0x1200, this->materialAmbient);
    }

    int count = this->lightCount;
    for (int index = 0; index < count; index += 1) {
        const LightColor &src = (&this->lightAmbient)[index];
        Vector &dst = (&this->lightAmbientShaded)[index];
        dst.x = (this->sceneAmbient[0] + src.r) * red;
        dst.y = (this->sceneAmbient[1] + src.g) * green;
        dst.z = (this->sceneAmbient[2] + src.b) * blue;
    }
    return this->ShaderUpdate();
}

unsigned char AbyssEngine::Engine::EnableGlow;
int AbyssEngine::Engine::ImageCount;
int AbyssEngine::Engine::switchGlow;
int AbyssEngine::Engine::SwapCounter;
int AbyssEngine::Engine::switchBloom;
unsigned char AbyssEngine::Engine::enableShader;
unsigned char AbyssEngine::Engine::EnableRefract;
float AbyssEngine::Engine::LodDistShader;
unsigned char AbyssEngine::Engine::DisableRefract;
float AbyssEngine::Engine::AnisotropyValue;
unsigned char AbyssEngine::Engine::KeepRawMeshData;
unsigned char AbyssEngine::Engine::backupSaveGames;
int AbyssEngine::Engine::MultiSampleValue;
unsigned char AbyssEngine::Engine::UseAdvancedShader;
unsigned char AbyssEngine::Engine::enableReverseFlag;
int AbyssEngine::Engine::tv_h;
int AbyssEngine::Engine::tv_w;
AbyssEngine::String AbyssEngine::Engine::vendor;
unsigned char AbyssEngine::Engine::DrawFBO;
int AbyssEngine::Engine::vboSize;
unsigned char AbyssEngine::Engine::DEBUGKEY;
unsigned char AbyssEngine::Engine::TVEnable;
float AbyssEngine::Engine::farPlane;
AbyssEngine::String AbyssEngine::Engine::renderer;
AbyssEngine::AEMath::Vector AbyssEngine::Engine::tv_modes;
unsigned char AbyssEngine::Engine::WireFrame;
float AbyssEngine::Engine::nearPlane;
