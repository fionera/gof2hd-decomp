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

#include <cstdlib>
#include <cstring>

#include "engine/render/Engine.h"

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

// Engine accelerometer/gravity sinks.
void Engine_SetAccelValue(AbyssEngine::Engine *self, double x, double y, double z)
    asm("_ZN11AbyssEngine6Engine13SetAccelValueEddd");
void Engine_SetGravValue(AbyssEngine::Engine *self, double x, double y, double z)
    asm("_ZN11AbyssEngine6Engine12SetGravValueEddd");

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
