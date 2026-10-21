

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
float AbyssEngine::Engine::lodBiasDiffuse = -1.0f;
float AbyssEngine::Engine::lodBiasNormal = 0.0f;
unsigned int AbyssEngine::Engine::countryCode = 0xffffffff;
bool AbyssEngine::Engine::EnablePostEffect = true;
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


    // Post-effect ids (0x1400000 base) and the black/white effect's switch-off bookkeeping.
    const uint32_t kPostEffectBase = 0x1400000;
    const uint32_t kPostEffectBlur = 0x1400001;
    const uint32_t kPostEffectBW = 0x1400002;
    unsigned char g_Engine_postEffectFlag;
    int g_Engine_postEffectCounter = 1;

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
    this->rimColor[0] = red;
    this->rimColor[1] = green;
    this->rimColor[2] = blue;
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

void Engine::GlowEndGlow() {
    if (Engine::EnableGlow == 0) {
        return;
    }
    this->glowActive = 0;
    glColorMask(1, 1, 1, 1);
    return glDepthFunc(0x201);
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

void Engine::LightSetParticleAmbient(float red, float green, float blue) {
    this->particleAmbientColor[0] = red;  // particle ambient @0x314
    this->particleAmbientColor[1] = green;
    this->particleAmbientColor[2] = blue;
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
        } else if ((flags & 0x1) != 0) {
            if ((mesh->vertexFormat & 0xa) == 8) {
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
        } else {
            this->ShaderSetActive(NoTexShader::ShaderIndex, mesh);
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
        if (this->field_0xfe != 0) {
            if (this->statsEnabled != 0) {
                indexedTriangles[1] += mesh->indexCount / 3;
                this->drawCallCountB += 1;
            } else {
                indexedTriangles[0] += mesh->indexCount / 3;
                this->drawCallCountA += 1;
            }
        }
        glDrawElements(4, mesh->indexCount, 0x1403, mesh->indices);
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
    if (((TextureEntry *) this->appManager->paintCanvas->cubeTextures.data_[textureIndex])->isCube != 0) {
        glBindTexture(0x8513, texture);
    } else {
        glBindTexture(0xde1, texture);
    }
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
    FBOContainer *source = this->postEffectFBO;
    FBOContainer *target = this->refractFBO;
    if (Engine::enableShader != 0) {
        // Each active pass renders source -> target; when another pass follows, the two FBOs swap.
        if ((flags & 2) != 0) {
            ShaderBaseStruct *shader = this->shaders[GlowPPShader::ShaderIndex];
            flags &= ~2u;
            if (flags != 0) {
                shader->RenderEffect(source, target, this);
                FBOContainer *swap = source;
                source = target;
                target = swap;
            } else {
                shader->RenderEffect(source, this);
                flags = 0;
            }
        }
        if ((this->postEffectFlags & 1) != 0) {
            ShaderBaseStruct *shader = this->shaders[BloomShader::ShaderIndex];
            flags &= ~1u;
            if (flags != 0) {
                shader->RenderEffect(source, target, this);
                FBOContainer *swap = source;
                source = target;
                target = swap;
            } else {
                shader->RenderEffect(source, this);
                flags = 0;
            }
        }
        if ((this->postEffectFlags & 4) != 0) {
            ShaderBaseStruct *shader = this->shaders[BlurShader::ShaderIndex];
            if ((flags & ~4u) != 0) {
                shader->RenderEffect(source, target, this, this->blurStrength, this->field_0x3cc);
                FBOContainer *swap = source;
                source = target;
                target = swap;
            } else {
                shader->RenderEffect(source, this, this->blurStrength, this->field_0x3cc);
            }
        }
        if (g_Engine_postEffectFlag == 1) {
            this->SetPostEffect(kPostEffectBW, false);
        }
    }
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
        this->specularShaded[index][0] = src.r * red;
        this->specularShaded[index][1] = src.g * green;
        this->specularShaded[index][2] = src.b * blue;
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
        this->ambientShaded[index][0] = (src.r + red) * this->materialAmbient[0];
        this->ambientShaded[index][1] = (src.g + green) * this->materialAmbient[1];
        this->ambientShaded[index][2] = (src.b + blue) * this->materialAmbient[2];
    }
    return this->ShaderUpdate();
}

void Engine::SetPostEffect(uint32_t effect, bool enable) {
    if (this->postEffectFBO == 0 && enable) {
        this->postEffectFBO = new FBOContainer(this, String("render2TextureFBO"));
        if (this->appManager->paintCanvas->gameOrientation == 2) {
            this->postEffectFBO->Create(this->displayWidth, this->displayHeight, true, false);
        } else {
            this->postEffectFBO->Create(this->displayHeight, this->displayWidth, true, false);
        }
    }

    if (effect == kPostEffectBase) {
        if (enable) {
            this->postEffectFlags |= 1;
        } else {
            this->postEffectFlags &= ~1u;
        }
    } else if (effect == kPostEffectBlur) {
        Engine::EnableGlow = enable;
        if (enable) {
            this->postEffectFlags |= 2;
        } else {
            this->postEffectFlags &= ~2u;
        }
    } else if (effect == kPostEffectBW) {
        if (enable) {
            if (g_Engine_postEffectCounter <= 0) {
                this->postEffectFlags |= 4;
            } else {
                g_Engine_postEffectCounter -= 1;
            }
        } else if (g_Engine_postEffectFlag == 1) {
            this->postEffectFlags &= ~4u;
            g_Engine_postEffectFlag = 0;
            g_Engine_postEffectCounter = 1;
        } else {
            g_Engine_postEffectFlag = g_Engine_postEffectCounter <= 0;
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
        this->diffuseShaded[i][0] = src.r * red;
        this->diffuseShaded[i][1] = src.g * green;
        this->diffuseShaded[i][2] = src.b * blue;
    }
    return this->ShaderUpdate();
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
    float *dst = this->ambientShaded[index];
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
    float *dst = this->diffuseShaded[index]; /* diffuse-shaded[2] @0x2fc */
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
    this->blurStrength = 0;
    this->field_0x3cc = Vector{0.5f, 0.3f, 0.0f};
    this->addData = 0;
    this->postEffectFBO = 0;
    this->refractFBO = 0;
    this->glowActive = 0;
    this->wheelDelta = 0.0f;
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
    this->rimColor[0] = 0;
    this->rimColor[1] = 0;
    this->rimColor[2] = 0;
    this->field_0x34 = 0;
    this->field_0x38 = 0;
    this->accelRaw[0] = 0;
    this->accelRaw[1] = 0;
    this->accelRaw[2] = 0;
    this->gravRaw[0] = 0;
    this->gravRaw[1] = 0;
    this->gravRaw[2] = 0;
    this->timerInterval = 20;
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
    this->ShaderRegister(new TextureShader());
    this->ShaderRegister(new SimpleShader());
    this->ShaderRegister(new TextureVtxColorShader());
    this->ShaderRegister(new NoTexShader());
    this->ShaderRegister(new NoTexVtxColorShader());
    this->ShaderRegister(new TextureAlphaTestShader());
    this->ShaderRegister(new CubeMapping());
    this->ShaderRegister(new CubeNormalMapping());
    this->ShaderRegister(new BumpMapping());
    this->ShaderRegister(new SandboxShader());
    this->ShaderRegister(new GenericShader());
    this->ShaderRegister(new GenericShader1());
    this->ShaderRegister(new GenericShader2());
    this->ShaderRegister(new TexOnlyShader());
    this->ShaderRegister(new BumpShaderV2());
    this->ShaderRegister(new BumpShaderV3());
    this->ShaderRegister(new VertexColorShader());
    this->ShaderRegister(new VertexColorAlphaTextureShader());
    this->ShaderRegister(new PulseShader());
    this->ShaderRegister(new BumpShader());
    this->ShaderRegister(new BumpShaderV4());
    this->ShaderRegister(new BloomShader());
    this->ShaderRegister(new BlurShader());
    this->ShaderRegister(new PostBWShader());
    this->ShaderRegister(new TextureLightShader());
    this->ShaderRegister(new BumpRimCubeShader());
    this->ShaderRegister(new BumpShaderCloak());
    this->ShaderRegister(new SpecCubeMapping());
    this->ShaderRegister(new SpecCubeAlphaMapping());
    this->ShaderRegister(new EnergyShield());
    this->ShaderRegister(new SimpleRefractionShader());
    this->ShaderRegister(new GlowShader());
    this->ShaderRegister(new GlowPPShader());
    this->ShaderRegister(new DrawFBOShader());
    this->ShaderRegister(new ColorMixAdd());
    this->ShaderRegister(new BumpShaderParticle());
    this->ShaderRegister(new DNSShader());
    this->ShaderRegister(new BumpShaderRefract());
    this->ShaderRegister(new BumpRimCubeShader_new());
    glGetError();
    return 1;
}

void Engine::SetEyePosition(float x, float y, float z) {
    this->eyePosition = Vector{x, y, z};
}

void Engine::SetModelMatrix(const Matrix &matrix) {
    if (Engine::enableShader != 0) {
        const float *m = matrix.m;
        this->normalMatrix[0] = m[0];
        this->normalMatrix[1] = m[4];
        this->normalMatrix[2] = m[8];
        this->normalMatrix[3] = m[1];
        this->normalMatrix[4] = m[5];
        this->normalMatrix[5] = m[9];
        this->normalMatrix[6] = m[2];
        this->normalMatrix[7] = m[6];
        this->normalMatrix[8] = m[10];
        float *gl = this->modelMatrixGL;
        gl[0] = m[0];
        gl[1] = m[4];
        gl[2] = m[8];
        gl[3] = 0.0f;
        gl[4] = m[1];
        gl[5] = m[5];
        gl[6] = m[9];
        gl[7] = 0.0f;
        gl[8] = m[2];
        gl[9] = m[6];
        gl[10] = m[10];
        gl[11] = 0.0f;
        gl[12] = m[3];
        gl[13] = m[7];
        gl[14] = m[11];
        gl[15] = 1.0f;
        if (this->lightDirty[0] == 0.0f) {
            this->lightDir = AbyssEngine::AEMath::VectorNormalize(
                    AbyssEngine::AEMath::MatrixInverseRotateVector(matrix, this->field_0x468));
        } else {
            this->lightDir = this->field_0x468;
        }
        if (this->lightCount > 1) {
            if (this->lightDirty[1] == 0.0f) {
                this->field_0x33c = AbyssEngine::AEMath::VectorNormalize(
                        AbyssEngine::AEMath::MatrixInverseRotateVector(matrix, this->field_0x474));
            } else {
                this->field_0x33c = this->field_0x474;
            }
        }
        this->ShaderUpdate();
        this->lightColor = AbyssEngine::AEMath::MatrixInverseTransformVector(matrix, this->eyePosition);
        this->lightColor.x /= m[12];
        this->lightColor.y /= m[13];
        this->lightColor.z /= m[14];
    }
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
            if (((TextureEntry *) this->appManager->paintCanvas->cubeTextures.data_[textureIndex])->isCube != 0) {
                glBindTexture(0x8513, texture);
            } else {
                glBindTexture(0xde1, texture);
            }
            this->boundTextures[slot] = texture;
        }
        va_end(args);
        for (uint32_t i = slot; i < 0x14; i += 1) {
            this->boundTextures[i] = -1;
        }
        glActiveTexture(0x84c0);
    }
}

void Engine::Release() {
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
    this->materialDiffuse[3] = 1.0f;
    this->materialAmbient[0] = 0.2f;
    this->materialAmbient[1] = 0.2f;
    this->materialAmbient[2] = 0.2f;
    this->materialAmbient[3] = 1.0f;
    this->materialSpecular[0] = 0.0f;
    this->materialSpecular[1] = 0.0f;
    this->materialSpecular[2] = 0.0f;
    this->materialSpecular[3] = 1.0f;
    this->materialShininess = 0.0f;

    this->lightAmbient.r = 0.0f;
    this->lightAmbient.g = 0.0f;
    this->lightAmbient.b = 0.0f;
    this->lightAmbient.a = 1.0f;
    this->lightDiffuse.r = 1.0f;
    this->lightDiffuse.g = 1.0f;
    this->lightDiffuse.b = 1.0f;
    this->lightDiffuse.a = 1.0f;
    this->lightSpecular.r = 1.0f;
    this->lightSpecular.g = 1.0f;
    this->lightSpecular.b = 1.0f;
    this->lightSpecular.a = 1.0f;
    this->lightAmbient1.r = 0.0f;
    this->lightAmbient1.g = 0.0f;
    this->lightAmbient1.b = 0.0f;
    this->lightAmbient1.a = 1.0f;
    this->lightDiffuse1.r = 0.0f;
    this->lightDiffuse1.g = 0.0f;
    this->lightDiffuse1.b = 0.0f;
    this->lightDiffuse1.a = 1.0f;
    this->lightSpecular1.r = 0.0f;
    this->lightSpecular1.g = 0.0f;
    this->lightSpecular1.b = 0.0f;
    this->lightSpecular1.a = 1.0f;
    this->sceneAmbient[0] = 0.0f;
    this->sceneAmbient[1] = 0.0f;
    this->sceneAmbient[2] = 0.0f;
    this->sceneAmbient[3] = 1.0f;
    this->ambientShaded[0][0] = 0.0f;
    this->ambientShaded[0][1] = 0.0f;
    this->ambientShaded[0][2] = 0.0f;
    this->ambientShaded[1][0] = 0.0f;
    this->ambientShaded[1][1] = 0.0f;
    this->ambientShaded[1][2] = 0.0f;
    this->specularShaded[0][0] = 0.0f;
    this->specularShaded[0][1] = 0.0f;
    this->specularShaded[0][2] = 0.0f;
    this->specularShaded[1][0] = 0.0f;
    this->specularShaded[1][1] = 0.0f;
    this->specularShaded[1][2] = 0.0f;
    this->diffuseShaded[0][0] = 0.8f;
    this->diffuseShaded[0][1] = 0.8f;
    this->diffuseShaded[0][2] = 0.8f;
    this->diffuseShaded[1][0] = 0.0f;
    this->diffuseShaded[1][1] = 0.0f;
    this->diffuseShaded[1][2] = 0.0f;
    this->particleAmbientColor[0] = 0.0f;
    this->particleAmbientColor[1] = 0.0f;
    this->particleAmbientColor[2] = 0.0f;

    if (Engine::enableShader == 0) {
        glLightfv(0x4000, 0x1200, &this->lightAmbient.r);
        glLightfv(0x4000, 0x1201, &this->lightDiffuse.r);
        glLightfv(0x4000, 0x1202, &this->lightSpecular.r);
        glMaterialfv(0x408, 0x1200, this->materialAmbient);
        glMaterialfv(0x408, 0x1201, this->materialDiffuse);
        glMaterialfv(0x408, 0x1202, this->materialSpecular);
        glMaterialf(0x408, 0x1601, this->materialShininess);
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
    float *dst = this->specularShaded[index]; /* specular-shaded[2] @0x2e4 */
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
        this->ambientShaded[index][0] = (this->sceneAmbient[0] + src.r) * red;
        this->ambientShaded[index][1] = (this->sceneAmbient[1] + src.g) * green;
        this->ambientShaded[index][2] = (this->sceneAmbient[2] + src.b) * blue;
    }
    return this->ShaderUpdate();
}

unsigned char AbyssEngine::Engine::EnableGlow;
int AbyssEngine::Engine::ImageCount;
int AbyssEngine::Engine::switchGlow = 3;
int AbyssEngine::Engine::SwapCounter;
int AbyssEngine::Engine::switchBloom = 3;
unsigned char AbyssEngine::Engine::enableShader;
unsigned char AbyssEngine::Engine::EnableRefract;
float AbyssEngine::Engine::LodDistShader = 5000.0f;
unsigned char AbyssEngine::Engine::DisableRefract;
float AbyssEngine::Engine::AnisotropyValue;
unsigned char AbyssEngine::Engine::KeepRawMeshData;
unsigned char AbyssEngine::Engine::backupSaveGames;
int AbyssEngine::Engine::MultiSampleValue;
unsigned char AbyssEngine::Engine::UseAdvancedShader = 1;
unsigned char AbyssEngine::Engine::enableReverseFlag = 1;
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
