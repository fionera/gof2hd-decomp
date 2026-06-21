// Android JNI bridge + NDK per-frame entry points, recovered from the binary
// (ghidra_addr 0x7b550.. for the JNI glue, 0xa4018.. for the ndk23_* render
// callbacks). All of these are unmangled C-linkage symbols exported to the Java
// side (the Java_net_fishlabs_* natives and the ndk23_* frame hooks), plus the
// loader's JNI_OnLoad.
//
// The render hooks reach the engine and its ApplicationManager through the same
// global Engine** the rest of the NDK layer uses (GetEngine() double-derefs it).
// Because ApplicationManager / Engine / the touch helpers live in namespace
// AbyssEngine in the binary but are modelled at global scope in our headers, the
// cross-TU references here are declared with explicit asm() labels so they bind
// to the exact mangled symbols the original calls.
#include <jni.h>
#include <android/log.h>

#include <cstdlib>
#include <cstring>

#include "engine/render/Engine.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include "engine/core/GameData.h"
#include "engine/file/AEFile.h"
#include "platform/gl.h"
#include "game/mission/Status.h"

// ---------------------------------------------------------------------------
// Global state shared with the Java side and the rest of the NDK layer.
// ---------------------------------------------------------------------------

// JavaVM captured in JNI_OnLoad (binary .bss 0x22d318).
static JavaVM *g_pVM;
// Current JNIEnv / activity / class handed in by setEnvironmentVariables
// (binary .bss 0x22d314 / 0x22d310 / 0x226970). g_pEnv is shared with the rest
// of the JNI glue, so it is exported.
extern "C" JNIEnv *g_pEnv;
JNIEnv *g_pEnv;
static jobject g_pClass;     // 0x22d310 (the native method's declaring jclass)
extern "C" jobject g_pActivity;  // 0x226970
jobject g_pActivity;

// Asset/zip locations copied out of the Java strings (binary .bss 0x2279a4 /
// 0x2279a8); also read by ToJNI_initialize.
extern "C" char *g_apkPath;  // 0x2279a4
char *g_apkPath;
extern "C" char *g_zipPath;  // 0x2279a8
char *g_zipPath;

// Engine root used by the render hooks: a pointer-to-Engine-pointer the NDK
// layer publishes (binary .bss 0x227b24, the slot GetEngine() double-derefs).
extern "C" AbyssEngine::Engine **g_pEngine;

// Back-button latch raised by the Android key handler (binary .bss 0x22d2fc);
// newrender drains it into a synthetic key press/release each frame.
extern "C" int g_android_back_button_pressed;

// Slow-motion / speed-up latch bytes read back by the Java side through the
// SlowMotion()/SpeedUp() accessors (binary .bss).
static bool g_slowMotion;
static bool g_speedUp;

// Analog gamepad axes pushed in from the Java input layer by
// setValuesForGamepad() (binary .data gamepad record; the two analog values sit
// at offsets +0x14 / +0x18 of that record).
static float g_gamepadAxisX;
static float g_gamepadAxisY;

// ---------------------------------------------------------------------------
// Cross-namespace references bound to their exact original mangled symbols.
// These are external declarations (the definitions live in their owning TUs);
// the asm() labels bind each to the exact symbol the binary calls, sidestepping
// the global-vs-AbyssEngine namespace mismatch in our headers.
// ---------------------------------------------------------------------------

// AbyssEngine::ApplicationManager methods (the manager is reached as
// (*g_pEngine)->appManager).
void AppMgr_SetExitCallback(ApplicationManager *self, void (*cb)())
    asm("_ZN11AbyssEngine18ApplicationManager15SetExitCallbackEPFvvE");
void AppMgr_OnTouchBegin(ApplicationManager *self, int x, int y, void *touch)
    asm("_ZN11AbyssEngine18ApplicationManager12OnTouchBeginEiiPv");
void AppMgr_OnTouchMove(ApplicationManager *self, int x, int y, void *touch)
    asm("_ZN11AbyssEngine18ApplicationManager11OnTouchMoveEiiPv");
void AppMgr_OnTouchEnd(ApplicationManager *self, int x, int y, void *touch)
    asm("_ZN11AbyssEngine18ApplicationManager10OnTouchEndEiiPv");
void AppMgr_OnTouchEnd(ApplicationManager *self)
    asm("_ZN11AbyssEngine18ApplicationManager10OnTouchEndEv");
void AppMgr_OnUpdate(ApplicationManager *self, long long now)
    asm("_ZN11AbyssEngine18ApplicationManager8OnUpdateEx");
char *AppMgr_GetApplicationData(ApplicationManager *self)
    asm("_ZN11AbyssEngine18ApplicationManager18GetApplicationDataEv");

// Free helpers (touch queue + key dispatch + per-frame offerwall poll).
int gof2_GetTouchCount() asm("_Z13GetTouchCountv");
void gof2_GetTouch(int touch) asm("_Z8GetTouchi");
void gof2_RemoveTouches() asm("_Z13RemoveTouchesv");
void gof2_keyPressed(AbyssEngine::Engine *engine, int key)
    asm("_Z10keyPressedPN11AbyssEngine6EngineEi");
void gof2_keyReleased(AbyssEngine::Engine *engine, int key)
    asm("_Z11keyReleasedPN11AbyssEngine6EngineEi");

extern "C" void ndk_checkPlaytimeAndSpendOfferwallCredits();

// ---------------------------------------------------------------------------
// Engine entry points the JNI natives forward into. These live in the wider NDK
// layer (or, for the touch/render hooks, just below); they are plain unmangled
// C-linkage symbols, so they are declared extern "C".
// ---------------------------------------------------------------------------

// Asset bring-up: unpacks the APK + zip, builds the Engine and shows the canvas.
extern "C" void ndk23_InitWithZip(const char *apkPath, const char *zipPath,
                                  int width, int height);
// Copies the asset root / zip directory strings into the loader's own buffers.
extern "C" void ndk23_setRootDirectory(const char *path);
extern "C" void ndk23_setZipDirectory(const char *path);
// One-shot per-frame tick and host-driven lifecycle / status hooks.
extern "C" void ndk23_renderstep(int width, int height);
extern "C" int ndk23_getExitFlag();
extern "C" int ndk23_getScreenshotFlag();
extern "C" void ndk23_resetScreenshotFlag();
extern "C" void ndk23_sendingPauseSignal();
extern "C" void ndk23_sendingResumeSignal();
extern "C" void ndk23_setCountryCode(unsigned int code);
// Accelerometer sink (defined below).
extern "C" void ndk23_handleAcceleration(float x, float y, float z);

// ---------------------------------------------------------------------------
// Loader-owned global state read/written by the bring-up and host hooks.
// ---------------------------------------------------------------------------

// Device framebuffer size cached by InitWithZip and consumed by the render path
// (binary .bss 0x227b38 / 0x227b3c).
static int gRealWidth;
static int gRealHeight;
// Asset root / zip directory copies owned by the NDK setters (binary .bss
// 0x227b40 / 0x227b44); each is a freshly malloc'd, mutable C string.
static char *rootDirectory;
static char *ZIPDirectory;
// Host-requested exit latch read back through getExitFlag (binary .bss 0x227b48).
static int forceExit;
// Locale code clamped to the 0..15 table range (binary .bss 0x2250e0).
static unsigned int countryCode;
// Pending offerwall credit reward, granted once the player has banked any play
// time and then cleared (binary .bss 0x227b54).
static int gb_android_offerwallCreditAmount;

// The NDK loader publishes the singleton Engine through this slot (binary .bss
// 0x227b24); the render hooks double-deref it.
extern "C" AbyssEngine::Engine **g_pEngine;
AbyssEngine::Engine **g_pEngine;

// Globals::status is the canonical Status singleton (binary .bss 0x2281b0) under
// its real mangled name; it is not exposed by Globals.h, so reach it here through
// its exact mangled symbol (mirrors the rest of the NDK layer).
extern Status *Globals_status asm("_ZN7Globals6statusE");

// Asset unpack + the engine's app-create/app-destroy callbacks. loadAPKAndZip is
// a plain C-linkage loader helper; OnCreateApplication is the global
// InitializeCallback the binary registers (no header declares it).
extern "C" int loadAPKAndZip(const char *apkPath, const char *patchPath);
void OnCreateApplication(AbyssEngine::Engine *engine);

// Persists the game after the offerwall reward is granted.
extern "C" void ndk_autosave();

// ---------------------------------------------------------------------------
// Asset / locale setters. Each copies the incoming string into a fresh malloc'd
// buffer (the loader keeps an owned, mutable copy), or clamps the locale code.
// ---------------------------------------------------------------------------

extern "C" void ndk23_setRootDirectory(const char *path)
{
    rootDirectory = static_cast<char *>(std::malloc(std::strlen(path) + 1));
    std::strcpy(rootDirectory, path);
}

extern "C" void ndk23_setZipDirectory(const char *path)
{
    ZIPDirectory = static_cast<char *>(std::malloc(std::strlen(path) + 1));
    std::strcpy(ZIPDirectory, path);
}

extern "C" void ndk23_setCountryCode(unsigned int code)
{
    // The locale table only has sixteen entries; anything past it falls back to 0.
    countryCode = (code > 15) ? 0u : code;
}

// ---------------------------------------------------------------------------
// Host status hooks. The screenshot path is a no-op stub in this build.
// ---------------------------------------------------------------------------

extern "C" int ndk23_getExitFlag()
{
    return forceExit;
}

extern "C" int ndk23_getScreenshotFlag()
{
    return 0;
}

extern "C" void ndk23_resetScreenshotFlag()
{
}

// ---------------------------------------------------------------------------
// Per-frame offerwall reward: once the player has banked any play time and an
// offerwall credit amount is pending, award it, persist the game and clear it.
// ---------------------------------------------------------------------------

extern "C" void ndk_checkPlaytimeAndSpendOfferwallCredits()
{
    if (Globals_status->getPlayingTime() >= 1) {
        int amount = gb_android_offerwallCreditAmount;
        if (amount > 0) {
            Globals_status->changeCredits(amount);
            ndk_autosave();
            gb_android_offerwallCreditAmount = 0;
        }
    }
}

// ---------------------------------------------------------------------------
// Asset bring-up: sets the GL viewport, clears the accelerometer filter state,
// caches the framebuffer size, unpacks the APK + zip, builds the Engine, stamps
// its version string, installs the asset root and the app-create/app-destroy
// callbacks, then forces the canvas into landscape.
// ---------------------------------------------------------------------------

// Low-pass gravity filter state shared with the accelerometer sink (binary .bss
// 0x227b60..0x227b78, six doubles); InitWithZip resets it before the engine
// starts.
double gAccelFilterState[6];

extern "C" void ndk23_InitWithZip(const char *apkPath, const char *zipPath,
                                  int width, int height)
{
    glViewport(0, 0, width, height);

    for (int i = 0; i < 6; ++i)
        gAccelFilterState[i] = 0.0;

    gRealWidth = width;
    gRealHeight = height;

    loadAPKAndZip(apkPath, zipPath);

    AbyssEngine::Engine *engine = new AbyssEngine::Engine();
    *g_pEngine = engine;
    engine->str_0x3c = String("2.0.16", false);

    if (rootDirectory != nullptr)
        AEFile::SetAppRootDir(rootDirectory);

    engine->Initialize(&OnCreateApplication);
    engine->SetOnDestroyApp(&OnDestroyApplication);
    engine->appManager->paintCanvas->SetGameOrientation(AbyssEngine::LandscapeMode_2);
}

// ---------------------------------------------------------------------------
// Accelerometer sink. The Java layer hands us a portrait-relative (x, y, z)
// gravity vector; we remap it onto the engine's landscape axes, low-pass filter
// it into a smoothed gravity estimate (gAccelFilterState[0..2]) and push both the
// raw acceleration and the smoothed gravity into the engine.
// ---------------------------------------------------------------------------

// When set, the analog tilt uses the landscape mapping (-x, y); otherwise the
// device is held in the portrait orientation and the axes are swapped (y, x)
// (binary .bss 0x227b28).
static int rotateAccelValues;

extern "C" void ndk23_handleAcceleration(float x, float y, float z)
{
    double tiltA, tiltB;
    if (rotateAccelValues != 0) {
        tiltA = -x;
        tiltB = y;
    } else {
        tiltA = y;
        tiltB = x;
    }

    AbyssEngine::Engine *engine = *g_pEngine;
    if (engine == nullptr)
        return;

    double *gravity = gAccelFilterState;
    double gz;
    if (engine->appManager->paintCanvas->initialized) {
        // Running low-pass: blend the new sample into the smoothed gravity.
        gravity[0] = gravity[0] * 0.95 + tiltB * 0.05;
        gravity[1] = gravity[1] * 0.95 + tiltA * 0.05;
        gz         = gravity[2] * 0.95 + z * 0.05;
    } else {
        // Rotation disabled: seed the estimate straight from the X tilt.
        gravity[0] = tiltB * 0.95;
        gravity[1] = tiltB * 0.95;
        gz         = tiltB * 0.95;
    }
    gravity[2] = gz;
    gravity[3] = -gravity[0];
    gravity[4] = -gravity[1];
    gravity[5] = -gz;

    engine->SetAccelValue(-tiltB, -tiltA, -z);
    engine->SetGravValue(gravity[3], gravity[4], gravity[5]);
}

// ---------------------------------------------------------------------------
// JNI bridge.
// ---------------------------------------------------------------------------

extern "C" jint JNI_OnLoad(JavaVM *vm, void * /*reserved*/)
{
    g_pVM = vm;
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4) != JNI_OK)
        return -1;
    if (env->FindClass("net/fishlabs/gof2hdallandroid2012/GOF2HD2012") == nullptr)
        return -1;
    return JNI_VERSION_1_4;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_setEnvironmentVariables(
    JNIEnv *env, jclass clazz, jobject activity)
{
    g_pEnv = env;
    g_pClass = clazz;
    g_pActivity = activity;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_setAPKPath(
    JNIEnv *env, jclass /*clazz*/, jstring path)
{
    jboolean isCopy;
    const char *utf = env->GetStringUTFChars(path, &isCopy);
    g_apkPath = static_cast<char *>(std::malloc(std::strlen(utf) + 1));
    if (isCopy) {
        std::strcpy(g_apkPath, utf);
        env->ReleaseStringUTFChars(path, utf);
    }
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_setZIPPath(
    JNIEnv *env, jclass /*clazz*/, jstring path)
{
    jboolean isCopy;
    const char *utf = env->GetStringUTFChars(path, &isCopy);
    g_zipPath = static_cast<char *>(std::malloc(std::strlen(utf) + 1));
    if (isCopy) {
        std::strcpy(g_zipPath, utf);
        env->ReleaseStringUTFChars(path, utf);
    }
}

// Root-asset / zip directory copies owned by SetDirectories (binary .bss
// 0x2279ac / 0x2279b0); each is a freshly malloc'd, mutable C string.
static char *g_rootDir;
static char *g_zipDir;

// Hands the engine its asset root and zip directories. Each incoming Java string
// is copied into a malloc'd buffer, published into the matching loader global and
// forwarded to the NDK, then (when the JNI returned a private copy) released and
// logged.
extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_SetDirectories(
    JNIEnv *env, jclass /*clazz*/, jstring rootDir, jstring zipDir)
{
    jboolean isCopy;
    const char *utf = env->GetStringUTFChars(rootDir, &isCopy);
    g_rootDir = static_cast<char *>(std::malloc(std::strlen(utf) + 1));
    g_zipDir = static_cast<char *>(std::malloc(std::strlen(utf) + 1));
    if (isCopy) {
        std::strcpy(g_rootDir, utf);
        std::strcpy(g_zipDir, utf);
        ndk23_setRootDirectory(g_rootDir);
        __android_log_print(ANDROID_LOG_ERROR, "gof2", "rootDir: %s", g_rootDir);
        __android_log_print(ANDROID_LOG_ERROR, "gof2", "zipDir: %s", g_zipDir);
        env->ReleaseStringUTFChars(rootDir, utf);
    }

    utf = env->GetStringUTFChars(zipDir, &isCopy);
    g_zipDir = static_cast<char *>(std::malloc(std::strlen(utf) + 1));
    if (isCopy) {
        std::strcpy(g_zipDir, utf);
        ndk23_setZipDirectory(g_zipDir);
        __android_log_print(ANDROID_LOG_ERROR, "gof2", "zipDir: %s", g_zipDir);
        env->ReleaseStringUTFChars(zipDir, utf);
    }
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_STARTUP(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    // Reserved Java entry point: the native side has nothing to do at startup.
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_initialize(
    JNIEnv * /*env*/, jclass /*clazz*/, jint width, jint height)
{
    ndk23_InitWithZip(g_apkPath, g_zipPath, width, height);
    ndk23_renderstep(width, height);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_renderstep(
    JNIEnv * /*env*/, jclass /*clazz*/, jint width, jint height)
{
    ndk23_renderstep(width, height);
}

extern "C" jboolean Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getExitFlag(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return ndk23_getExitFlag() != 0;
}

extern "C" jboolean Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getScreenshotFlag(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return ndk23_getScreenshotFlag() != 0;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_resetScreenshotFlag(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    ndk23_resetScreenshotFlag();
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_sendPauseSignalToGame(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    ndk23_sendingPauseSignal();
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_sendResumeSignalToGame(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    ndk23_sendingResumeSignal();
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_setCountryCodeOfDevice(
    JNIEnv * /*env*/, jclass /*clazz*/, jint code)
{
    ndk23_setCountryCode(static_cast<unsigned int>(code));
}

// The Java layer reports portrait-relative acceleration; the engine wants the
// landscape x axis flipped, so the x component is sign-bit-toggled before it is
// handed to the accelerometer sink.
extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_handleAccelerometer(
    JNIEnv * /*env*/, jclass /*clazz*/, jfloat x, jfloat y, jfloat z)
{
    ndk23_handleAcceleration(-x, y, z);
}

// ---------------------------------------------------------------------------
// Per-frame render hook: drains the touch queue into the ApplicationManager,
// turns a pending back-button press into a synthetic key event, ticks the
// application and clears any one-shot application-data flags.
// ---------------------------------------------------------------------------

// External exit callback handed to the manager (binary 0xa3c98).
extern "C" void ExitFunction();

extern "C" void ndk23_newrender(long long now)
{
    ApplicationManager *manager = (*g_pEngine)->appManager;
    AppMgr_SetExitCallback(manager, &ExitFunction);

    int touchCount = gof2_GetTouchCount();
    for (int i = 0; i < touchCount; ++i) {
        // GetTouch fills a 4-word record: [touch, phase, x, y].
        int rec[4];
        gof2_GetTouch(reinterpret_cast<int>(rec));
        void *touch = reinterpret_cast<void *>(rec[0]);
        int phase = rec[1];
        int x = rec[2];
        int y = rec[3];
        if (phase == 2) {
            AppMgr_OnTouchMove(manager, x, y, touch);
        } else if (phase == 1) {
            AppMgr_OnTouchEnd(manager, x, y, touch);
            AppMgr_OnTouchEnd(manager);
        } else if (phase == 0) {
            AppMgr_OnTouchBegin(manager, x, y, touch);
        }
    }

    if (g_android_back_button_pressed != 0) {
        gof2_keyPressed(*g_pEngine, 0x35);
        gof2_keyReleased(*g_pEngine, 0x35);
        g_android_back_button_pressed = 0;
    }

    gof2_RemoveTouches();
    AppMgr_OnUpdate(manager, now);

    char *appData = AppMgr_GetApplicationData(manager);
    if (appData[0x3d] != 0) {
        appData[0x40] = 1;
        appData[0x3d] = 0;
    } else if (appData[0x3c] != 0) {
        appData[0x3c] = 0;
    } else if (appData[0x3e] != 0) {
        appData[0x3e] = 0;
    } else if (appData[0x3f] != 0) {
        appData[0x3f] = 0;
    } else if (appData[0xa0] != 0) {
        appData[0xa0] = 0;
    } else if (appData[0xa1] != 0) {
        appData[0xa1] = 0;
    } else if (appData[0xa2] != 0) {
        appData[0xa2] = 0;
    } else if (appData[0xa3] != 0) {
        appData[0xa3] = 0;
    }

    ndk_checkPlaytimeAndSpendOfferwallCredits();
}

// ---------------------------------------------------------------------------
// Direct touch injection (used by the touch-pad / touch-screen host paths, which
// bypass the queued newrender path and dispatch straight into the manager). The
// x/y come in as floats and are truncated to integer pixels.
// ---------------------------------------------------------------------------

extern "C" void ndk23_handleTouchPadEvent(jclass /*clazz*/, void *touch, int phase,
                                          float x, float y)
{
    ApplicationManager *manager = (*g_pEngine)->appManager;
    int px = static_cast<int>(x);
    int py = static_cast<int>(y);
    if (phase == 2) {
        AppMgr_OnTouchMove(manager, px, py, touch);
    } else if (phase == 1) {
        AppMgr_OnTouchEnd(manager, px, py, touch);
        AppMgr_OnTouchEnd(manager);
    } else if (phase == 0) {
        AppMgr_OnTouchBegin(manager, px, py, touch);
    }
}

extern "C" void ndk23_handleTouchScreenEvent(jclass clazz, void *touch, int phase,
                                             float x, float y)
{
    ndk23_handleTouchPadEvent(clazz, touch, phase, x, y);
}

// ---------------------------------------------------------------------------
// Engine teardown: releases and destroys the singleton Engine, clearing the slot.
// ---------------------------------------------------------------------------

extern "C" void ndk23_ndkDone()
{
    AbyssEngine::Engine *engine = *g_pEngine;
    if (engine != nullptr) {
        engine->Release();
        delete engine;
        *g_pEngine = nullptr;
    }
}

// ---------------------------------------------------------------------------
// Host-side input accessors. SlowMotion()/SpeedUp() report the matching latch
// bytes; setValuesForGamepad() stores the latest analog stick values.
// ---------------------------------------------------------------------------

bool SlowMotion()
{
    return g_slowMotion;
}

bool SpeedUp()
{
    return g_speedUp;
}

void setValuesForGamepad(float x, float y)
{
    g_gamepadAxisX = x;
    g_gamepadAxisY = y;
}
