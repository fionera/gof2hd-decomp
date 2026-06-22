#ifndef GOF2_ENGINE_H
#define GOF2_ENGINE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/math/Vector.h"
#include "engine/math/Matrix.h"

// The class body below uses the unqualified spellings `Vector` (engine math vec3)
// for its color/light/direction fields; bring it into scope here.
namespace AbyssEngine { using AEMath::Vector; }

namespace AbyssEngine {
    class FBOContainer;
    class ShaderBaseStruct;
    class Mesh;
} // namespace AbyssEngine

namespace AbyssEngine {
    class ApplicationManager;
}

using ::AbyssEngine::ApplicationManager;

namespace AbyssEngine {
    class Engine;
}

using ::AbyssEngine::Engine;

typedef AbyssEngine::Mesh MeshFull;

typedef void DestroyCallback(Engine *);

typedef void InitializeCallback(Engine *);

void glError();

struct DeviceInfo {
    uint32_t width; // +0x0 NFC::getWidth()
    uint32_t height; // +0x4 NFC::getHeight()
    uint8_t isPad; // +0x8 NFC::isPad()
};

struct LightColor {
    float r;
    float g;
    float b;
    float a;
};

namespace AbyssEngine {
#ifndef GOF2_ENUM_LandscapeMode
#define GOF2_ENUM_LandscapeMode

    enum LandscapeMode { LandscapeMode_dummy = 0, LandscapeMode_1 = 1, LandscapeMode_2 = 2, LandscapeMode_3 = 3 };
#endif

    class Engine {
    public:
        uint32_t deviceWidth; // +0x0 device width (NFC::getWidth())
        uint32_t deviceHeight; // +0x4 device height (NFC::getHeight())
        uint8_t isPad; // +0x8 NFC::isPad()
        union {
            uint32_t maxTextureSize;
            uint32_t depthBits;
        }; // +0xc glGetIntegerv(GL_MAX_TEXTURE_SIZE) (was depthBits; the binary queries GL_MAX_TEXTURE_SIZE here)
        union {
            uint32_t lastGlError;
            uint32_t field_0x10;
        }; // +0x10 last glGetError() result
        String lastErrorPath;

        // +0x14 embedded String (12B, auto ctor/dtor): path of last failed texture load (str_0x14)
        union {
            uint8_t linearFilterFlag;
            uint8_t field_0x20;
        }; // +0x20 use linear (GL_LINEAR) min/mag filter
        void *fileInterface; // +0x24 FileInterfaceAndroid* (AEFile backend) (0x24..0x27)
        // +0x28 reinterpreted as float by Level.cpp (explosion timeline progress).
        union {
            int field_0x28;
            float explosionTimeline;
        };

        bool vibrationSupported; // +0x2c device vibration capability (HasVibration)
        uint8_t _pad0x2d[3]; //  0x2d..0x2f (align appManager to 0x30)
        // +0x30 owned ApplicationManager. External TUs read this as a raw char** to reach the
        // PaintCanvas at *appManager, so the field_0x30 alias is preserved for them. The conversion
        // phase also wants a `paintCanvas` view of this slot (source casts *(void**)(c+0x30) to PaintCanvas*).
        union {
            ApplicationManager *appManager;
            char **field_0x30;
            void *paintCanvas;
        };

        uint32_t field_0x34; // +0x34
        uint8_t _pad0x38[4]; //  0x38..0x3b (align str_0x3c to 0x3c)
        String str_0x3c; // +0x3c embedded String (12B, auto ctor/dtor)
        uint8_t _pad0x48[4]; //  0x48..0x4b (align str_0x4c to 0x4c)
        String str_0x4c; // +0x4c embedded String (12B, auto ctor/dtor)
        uint64_t field_0x58; // +0x58 (0x58..0x5f)
        // +0x60..0x6c per-frame render counters tallied by RenderMesh.
        int drawCallCountB; // +0x60
        int drawCallCountA; // +0x64
        int triangleCountA; // +0x68
        int triangleCountB; // +0x6c
        union {
            int textureByteCounter;
            uint32_t field_0x70;
        }; // +0x70 running uploaded-texture byte total
        bool field_0x74; // +0x74
        uint8_t _pad0x75[3]; //  0x75..0x77 (align field_0x78 to 0x78)
        int field_0x78; // +0x78
        int boundTextures[20]; // +0x7c per-slot bound GL texture id (slot 0..19) (0x7c..0xcb)
        float field_0xcc; // +0xcc scalar bound by the cube shaders (u_*)
        // +0xd0 current draw color (rgba), set by SetColor; bound to glColor / material diffuse.
        union {
            float glColor[4];

            struct {
                float flCurrentColorR, field_0xd4, field_0xd8, field_0xdc;
            };
        };

        int packedColor; // +0xe0 packed RGBA from SetColor (A | G<<16 | R<<24 | B<<8) (0xe0..0xe3)
        uint8_t _pad0xe4[24]; //  0xe4..0xfb (no live fields in the binary here)
        unsigned char shaderModeFlag; // +0xfc shader-vs-fixed-function path flag (field_0xfc)
        uint16_t field_0xfd; // +0xfd (natural alignment lands it at 0xfe; field_0x100 still follows at 0x100)
        uint32_t field_0x100; // +0x100
        float worldViewProjMatrix[16]; // world*view*proj (esMatrixMultiply target)
        float modelMatrixGL[16]; // GL column-major model matrix
        float worldViewMatrixGL[16]; // GL column-major world-view matrix
        float uvMatrix[16]; // texture/UV matrix (shader path)
        // +0x204 normal (rotation 3x3) matrix, rows packed from the model matrix; bound as u_NormalMatrix.
        union {
            float normalMatrix[9];

            struct {
                uint32_t field_0x204, field_0x208, field_0x20c,
                        field_0x210, field_0x214, field_0x218,
                        field_0x21c, field_0x220, field_0x224;
            };
        };

        // +0x228..0x287 fixed-function light color block. The binary lays these out sparsely
        // (per-light rgb at 0x228 / 0x248 with padding, diffuse-alpha at 0x264, ambient at 0x268);
        // none of these sub-fields are referenced by name outside Engine.cpp, so the block is sized
        // to land the following named fields (sceneAmbient @0x288, materialDiffuse @0x298) correctly.
        float flLightDiffuseR, flLightDiffuseG, flLightDiffuseB; // +0x228 per-light diffuse rgb
        uint8_t _pad0x234[20]; //  0x234..0x247
        float flLightSpecularR, flLightSpecularG, flLightSpecularB; // +0x248 per-light specular rgb
        uint8_t _pad0x254[16]; //  0x254..0x263
        float flLightDiffuseA; // +0x264 per-light diffuse alpha
        uint8_t _pad0x268[32]; //  0x268..0x287 per-light ambient block (qwLightAmbient*)
        // +0x288 global scene ambient color (rgba). Bound by LightSetGlobalSceneColorAmbient
        // to GL_LIGHT_MODEL_AMBIENT / the lighting shaders.
        union {
            float sceneAmbient[4];

            struct {
                float field_0x288, field_0x28c, field_0x290;
                uint32_t field_0x294;
            };
        };

        // Fixed-function material colors (rgba). Bound by Light*Material* and the lighting shaders.
        union {
            float materialDiffuse[4];

            struct {
                float field_0x298, field_0x29c, field_0x2a0;
                uint32_t field_0x2a4;
            };
        }; // GL_DIFFUSE
        union {
            float materialAmbient[4];

            struct {
                float field_0x2a8, field_0x2ac, field_0x2b0;
                uint32_t field_0x2b4;
            };
        }; // GL_AMBIENT
        union {
            float materialSpecular[4];

            struct {
                float field_0x2b8, field_0x2bc, field_0x2c0;
                uint32_t field_0x2c4;
            };
        }; // GL_SPECULAR
        union {
            float materialShininess;
            float field_0x2c8;
        }; // GL_SHININESS
        Vector lightAmbientShaded; // per-light shaded ambient (rgb, stride 0x0c)
        Vector lightSpecularShaded; // per-light shaded specular (rgb, stride 0x0c)
        Vector lightDiffuseShaded; // per-light shaded diffuse (rgb, stride 0x0c)
        Vector particleAmbient; // particle ambient color (rgb)
        // Contiguous with the shaded-light block above: Engine indexes (&lightAmbientShaded)[i]
        // across these, and the lighting shaders bind them by offset.
        Vector field_0x2fc;
        Vector field_0x308;
        Vector field_0x314;

        union {
            Vector rimColor;

            struct {
                float field_0x320, field_0x324, field_0x328;
            };
        }; // +0x320 rim-light color (LightSetRimColor)
        union {
            int lightCount;
            int field_0x32c;
        }; // +0x32c active light count
        union {
            Vector lightDir;
            Vector field_0x330;
        }; // scene light direction (rgb-as-xyz)
        // +0x33c second light-direction vector (shaders read .x/.y/.z at 0x33c/0x340/0x344).
        // The binary init also zeroes its .y by name as field_0x340, so alias it here. Modeled
        // as a 12-byte Vector (NOT Vector+uint64, which would over-size the slot by 8 bytes).
        union {
            Vector field_0x33c;
            struct {
                float field_0x33c_x;
                uint32_t field_0x340;
                float field_0x344;
            };
        };

        union {
            int lineVertexBase;
            int field_0x348;
        }; // +0x348 base vertex index for DrawLine2D
        union {
            Vector lightColor;

            struct {
                float field_0x34c, field_0x350, field_0x354;
            };
        }; // scene light color
        void *addData; // +0x358 extra per-frame data (SetAddData)
        int addDataSize; // +0x35c size of addData
        union {
            int autoPilotEngaged;
            uint32_t field_0x360;
        }; // +0x360 autopilot-engaged flag (PlayerEgo::setAutoPilot)
        uint8_t _pad0x364[4]; //  0x364..0x367 (Ghidra gap before framebufferWidth)
        union {
            int framebufferWidth;
            int displayWidth;
        }; // +0x368 GL framebuffer/display width (GetDisplayWidth)
        union {
            int framebufferHeight;
            int displayHeight;
        }; // +0x36c GL framebuffer/display height (GetDisplayHeight)
        int viewportWidth; // +0x370
        int viewportHeight; // +0x374
        float lightDirty[2]; // per-light "direction is in eye space" flag
        // +0x380 shared quad mesh used by DrawQuad and the post-process shaders. External TUs
        // walk it as a raw char* (+4 = positions, +8 = uvs), so the field_0x380 alias stays.
        union {
            MeshFull *quadMesh;
            char *field_0x380;
        };

        float projMatrix[16]; // projection / ortho matrix (shader path)
        uint8_t field_0x3c4;
        uint32_t field_0x3c8;
        // +0x3cc MGame writes a Vector here via *(Vector*)&field_0x3cc. Modeled as two 4-byte
        // words (NOT a uint64, which would 8-align the field to 0x3d0 and shift the tail).
        uint32_t field_0x3cc; // +0x3cc
        uint32_t field_0x3d0; // +0x3d0
        uint32_t field_0x3d4; // +0x3d4
        uint32_t field_0x3d8; // +0x3d8 (Ghidra has a live word here before triangleCounts)
        Array<int> *triangleCounts; // +0x3dc per-shader triangle counters
        uint32_t field_0x3e0; // +0x3e0 (Ghidra gap before currentProgram)
        union {
            int currentProgram;
            int field_0x3e4;
        }; // +0x3e4 currently-bound GL program (ShaderSetActive)
        union {
            float fogMinDist;
            uint32_t field_0x3e8;
        }; // u_fogMinDist
        union {
            float fogMaxDist;
            uint32_t field_0x3ec;
        }; // u_fogMaxDist
        union {
            Vector fogColor;
            Vector field_0x3f0;
        }; // u_fogColor (rgb)
        // +0x3fc eye/camera position (SetEyePosition writes (field_0x3f0).field_0xc, i.e. 0x3fc).
        // The init zeroes 0x400 by name as field_0x400 (= eyePosition.y), so alias it here.
        union {
            Vector eyePosition;
            struct {
                float eyePosition_x;
                uint32_t field_0x400;
                float eyePosition_z;
            };
        };
        uint8_t _pad0x408[4]; //  0x408..0x40b (Ghidra gap before viewFramebuffer)
        union {
            uint32_t viewFramebuffer;
            uint32_t field_0x40c;
        }; // +0x40c default GL framebuffer (view buffer)
        uint32_t postEffectFlags; // +0x410 active post-effect flags
        AbyssEngine::FBOContainer *postEffectFBO; // +0x414 post-effect render target
        union {
            AbyssEngine::FBOContainer *fboContainer;
            AbyssEngine::FBOContainer *refractFBO;
        }; // +0x418 refraction render target (fboContainer)
        uint8_t glowActive; // +0x41c glow pass currently active
        uint32_t glEnableFlags; // +0x420 GlEnable() shader-path capability bits
        uint8_t lightingEnabled; // +0x424 fixed-function lighting enabled
        float uvMatrixGL[16]; // fixed-function UV matrix (glLoadMatrixf)
        Vector field_0x468; // +0x468 light[0] direction (light array base) (0x468..0x473)
        // +0x474 light[1] direction. The init zeroes 0x478 by name as field_0x478 (= .y), so alias it.
        // Modeled as a 12-byte Vector (NOT Vector+uint64, which would over-size the slot by 8 bytes).
        union {
            Vector field_0x474;
            struct {
                float field_0x474_x;
                uint32_t field_0x478;
                float field_0x47c;
            };
        };
        bool hasVibration; // +0x480 device supports vibration (HasVibration)
        DestroyCallback *onDestroyCallback; // +0x484 app-destroy callback (SetOnDestroyApp)
        float materialAlpha; // +0x488 current material alpha (LightSetMaterialColorAlpha)
        int frameBufferTextures[2]; // +0x48c FBO color-texture id per slot (0x48c..0x493)
        uint8_t _pad0x494[16]; //  0x494..0x4a3 (Ghidra gap before clientStateFlagsAE)
        uint32_t clientStateFlagsAE; // +0x4a4 AEClientState() enable bits
        uint32_t field_0x4a8;

        // +0x4b0 accelerometer: raw x/y/z (SetAccelValue) then orientation-corrected x/y/z (GetAccelValue).
        union {
            struct {
                double accelRaw[3];
                volatile double accelValue[3];
            };

            struct {
                double field_0x4b0, field_0x4b8;
                volatile double field_0x4c0, field_0x4c8, field_0x4d0;
                double field_0x4d8;
            };
        };

        // +0x4e0 gravity: raw x/y/z (SetGravValue) then orientation-corrected x/y/z (GetGravValue).
        union {
            struct {
                double gravRaw[3];
                volatile double gravValue[3];
            };

            struct {
                double field_0x4e0, field_0x4e8;
                volatile double field_0x4f0, field_0x4f8, field_0x500;
                double field_0x508;
            };
        };

        uint32_t shaderCount; // +0x510 registered-shader count (Ghidra shaderCount)
        Array<AbyssEngine::ShaderBaseStruct *> *shaders; // +0x514 registered shaders

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

        double *GetAccelValue();

        // Returns the live device geometry/capabilities (NFC width/height/isPad). The
        // original returns the struct by value (sret); callers branch on it to derive the
        // iPad / retina / large-screen capability flags.
        DeviceInfo GetDeviceInfo();

        uint32_t GetDisplayHeight();

        uint32_t GetDisplayWidth();

        double *GetGravValue();

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

        void *GetJPEGImageData(float quality);

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

        // ---- device render-capability globals (recovered .bss/.data) ------------
        // Plain globals in the original under the AbyssEngine::Engine scope; modelled as
        // static members so they mangle to the same AbyssEngine::Engine::<name> symbols.
        // OnCreateApplication seeds these from the device-info probe / fixed defaults.
        static bool vboSupported; // 0x227ab8 vertex-buffer-objects available
        static bool clampTextures; // 0x227ab9 clamp texture wrap mode (low-mem path)
        static bool vfc; // 0x227aba view-frustum culling enabled
        static float lodBiasDiffuse; // 0x2250c8 diffuse-map LOD bias (-1.3)
        static float lodBiasNormal; // 0x227ab4 normal-map LOD bias (-0.5)
        static unsigned int countryCode; // 0x2250e0 device locale / language code

        // Post-processing toggle flipped by the synthetic-touch glue (simulateTouch)
        // while the loading screen / GL post pass is active.
        static bool EnablePostEffect; // 0x2250d8
    };
} // namespace AbyssEngine

using ::AbyssEngine::Engine;

extern Engine *gEngine; // canonical Engine singleton (binary .bss 0x2281e8)

#endif
