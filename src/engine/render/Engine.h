#ifndef GOF2_ENGINE_H
#define GOF2_ENGINE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Cross-engine types touched by pointer only. The real definitions live in
// namespace AbyssEngine (FBOContainer.h / ShaderBaseStruct.h / Mesh.h); a value
// or base use would #include the owning header instead.
namespace AbyssEngine {
class FBOContainer;
class ShaderBaseStruct;
class Mesh;
} // namespace AbyssEngine

// Game-side singletons referenced by pointer.
namespace AbyssEngine { class ApplicationManager; }
using ::AbyssEngine::ApplicationManager;

// Engine is the renderer/device root. The real definition lives in
// namespace AbyssEngine (to match the binary's mangling N11AbyssEngine6EngineE);
// a global `using` alias below lets bare `Engine` resolve to AbyssEngine::Engine.
namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;

// MeshFull is the complete in-engine mesh type used by the render entry points.
typedef AbyssEngine::Mesh MeshFull;

// Engine lifecycle callbacks (function-pointer typedefs).
typedef void DestroyCallback(Engine *);
typedef void InitializeCallback(Engine *);

// Poll-and-clear the GLES error flag. A thin debug helper used after a batch of
// GL calls; the result is discarded here (callers that need the code query
// glGetError directly).
void glError();

// Per-light fixed-function color slot (GL light color: rgba). The engine keeps up
// to 8 of these contiguously (one per GL light) and indexes them by (light - GL_LIGHT0).
struct LightColor {
    float r;
    float g;
    float b;
    float a;
};

namespace AbyssEngine {

// Screen/game orientation, shared with PaintCanvas. Guarded so a TU that already
// pulls a definer (PaintCanvas.h / PaintCanvasClass.h) does not get a redefinition.
#ifndef GOF2_ENUM_LandscapeMode
#define GOF2_ENUM_LandscapeMode
enum LandscapeMode { LandscapeMode_dummy = 0, LandscapeMode_1 = 1, LandscapeMode_2 = 2, LandscapeMode_3 = 3 };
#endif

class Engine {
public:
    uint32_t deviceWidth;               // +0x0 device width (NFC::getWidth())
    uint32_t deviceHeight;              // +0x4 device height (NFC::getHeight())
    uint8_t isPad;                      // +0x8 NFC::isPad()
    uint32_t depthBits;                 // +0xc GL_DEPTH_BITS query result
    uint32_t field_0x10;
    String str_0x14;                    // embedded String (auto ctor/dtor)
    uint8_t field_0x20;
    void* fileInterface;                // +0x24 FileInterfaceAndroid* (AEFile backend)
    uint8_t field_0x25;
    int field_0x28;
    bool vibrationSupported;            // +0x2c device vibration capability (HasVibration)
    // +0x30 owned ApplicationManager. External TUs read this as a raw char** to reach the
    // PaintCanvas at *appManager, so the field_0x30 alias is preserved for them.
    union { ApplicationManager* appManager; char** field_0x30; };
    uint32_t field_0x34;
    String str_0x3c;                    // embedded String (auto ctor/dtor)
    uint8_t field_0x40;
    String str_0x4c;                    // embedded String (auto ctor/dtor)
    uint64_t field_0x58;
    uint64_t field_0x68;
    uint32_t field_0x70;
    bool field_0x74;
    int field_0x78;
    int boundTextures[20];              // per-slot bound GL texture id (slot 0..19)
    float field_0xcc;                   // scalar bound by the cube shaders (u_*)
    // +0xd0 current draw color (rgba), set by SetColor; bound to glColor / material diffuse.
    union { float glColor[4]; struct { float field_0xd0, field_0xd4, field_0xd8, field_0xdc; }; };
    int packedColor;                    // +0xe0 packed RGBA from SetColor (A | G<<16 | R<<24 | B<<8)
    unsigned char field_0xfc;
    uint16_t field_0xfd;
    uint32_t field_0x100;
    float worldViewProjMatrix[16];      // world*view*proj (esMatrixMultiply target)
    float modelMatrixGL[16];            // GL column-major model matrix
    float worldViewMatrixGL[16];        // GL column-major world-view matrix
    float uvMatrix[16];                 // texture/UV matrix (shader path)
    // +0x204 normal (rotation 3x3) matrix, rows packed from the model matrix; bound as u_NormalMatrix.
    union { float normalMatrix[9]; struct { uint32_t field_0x204, field_0x208, field_0x20c,
                                                     field_0x210, field_0x214, field_0x218,
                                                     field_0x21c, field_0x220, field_0x224; }; };
    LightColor lightDiffuse;            // per-light diffuse color (rgba, stride 0x10)
    LightColor lightSpecular;           // per-light specular color (rgba, stride 0x10)
    LightColor lightAmbient;            // per-light ambient color (rgba, stride 0x10)
    // +0x288 global scene ambient color (rgba). Bound by LightSetGlobalSceneColorAmbient
    // to GL_LIGHT_MODEL_AMBIENT / the lighting shaders.
    union { float sceneAmbient[4]; struct { float field_0x288, field_0x28c, field_0x290; uint32_t field_0x294; }; };
    // Fixed-function material colors (rgba). Bound by Light*Material* and the lighting shaders.
    union { float materialDiffuse[4];  struct { float field_0x298, field_0x29c, field_0x2a0; uint32_t field_0x2a4; }; };  // GL_DIFFUSE
    union { float materialAmbient[4];  struct { float field_0x2a8, field_0x2ac, field_0x2b0; uint32_t field_0x2b4; }; };  // GL_AMBIENT
    union { float materialSpecular[4]; struct { float field_0x2b8, field_0x2bc, field_0x2c0; uint32_t field_0x2c4; }; };  // GL_SPECULAR
    union { float materialShininess;   float field_0x2c8; };  // GL_SHININESS
    Vector lightAmbientShaded;          // per-light shaded ambient (rgb, stride 0x0c)
    Vector lightSpecularShaded;         // per-light shaded specular (rgb, stride 0x0c)
    Vector lightDiffuseShaded;          // per-light shaded diffuse (rgb, stride 0x0c)
    Vector particleAmbient;             // particle ambient color (rgb)
    // Contiguous with the shaded-light block above: Engine indexes (&lightAmbientShaded)[i]
    // across these, and the lighting shaders bind them by offset.
    Vector field_0x2fc;
    Vector field_0x308;
    Vector field_0x314;
    union { Vector rimColor; struct { float field_0x320, field_0x324, field_0x328; }; };  // +0x320 rim-light color (LightSetRimColor)
    union { int lightCount; int field_0x32c; };  // +0x32c active light count
    union { Vector lightDir; Vector field_0x330; };   // scene light direction (rgb-as-xyz)
    Vector field_0x33c;
    uint64_t field_0x340;
    union { int lineVertexBase; int field_0x348; };  // +0x348 base vertex index for DrawLine2D
    union { Vector lightColor; struct { float field_0x34c, field_0x350, field_0x354; }; };  // scene light color
    void* addData;                      // +0x358 extra per-frame data (SetAddData)
    int addDataSize;                    // +0x35c size of addData
    uint32_t field_0x360;
    int displayWidth;                   // +0x368 GL display width (GetDisplayWidth)
    int displayHeight;                  // +0x36c GL display height (GetDisplayHeight)
    int viewportWidth;                  // +0x370
    int viewportHeight;                 // +0x374
    float lightDirty[2];                // per-light "direction is in eye space" flag
    // +0x380 shared quad mesh used by DrawQuad and the post-process shaders. External TUs
    // walk it as a raw char* (+4 = positions, +8 = uvs), so the field_0x380 alias stays.
    union { MeshFull* quadMesh; char* field_0x380; };
    float projMatrix[16];               // projection / ortho matrix (shader path)
    uint8_t field_0x3c4;
    uint32_t field_0x3c8;
    uint64_t field_0x3cc;
    uint32_t field_0x3d4;
    Array<int>* triangleCounts;         // per-shader triangle counters
    union { int currentProgram; int field_0x3e4; };  // +0x3e4 currently-bound GL program (ShaderSetActive)
    union { float fogMinDist; uint32_t field_0x3e8; };  // u_fogMinDist
    union { float fogMaxDist; uint32_t field_0x3ec; };  // u_fogMaxDist
    union { Vector fogColor; Vector field_0x3f0; };     // u_fogColor (rgb)
    Vector eyePosition;                 // eye/camera position (SetEyePosition)
    uint64_t field_0x400;
    union { uint32_t viewFramebuffer; uint32_t field_0x40c; };  // +0x40c default GL framebuffer (view buffer)
    uint32_t postEffectFlags;           // +0x410 active post-effect flags
    AbyssEngine::FBOContainer* postEffectFBO;   // +0x414 post-effect render target
    AbyssEngine::FBOContainer* refractFBO;      // +0x418 refraction render target
    uint8_t glowActive;                 // +0x41c glow pass currently active
    uint32_t glEnableFlags;             // +0x420 GlEnable() shader-path capability bits
    uint8_t lightingEnabled;            // +0x424 fixed-function lighting enabled
    float uvMatrixGL[16];               // fixed-function UV matrix (glLoadMatrixf)
    Vector field_0x468;                 // light[0] direction (light array base)
    Vector field_0x474;                 // light[1] direction
    uint64_t field_0x478;
    bool hasVibration;                  // +0x480 device supports vibration (HasVibration)
    DestroyCallback* onDestroyCallback; // +0x484 app-destroy callback (SetOnDestroyApp)
    float materialAlpha;                // +0x488 current material alpha (LightSetMaterialColorAlpha)
    int frameBufferTextures[2];         // FBO color-texture id per slot
    uint32_t clientStateFlagsAE;        // +0x4a4 AEClientState() enable bits
    uint32_t field_0x4a8;
    // +0x4b0 accelerometer: raw x/y/z (SetAccelValue) then orientation-corrected x/y/z (GetAccelValue).
    union { struct { double accelRaw[3]; volatile double accelValue[3]; };
            struct { double field_0x4b0, field_0x4b8; volatile double field_0x4c0, field_0x4c8, field_0x4d0; double field_0x4d8; }; };
    // +0x4e0 gravity: raw x/y/z (SetGravValue) then orientation-corrected x/y/z (GetGravValue).
    union { struct { double gravRaw[3]; volatile double gravValue[3]; };
            struct { double field_0x4e0, field_0x4e8; volatile double field_0x4f0, field_0x4f8, field_0x500; double field_0x508; }; };
    Array<AbyssEngine::ShaderBaseStruct*>* shaders;  // registered shaders

    Engine();
    ~Engine();

    // ---- render / device methods ----
    void AEClientState(unsigned int state, bool enable);
    void ClearBuffer(uint32_t color);
    bool IsExtensionSupported(const char *extension);
    void ReleaseGL();
    void ActivateRefractFBO();
    void ActivateRender2FracFBO();
    void ActivateRender2TextureFBO();
    void ActivateTextureFBO();
    void ActivateViewBuffer();
    void AfterGLInit();
    void CopyFBO();
    void DeactivateRender2FracFBO();
    void DeactivateRender2TextureFBO();
    void DoPostEffect();
    void DrawCloakFBO(AbyssEngine::FBOContainer *fbo);
    void DrawLine2D(float *verts, int count, bool strip);
    void DrawQuad(int x, int y, int width, int height);
    double * GetAccelValue();
    void GetDeviceInfo();
    uint32_t GetDisplayHeight();
    uint32_t GetDisplayWidth();
    double * GetGravValue();
    void GlEnable(unsigned int cap, bool enable);
    void GlowBeginGlow(int depthFunc);
    void GlowEnableGlow();
    void GlowEndGlow();
    bool HasVibration();
    int InitGL(bool shaders, int width, int height);
    void Initialize(InitializeCallback *callback);
    bool IsPostEffectActivated();
    void LightEnable(bool enabled);
    void LightSetGlobalSceneColorAmbient(float red, float green, float blue);
    void LightSetLight(unsigned int light);
    void LightSetLightColorAmbient(float red, float green, float blue, unsigned int light);
    void LightSetLightColorDiffuse(float red, float green, float blue, unsigned int light);
    void LightSetLightColorSpecular(float red, float green, float blue, unsigned int light);
    void LightSetLightCount(int count);
    void LightSetLightDirection(float x, float y, float z, unsigned int light);
    void LightSetLightPosition(float x, float y, float z, unsigned int light);
    void LightSetMaterialColorAlpha(float alpha);
    void LightSetMaterialColorAmbient(float red, float green, float blue);
    void LightSetMaterialColorDiffuse(float red, float green, float blue);
    void LightSetMaterialColorShininess(float shininess);
    void LightSetMaterialColorSpecular(float red, float green, float blue);
    void LightSetParticleAmbient(float red, float green, float blue);
    void LightSetRimColor(float red, float green, float blue);
    void ReloadShaders();
    void RenderMesh(MeshFull *mesh);
    void ResetLightParam();
    void ResetUVMatrix();
    uint32_t Resume();
    void SetAccelValue(double x, double y, double z);
    void SetAddData(void *data, int size);
    void SetColor(float red, float green, float blue, float alpha);
    uint64_t SetEyePosition(float x, float y, float z);
    void SetFrameBufferTexture(int slot0, int slot1);
    void SetGravValue(double x, double y, double z);
    void SetModelMatrix(const AbyssEngine::AEMath::Matrix &matrix);
    void SetOnDestroyApp(DestroyCallback *callback);
    void SetOrthoMatrix(float *projection, float *view, bool multiply);
    void SetPerspMatrix(float *matrix);
    void SetPostEffect(uint32_t effect, bool enable);
    void SetTextureSlot(uint32_t textureIndex, uint32_t slot);
    void SetTextures(uint32_t first, uint32_t second);
    void SetTexturesExt(uint32_t first, ...);
    // ---- platform / screenshot / device stubs (no-ops on the Android port) ----
    void PreUpdate();
    void Release();
    void ActivateFrameBuffer(int slot);
    void SetFrameBufferScaleFactor(float factor, int slot);
    void GrabFrameBuffer();
    void* GetJPEGImageData(float quality);
    void SaveImageToPhotosAlbum();
    void SetScreenOrientation(AbyssEngine::LandscapeMode orientation);
    bool IsRefractActivated();
    void Vibrate(unsigned short duration);
    void SetUVMatrix(const AbyssEngine::AEMath::Matrix &matrix);
    void SetWorldViewMatrix(const AbyssEngine::AEMath::Matrix &matrix);
    uint32_t ShaderInit();
    void ShaderRegister(AbyssEngine::ShaderBaseStruct *shader);
    void ShaderSetActive(int shaderIndex, MeshFull *mesh);
    void ShaderSetInActive();
    void ShaderUpdate();
    uint32_t Suspend();
    void SwapBuffer();
    void initFileInterface();
};

} // namespace AbyssEngine

using ::AbyssEngine::Engine;

extern Engine* gEngine;            // canonical Engine singleton (binary .bss 0x2281e8)

#endif
