#ifndef GOF2_ENGINE_H
#define GOF2_ENGINE_H
#include "gof2/common.h"

// Cross-engine types touched by pointer only. The real definitions live in
// namespace AbyssEngine (FBOContainer.h / ShaderBaseStruct.h / Mesh.h); a value
// or base use would #include the owning header instead.
namespace AbyssEngine {
class FBOContainer;
class ShaderBaseStruct;
class Mesh;
} // namespace AbyssEngine

// Game-side singletons referenced by pointer.
class ApplicationManager;

// Engine is the renderer/device root; it is referenced at global scope by the
// engine glue (externs.h) and by FBOContainer, so it stays a top-level class.
class Engine;

// MeshFull is the complete in-engine mesh type used by the render entry points.
typedef AbyssEngine::Mesh MeshFull;

// Engine lifecycle callbacks (function-pointer typedefs).
typedef void DestroyCallback(Engine *);
typedef void InitializeCallback(Engine *);

// Per-light fixed-function color slot (GL light color: rgba). The engine keeps up
// to 8 of these contiguously (one per GL light) and indexes them by (light - GL_LIGHT0).
struct LightColor {
    float r;
    float g;
    float b;
    float a;
};

class Engine {
public:
    uint32_t field_0x0;                 // display width
    uint32_t field_0x4;                 // display height (NFC::getHeight())
    uint8_t field_0x8;                  // isPad
    uint32_t field_0xc;
    uint32_t field_0x10;
    String str_0x14;                    // embedded String (auto ctor/dtor)
    uint8_t field_0x20;
    void* field_0x24;                   // file interface
    uint8_t field_0x25;
    int field_0x28;
    bool field_0x2c;
    char** field_0x30;                  // ApplicationManager / texture-manager handle
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
    float field_0xd0;
    float field_0xd4;
    float field_0xd8;
    float field_0xdc;
    int field_0xe0;
    unsigned char field_0xfc;
    uint16_t field_0xfd;
    uint32_t field_0x100;
    float worldViewProjMatrix[16];      // world*view*proj (esMatrixMultiply target)
    float modelMatrixGL[16];            // GL column-major model matrix
    float worldViewMatrixGL[16];        // GL column-major world-view matrix
    float uvMatrix[16];                 // texture/UV matrix (shader path)
    uint32_t field_0x204;               // modelMatrix3x4 row0.x
    uint32_t field_0x208;
    uint32_t field_0x20c;
    uint32_t field_0x210;
    uint32_t field_0x214;
    uint32_t field_0x218;
    uint32_t field_0x21c;
    uint32_t field_0x220;
    uint32_t field_0x224;
    LightColor lightDiffuse;            // per-light diffuse color (rgba, stride 0x10)
    LightColor lightSpecular;           // per-light specular color (rgba, stride 0x10)
    LightColor lightAmbient;            // per-light ambient color (rgba, stride 0x10)
    float field_0x288;
    float field_0x28c;
    float field_0x290;
    uint32_t field_0x294;
    float field_0x298;
    float field_0x29c;
    float field_0x2a0;
    uint32_t field_0x2a4;
    float field_0x2a8;
    float field_0x2ac;
    float field_0x2b0;
    uint32_t field_0x2b4;
    float field_0x2b8;
    float field_0x2bc;
    float field_0x2c0;
    uint32_t field_0x2c4;
    float field_0x2c8;
    Vector lightAmbientShaded;          // per-light shaded ambient (rgb, stride 0x0c)
    Vector lightSpecularShaded;         // per-light shaded specular (rgb, stride 0x0c)
    Vector lightDiffuseShaded;          // per-light shaded diffuse (rgb, stride 0x0c)
    Vector particleAmbient;             // particle ambient color (rgb)
    float field_0x320;
    float field_0x324;
    float field_0x328;
    int field_0x32c;                    // active light count
    Vector field_0x330;
    Vector field_0x33c;
    uint64_t field_0x340;
    int field_0x348;
    float field_0x34c;                  // light dir vector x (y@0x350, z@0x354)
    float field_0x350;
    float field_0x354;
    void* field_0x358;
    int field_0x35c;
    uint32_t field_0x360;
    int field_0x368;                    // display width
    int field_0x36c;                    // display height
    int field_0x370;
    int field_0x374;
    float lightDirty[2];                // per-light "direction is in eye space" flag
    char* field_0x380;                  // quad mesh (DrawQuad)
    float projMatrix[16];               // projection / ortho matrix (shader path)
    uint8_t field_0x3c4;
    uint32_t field_0x3c8;
    uint64_t field_0x3cc;
    uint32_t field_0x3d4;
    Array<int>* triangleCounts;         // per-shader triangle counters
    int field_0x3e4;
    uint32_t field_0x3e8;
    uint32_t field_0x3ec;
    Vector field_0x3f0;
    Vector eyePosition;                 // eye/camera position (SetEyePosition)
    uint64_t field_0x400;
    uint32_t field_0x40c;
    uint32_t field_0x410;               // active post-effect flags
    AbyssEngine::FBOContainer* field_0x414;
    AbyssEngine::FBOContainer* field_0x418;
    uint8_t field_0x41c;
    uint32_t field_0x420;
    uint8_t field_0x424;
    float uvMatrixGL[16];               // fixed-function UV matrix (glLoadMatrixf)
    Vector field_0x468;                 // light[0] direction (light array base)
    Vector field_0x474;                 // light[1] direction
    uint64_t field_0x478;
    bool field_0x480;
    DestroyCallback* field_0x484;
    uint32_t field_0x488;
    int frameBufferTextures[2];         // FBO color-texture id per slot
    uint32_t field_0x4a4;               // GL client-state flags
    uint32_t field_0x4a8;
    double field_0x4b0;
    double field_0x4b8;
    volatile double field_0x4c0;
    volatile double field_0x4c8;
    volatile double field_0x4d0;
    double field_0x4d8;
    double field_0x4e0;
    double field_0x4e8;
    volatile double field_0x4f0;
    volatile double field_0x4f8;
    volatile double field_0x500;
    double field_0x508;
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
    void DrawLine2D(int vertexCount, int count, bool strip);
    void DrawQuad(int x, int y, int width, int height);
    double * GetAccelValue();
    void GetDeviceInfo();
    uint32_t GetDisplayHeight();
    uint32_t GetDisplayWidth();
    double * GetGravValue();
    void GlEnable(unsigned int cap, bool enable);
    void GlowBeginGlow(unsigned int depthFunc);
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
    uint64_t SetEyePosition(uint32_t x, uint32_t y, uint32_t z);
    void SetFrameBufferTexture(int slot0, int slot1);
    void SetGravValue(double x, double y, double z);
    void SetModelMatrix(const uint32_t *matrix);
    void SetOnDestroyApp(DestroyCallback *callback);
    void SetOrthoMatrix(const uint32_t *projection, const uint32_t *view, bool multiply);
    void SetPerspMatrix(const uint32_t *matrix);
    void SetPostEffect(uint32_t effect, bool enable);
    void SetTextureSlot(uint32_t textureIndex, uint32_t slot);
    void SetTextures(uint32_t first, uint32_t second);
    void SetTexturesExt(uint32_t first, uint32_t second, uint32_t third, ...);
    void SetUVMatrix(const uint32_t *matrix);
    void SetWorldViewMatrix(const uint32_t *matrix);
    uint32_t ShaderInit();
    void ShaderRegister(AbyssEngine::ShaderBaseStruct *shader);
    void ShaderSetActive(int shaderIndex, MeshFull *mesh);
    void ShaderSetInActive();
    void ShaderUpdate();
    uint32_t Suspend();
    void SwapBuffer();
    void initFileInterface();
};
#endif
