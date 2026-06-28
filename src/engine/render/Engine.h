#ifndef GOF2_ENGINE_H
#define GOF2_ENGINE_H
#include "engine/core/Array.h"
#include "../core/AEString.h"
#include "engine/math/Vector.h"
#include "engine/math/Matrix.h"

#include "engine/math/AEMath.h"

#include "engine/render/DeviceInfo.h"
#include "engine/render/LightColor.h"
class FileInterface;

namespace AbyssEngine {
    class FBOContainer;
    class Mesh;
    class ShaderBaseStruct;
}

namespace AbyssEngine {
    class Engine;
    class ApplicationManager;
    enum LandscapeMode : int;

    extern bool PostEffectFlag;
}

using ::AbyssEngine::ApplicationManager;

typedef void DestroyCallback(AbyssEngine::Engine *);

typedef void InitializeCallback(AbyssEngine::Engine *);

void glError();

namespace AbyssEngine {
    class Engine {
    public:
        uint32_t deviceWidth;
        uint32_t deviceHeight;
        uint8_t isPad;

        uint32_t maxTextureSize;

        uint32_t lastGlError;

        String lastErrorPath;

        uint8_t linearFilterFlag;

        FileInterface *fileInterface;

        // Supernova-flare timeline (read as a float, `1.0f <= explosionTimeline`, in Level.cpp).
        // NOTE: not a real type-pun. The ctor only raw-int-inits it to 0x14 (see Engine.cpp); the
        // float writer that ramps it toward 1.0 is not yet recovered, so today the flare check is
        // effectively dead. Modelled as the float it is read as; the int init is an explicit
        // reinterpret at the one write site.
        float explosionTimeline;

        bool vibrationSupported;
        uint8_t _pad0x2d[3];

        ApplicationManager *appManager;

        uint32_t field_0x34;
        uint8_t _pad0x38[4];
        String str_0x3c;
        uint8_t _pad0x48[4];
        String str_0x4c;
        uint64_t field_0x58;

        int drawCallCountB;
        int drawCallCountA;
        int triangleCountA;
        int triangleCountB;

        int textureByteCounter;

        bool field_0x74;
        uint8_t _pad0x75[3];
        int field_0x78;
        int boundTextures[20];
        float field_0xcc;

        float glColor[4];

        int packedColor;
        uint8_t _pad0xe4[24];
        unsigned char shaderModeFlag;
        unsigned char statsBucketFlag;

        union {
            // lint: union_decl multi-size pun (uint16 0x100 store vs uint8 read)
            uint16_t field_0xfd;
            unsigned char statsEnabled;
        };

        uint32_t field_0x100;
        float worldViewProjMatrix[16];
        float modelMatrixGL[16];
        float worldViewMatrixGL[16];
        float uvMatrix[16];

        float normalMatrix[9];

        LightColor lightDiffuse;
        uint8_t _pad0x238[16];

        LightColor lightSpecular;
        uint8_t _pad0x258[16];

        LightColor lightAmbient;
        uint8_t _pad0x278[16];

        float sceneAmbient[4];

        float materialDiffuse[4];

        float materialAmbient[4];

        float materialSpecular[4];

        float materialShininess;

        Vector lightAmbientShaded;
        Vector lightSpecularShaded;
        Vector lightDiffuseShaded;
        Vector particleAmbient;

        Vector field_0x2fc;
        Vector field_0x308;
        Vector field_0x314;

        Vector rimColor;

        int lightCount;

        Vector lightDir;

        Vector field_0x33c;

        int lineVertexBase;

        Vector lightColor;

        unsigned char *addData;
        int addDataSize;

        int autoPilotEngaged;

        uint8_t _pad0x364[4];

        int displayWidth;

        int displayHeight;

        int viewportWidth;
        int viewportHeight;
        float lightDirty[2];

        Mesh *quadMesh;

        float projMatrix[16];
        uint8_t field_0x3c4;
        uint32_t field_0x3c8;

        uint32_t field_0x3cc;
        uint32_t field_0x3d0;
        uint32_t field_0x3d4;
        uint32_t field_0x3d8;
        Array<int> *triangleCounts;
        uint32_t field_0x3e0;

        int currentProgram;

        float fogMinDist;

        float fogMaxDist;

        Vector fogColor;

        Vector eyePosition;

        uint8_t _pad0x408[4];

        uint32_t viewFramebuffer;

        uint32_t postEffectFlags;
        AbyssEngine::FBOContainer *postEffectFBO;

        AbyssEngine::FBOContainer *refractFBO;

        uint8_t glowActive;
        uint32_t glEnableFlags;
        uint8_t lightingEnabled;
        float uvMatrixGL[16];
        Vector field_0x468;

        Vector field_0x474;

        bool hasVibration;
        DestroyCallback *onDestroyCallback;
        float materialAlpha;
        int frameBufferTextures[2];
        uint8_t _pad0x494[16];
        uint32_t clientStateFlagsAE;
        uint32_t field_0x4a8;

        double accelRaw[3];
        volatile double accelValue[3];

        double gravRaw[3];
        volatile double gravValue[3];

        uint32_t shaderCount;
        Array<ShaderBaseStruct *> *shaders;

        Engine();

        ~Engine();

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

        void RenderMesh(Mesh *mesh);

        void ResetLightParam();

        void ResetUVMatrix();

        uint32_t Resume();

        void SetAccelValue(double x, double y, double z);

        void SetAddData(void *data, int size); // lint: void_ptr exported method signature

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

        void PreUpdate();

        void Release();

        void ActivateFrameBuffer(int slot);

        void SetFrameBufferScaleFactor(float factor, int slot);

        void GrabFrameBuffer();

        void *GetJPEGImageData(float quality); // lint: void_ptr exported method signature

        void SaveImageToPhotosAlbum();

        void SetScreenOrientation(AbyssEngine::LandscapeMode orientation);

        bool IsRefractActivated();

        void Vibrate(unsigned short duration);

        void VibrateSupported();

        void SetUVMatrix(const AbyssEngine::AEMath::Matrix &matrix);

        void SetWorldViewMatrix(const AbyssEngine::AEMath::Matrix &matrix);

        uint32_t ShaderInit();

        void ShaderRegister(AbyssEngine::ShaderBaseStruct *shader);

        void ShaderSetActive(int shaderIndex, Mesh *mesh);

        void ShaderSetInActive();

        void ShaderUpdate();

        uint32_t Suspend();

        void SwapBuffer();

        void initFileInterface();

        static bool vboSupported;
        static bool clampTextures;
        static bool vfc;
        static float lodBiasDiffuse;
        static float lodBiasNormal;
        static unsigned int countryCode;

        static bool EnablePostEffect;

        static bool CheckForOrientationChange;

        static bool fogEnabled;

        static unsigned char EnableGlow;
        static int ImageCount;
        static int switchGlow;
        static int SwapCounter;
        static int switchBloom;
        static unsigned char enableShader;
        static unsigned char EnableRefract;
        static ShaderBaseStruct *LodDistShader;
        static unsigned char DisableRefract;
        static int AnisotropyValue;
        static unsigned char KeepRawMeshData;
        static unsigned char backupSaveGames;
        static int MultiSampleValue;
        static unsigned char UseAdvancedShader;
        static unsigned char enableReverseFlag;
        static int tv_h;
        static int tv_w;
        static AbyssEngine::AEMath::Vector vendor;
        static unsigned char DrawFBO;
        static int vboSize;
        static unsigned char DEBUGKEY;
        static unsigned char TVEnable;
        static float farPlane;
        static AbyssEngine::AEMath::Vector renderer;
        static AbyssEngine::AEMath::Vector tv_modes;
        static unsigned char WireFrame;
        static float nearPlane;
    };
}

using ::AbyssEngine::Engine;

extern AbyssEngine::Engine *gEngine;

#endif
