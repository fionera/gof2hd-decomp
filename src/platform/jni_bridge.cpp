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
#include "platform/recovered_a462c.h"
#include "platform/recovered_7a41c.h"

// Clears the cached native store item list (defined in LODManager.cpp); a plain
// C-linkage loader hook, declared here to avoid pulling in the LOD render header.
extern "C" void ndk_resetNativeItemInformationList();

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

// ---------------------------------------------------------------------------
// DLC purchase bridge. The Java store UI reads each pack's "already bought"
// flag through ToJNI_getDLCnBOUGHT (a thin forward to the matching ndk_getDLC_n_
// BOUGHT accessor) and, when restoring purchases, marks a pack as owned through
// ToJNI_correctBoughtDLCn (a forward to ndk_iapBoughtPremium(pack, /*bought=*/1)
// with the zero-based pack index).
// ---------------------------------------------------------------------------

extern "C" jboolean Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC1BOUGHT(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return ndk_getDLC_1_BOUGHT();
}

extern "C" jboolean Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC2BOUGHT(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return ndk_getDLC_2_BOUGHT();
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC1(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    ndk_iapBoughtPremium(0, 1);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC2(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    ndk_iapBoughtPremium(1, 1);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC3(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    ndk_iapBoughtPremium(2, 1);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC4(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    ndk_iapBoughtPremium(3, 1);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC5(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    ndk_iapBoughtPremium(4, 1);
}

// ---------------------------------------------------------------------------
// Google Play achievement / leaderboard / link bridge. The Java side keeps the
// pending achievement-id and leaderboard-score arrays plus the GP-link state
// behind these pointers (allocated and published from the Java layer); the
// native code only reads back the queued values and clears them once consumed.
// ---------------------------------------------------------------------------

// Queued achievement ids (the engine pushes up to three at a time) and per-rank
// leaderboard scores; the GP-link flag, and the two "show overlay" request flags
// the host polls each frame.
static jint *g_achievementIds;
static jint *g_leaderboardScores;
static jint *g_linkGameGP;
static jint *g_showAchievements;
static jint *g_showLeaderboards;

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetAchievementId(
    JNIEnv * /*env*/, jclass /*clazz*/, jint index)
{
    return g_achievementIds[index];
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetLeaderboardScore(
    JNIEnv * /*env*/, jclass /*clazz*/, jint index)
{
    return g_leaderboardScores[index];
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetLeaderboardScore(
    JNIEnv * /*env*/, jclass /*clazz*/, jint index)
{
    g_leaderboardScores[index] = 0;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetAchievements(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    g_achievementIds[0] = 0;
    g_achievementIds[1] = 0;
    g_achievementIds[2] = 0;
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetShowAchievements(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return *g_showAchievements;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetShowAchievements(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    *g_showAchievements = 0;
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetLinkGameGP(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return *g_linkGameGP;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetLinkGameGP(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    *g_linkGameGP = 0;
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetShowLeaderboards(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return *g_showLeaderboards;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetShowLeaderboards(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    *g_showLeaderboards = 0;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_SetGPIsLinked(
    JNIEnv * /*env*/, jclass /*clazz*/, jint linked)
{
    *g_linkGameGP = linked;
}

// ---------------------------------------------------------------------------
// Remaining DLC "already bought" accessors (packs 3..5). Each is the same thin
// forward to its ndk_getDLC_n_BOUGHT accessor as packs 1..2 above.
// ---------------------------------------------------------------------------

extern "C" jboolean Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC3BOUGHT(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return ndk_getDLC_3_BOUGHT();
}

extern "C" jboolean Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC4BOUGHT(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return ndk_getDLC_4_BOUGHT();
}

extern "C" jboolean Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC5BOUGHT(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return ndk_getDLC_5_BOUGHT();
}

// ---------------------------------------------------------------------------
// Title / boot state queries. The host polls these to decide when the splash
// has cleared and whether the player is sitting in the main menu.
// ---------------------------------------------------------------------------

extern "C" jboolean Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getLogoShown(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return ndk_getLogoShown() != 0;
}

extern "C" jboolean Java_net_fishlabs_gof2hdallandroid2012_ToJNI_isInMainMenu(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return ndk_isInMainMenu() != 0;
}

// ---------------------------------------------------------------------------
// Window resize / direct touch host hooks. resize re-runs a render step at the
// new framebuffer size; handleTouchEvent injects a single screen touch (touch
// id, phase, x, y) straight into the active application module.
// ---------------------------------------------------------------------------

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_resize(
    JNIEnv * /*env*/, jclass /*clazz*/, jint width, jint height)
{
    ndk23_renderstep(width, height);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_handleTouchEvent(
    JNIEnv * /*env*/, jclass clazz, jint touch, jint phase, jfloat x, jfloat y)
{
    ndk23_handleTouchScreenEvent(clazz, reinterpret_cast<void *>(touch), phase, x, y);
}

// ---------------------------------------------------------------------------
// In-app-purchase debug / completion bridge.
//   testPurchase            grants the first consumable credit pack (index 0)
//                           as a developer test purchase.
//   iapBoughtConsumable     awards the credit pack the host just resolved.
//   iapBoughtPremium        records the bought/refunded state of a DLC pack.
//   iapSet/ResetNativeItemInformationList  load / clear the cached store list.
// ---------------------------------------------------------------------------

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_testPurchase(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    ndk_iapBoughtConsumable(0);
}

extern "C" void Java_net_fishlabs_googleplay_ToJNI_iapBoughtConsumable(
    JNIEnv * /*env*/, jclass /*clazz*/, jint consumable)
{
    ndk_iapBoughtConsumable(static_cast<unsigned int>(consumable));
}

extern "C" void Java_net_fishlabs_googleplay_ToJNI_iapBoughtPremium(
    JNIEnv * /*env*/, jclass /*clazz*/, jint pack, jint bought)
{
    ndk_iapBoughtPremium(static_cast<unsigned int>(pack),
                         static_cast<unsigned int>(bought));
}

extern "C" void Java_net_fishlabs_googleplay_ToJNI_iapSetNativeItemInformationList(
    JNIEnv *env, jclass clazz, jobjectArray ids, jobjectArray names,
    jobjectArray descriptions, jobjectArray currencies, jobjectArray prices)
{
    ndk_setNativeItemInformationList(env, clazz, ids, names, descriptions,
                                     currencies, prices);
}

extern "C" void Java_net_fishlabs_googleplay_ToJNI_iapResetNativeItemInformationList(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    ndk_resetNativeItemInformationList();
}

// ---------------------------------------------------------------------------
// Google Play store SKU lookup. The billing layer asks the native side for the
// product id of each of the five consumable / premium products by index; an
// out-of-range index yields a null string.
// ---------------------------------------------------------------------------

// Consumable credit-pack product ids, smallest pack first.
static const char *const kConsumableSKU[5] = {
    "net.fishlabs.gof2hd2012.creditpack1",
    "net.fishlabs.gof2hd2012.creditpack2",
    "net.fishlabs.gof2hd2012.creditpack3",
    "net.fishlabs.gof2hd2012.creditpack4",
    "net.fishlabs.gof2hd2012.creditpack5",
};

// Premium DLC product ids.
static const char *const kPremiumSKU[5] = {
    "net.fishlabs.gof2hd2012.dlc1",
    "net.fishlabs.gof2hd2012.dlc2",
    "net.fishlabs.gof2hd2012.dlc3",
    "net.fishlabs.gof2hd2012.dlc4",
    "net.fishlabs.gof2hd2012.dlc5",
};

extern "C" jstring Java_net_fishlabs_googleplay_ToJNI_gof2hd2012getConsumableSKU(
    JNIEnv *env, jclass /*clazz*/, jint index)
{
    if (index < 0 || index > 4)
        return nullptr;
    return env->NewStringUTF(kConsumableSKU[index]);
}

extern "C" jstring Java_net_fishlabs_googleplay_ToJNI_gof2hd2012getPremiumSKU(
    JNIEnv *env, jclass /*clazz*/, jint index)
{
    if (index < 0 || index > 4)
        return nullptr;
    return env->NewStringUTF(kPremiumSKU[index]);
}

// The licensing handshake hands the Java side the expected APK package back as a
// freshly built Java string.
extern "C" jstring Java_net_fishlabs_googleplay_ToJNI_gof2hd2012apk(
    JNIEnv *env, jclass /*clazz*/)
{
    return env->NewStringUTF("net.fishlabs.gof2hdallandroid2012");
}

// ---------------------------------------------------------------------------
// Module / offerwall host hooks.
//   getCurrentApplicationModule  index of the active application module (-1 down).
//   spentAmountOfCredits         the Tapjoy offerwall reward to bank next frame.
// ---------------------------------------------------------------------------

extern "C" jint Java_net_fishlabs_playhaven_ToJNI_getCurrentApplicationModule(
    JNIEnv * /*env*/, jclass /*clazz*/)
{
    return ndk_getCurrentApplicationModule();
}

extern "C" void Java_net_fishlabs_tapjoy_ToJNI_spentAmountOfCredits(
    JNIEnv * /*env*/, jclass /*clazz*/, jint amount)
{
    // Queue the offerwall reward; ndk_checkPlaytimeAndSpendOfferwallCredits
    // banks it on the next frame once the player has logged any play time.
    gb_android_offerwallCreditAmount = amount;
    ndk_checkPlaytimeAndSpendOfferwallCredits();
}

// ---------------------------------------------------------------------------
// Asset bring-up without a separate patch zip: the no-zip sibling of
// InitWithZip. Sets the GL viewport, clears the accelerometer filter state,
// caches the framebuffer size, opens just the APK archive, builds the Engine,
// stamps its version, installs the asset root and app-create/-destroy callbacks
// and forces the canvas into landscape.
// ---------------------------------------------------------------------------

extern "C" int loadAPK(const char *path);

extern "C" void ndk23_Init(const char *apkPath, int width, int height)
{
    glViewport(0, 0, width, height);

    for (int i = 0; i < 6; ++i)
        gAccelFilterState[i] = 0.0;

    gRealWidth = width;
    gRealHeight = height;

    loadAPK(apkPath);

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
// Host-driven viewport resize: re-aims the GL viewport at the new framebuffer
// size, caches it for the render path and replays the current virtual-pointer
// state into the active module so the on-screen controls track the new bounds
// (binary ghidra_addr 0xa3fac).
// ---------------------------------------------------------------------------

extern "C" void ndk23_resize(int width, int height)
{
    glViewport(0, 0, width, height);

    gRealHeight = height;
    gRealWidth = width;

    simulateTouch(gEngine);
}

// ---------------------------------------------------------------------------
// Reserved host hooks. The display geometry is already established through the
// init/resize path, and the host never polls a separate fired-weapon status in
// this build, so both are inert (binary ghidra_addr 0xa3d50 / 0xa3d52).
// ---------------------------------------------------------------------------

extern "C" void ndk23_setDisplayHeightAndWidth(int height, int width)
{
    // Display geometry is owned by InitWithZip/resize; nothing to do here.
    (void)height;
    (void)width;
}

extern "C" int ndk23_getCurrentFiredStatus()
{
    return 0;
}
