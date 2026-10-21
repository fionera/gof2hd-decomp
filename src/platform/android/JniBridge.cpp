

#include <jni.h>
#include <android/log.h>

#include <cstdlib>
#include <cstring>

#include "engine/render/Engine.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include "engine/core/GameData.h"
#include "engine/core/GameText.h"
#include "engine/core/AERandom.h"
#include "engine/file/AEFile.h"
#include "engine/file/FileInterfaceAndroid.h"
#include <GLES2/gl2.h>
#include "game/mission/Status.h"
#include "game/mission/RecordHandler.h"
#include "game/core/Globals.h"
#include "game/core/CheatHandler.h"
#include "game/ui/Layout.h"
#include "game/menu/MGame.h"
#include "game/menu/ModMainMenu.h"
#include "game/menu/ModStation.h"
#include "game/menu/MTitle.h"
#include "platform/android/NdkHooks.h"
#include "engine/input/VirtualInput.h"
#include "engine/core/NFC.h"
#include "engine/render/LODManager.h"

JavaVM *g_pVM;

JNIEnv *g_pEnv;
jobject g_pClass;
jobject g_pActivity;

char *ndk_APK_Path;
char *ndk_ZIP_Path;

extern "C" void ndk_checkPlaytimeAndSpendOfferwallCredits(); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

extern "C" void ndk23_InitWithZip(const char *apkPath, const char *zipPath, // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
                                  int width, int height);

extern "C" void ndk23_setRootDirectory(const char *path); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

extern "C" void ndk23_setZipDirectory(const char *path); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

void ndk23_renderstep(int width, int height);

extern "C" int ndk23_getExitFlag(); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

extern "C" int ndk23_getScreenshotFlag(); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

extern "C" void ndk23_resetScreenshotFlag(); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

extern "C" void ndk23_setCountryCode(unsigned int code); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

extern "C" void ndk23_handleAcceleration(float x, float y, float z); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

int gRealWidth;
int gRealHeight;

char *rootDirectory;
char *ZIPDirectory;
char *flstat_rootPath;

int forceExit;

unsigned int hrwhs_country_code;

int gb_android_offerwallCreditAmount;

int AccIndex;

const char *ndkEntrance;

extern "C" int loadAPKAndZip(const char *apkPath, const char *patchPath); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

void OnCreateApplication(AbyssEngine::Engine * engine);

void OnCreateApplication(AbyssEngine::Engine *engine) {
    using AbyssEngine::Engine;

    GameData *data = new GameData();
    data->field_0x04 = 0;
    data->field_0x0c = 0;
    data->field_0x0e = 0;
    data->field_0x14 = 0;
    data->field_0x10 = -1;
    data->field_0x44 = 0;
    data->field_0x3c = 0;
    data->field_0x40 = 0;
    data->field_0x48 = -1;
    data->field_0x4c = 1;
    data->field_0x08 = 0;
    data->field_0x6d = 0;
    data->field_0x6e = 0;
    data->field_0x6f = 0;
    data->field_0x70 = 0;
    data->field_0x71 = 0;
    data->field_0x72 = 0;
    data->field_0x73 = 0;
    data->field_0x74 = 0;
    data->field_0x75 = 0;
    data->field_0x76 = 0;
    data->field_0x77 = 0;
    data->field_0x50 = 0;
    data->field_0x54 = 0;
    data->field_0x58 = 0;
    data->field_0x5c = 0;
    data->field_0x60 = 0;
    data->field_0x64 = 0;
    data->field_0x68 = 0;
    data->field_0x6c = 0;

    data->globals = new Globals();

    DeviceInfo dev = engine->GetDeviceInfo();

    if (dev.height < 640 || dev.isPad != 0) {
        DeviceInfo padDev = engine->GetDeviceInfo();
        Globals::retinaDisplay = (padDev.isPad != 0) && (static_cast<int>(dev.height) > 800);
    } else {
        Globals::retinaDisplay = true;
    }

    data->field_0xa8 = AbyssEngine::String("", false);
    data->field_0xa5 = 0;
    data->field_0xb8 = AbyssEngine::String("", false);
    data->field_0xc4 = 0;
    data->field_0xb4 = 0;
    data->field_0x78 = 0;
    data->field_0x7a = 0;
    data->field_0x7c = AbyssEngine::String("", false);
    data->field_0x88 = AbyssEngine::String("", false);
    data->field_0x94 = AbyssEngine::String("", false);
    data->field_0xa4 = 0;
    data->field_0xa0 = 0;

    Globals::iPad = (dev.height == 480) && (dev.width == 864);

    engine->GetDeviceInfo();
    Globals::iPadHD = 0;
    Globals::iPad = dev.isPad;
    Globals::iPadLargePossible = 0;
    Globals::iPadLarge = (dev.isPad != 0) ? Globals::retinaDisplay : 0;
    if (dev.isPad != 0) {
        Globals::iPadAssetsWithLowerRes =
                (static_cast<int>(dev.height) < 768) && (static_cast<int>(dev.width) < 1024);
    } else {
        Globals::iPadAssetsWithLowerRes = 0;
    }
    Globals::switch_to_target_setting = -1;
    Globals::enterSpaceLounge = 0;

    BuildResourceList(engine);

    Globals::appManager = engine->appManager;
    Globals::Canvas = engine->appManager->paintCanvas;
    Globals::w = Globals::Canvas->GetWidth();
    Globals::h = Globals::Canvas->GetHeight();

    GameText *text = new GameText();
    Globals::gameText = text;
    text->setLanguage(static_cast<short>(hrwhs_country_code), 3401);
    data->globals->loadFont(GameText::getLanguage());
    data->globals->init(engine->appManager, engine);

    engine->appManager->SetApplicationData(data);
    engine->appManager->SetLoadingCallback(&loadingScreen, Globals::font);

    engine->appManager->RegisterApplicationModule(
        2, reinterpret_cast<AbyssEngine::IApplicationModule *>(new MGame()));
    engine->appManager->RegisterApplicationModule(
        1, reinterpret_cast<AbyssEngine::IApplicationModule *>(new ModMainMenu()));
    engine->appManager->RegisterApplicationModule(
        5, reinterpret_cast<AbyssEngine::IApplicationModule *>(new ModStation()));
    engine->appManager->RegisterApplicationModule(
        0, reinterpret_cast<AbyssEngine::IApplicationModule *>(new MTitle()));

    engine->appManager->CheatSetCallback(&OnCheatActivated, nullptr);
    engine->appManager->CheatAddCode(AbyssEngine::String("754753835", false), 0);
    engine->appManager->CheatAddCode(AbyssEngine::String("448366639", false), 1);
    engine->appManager->CheatAddCode(AbyssEngine::String("373352623", false), 2);

    Globals::layout->initTip();
    Globals::status->resetGame();
    AbyssEngine::Engine::vfc = true;
    Globals::rnd->reset();
    engine->appManager->SetCurrentApplicationModule(0);
    AbyssEngine::Engine::clampTextures = false;

    engine->appManager->paintCanvas->TextureCreateGlobal(
        AbyssEngine::String("data/textures/pow_texture.aei", false), 2);

    AbyssEngine::Engine::vboSupported = true;
    AbyssEngine::Engine::clampTextures = false;
    engine->field_0x74 = false;
    AbyssEngine::Engine::lodBiasNormal = -0.5f;
    AbyssEngine::Engine::lodBiasDiffuse = -1.3f;
}

extern "C" void ndk23_setRootDirectory(const char *path) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    rootDirectory = static_cast<char *>(std::malloc(std::strlen(path) + 1));
    std::strcpy(rootDirectory, path);
}

extern "C" void ndk23_setZipDirectory(const char *path) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    ZIPDirectory = static_cast<char *>(std::malloc(std::strlen(path) + 1));
    std::strcpy(ZIPDirectory, path);
}

extern "C" void ndk23_setCountryCode(unsigned int code) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    AbyssEngine::Engine::countryCode = (code < 16) ? code : 0u;
}

extern "C" int ndk23_getExitFlag() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    return forceExit;
}

extern "C" int ndk23_getScreenshotFlag() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    return 0;
}

extern "C" void ndk23_resetScreenshotFlag() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
}

extern "C" void ndk_checkPlaytimeAndSpendOfferwallCredits() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    if (Globals::status->getPlayingTime() >= 1) {
        int amount = gb_android_offerwallCreditAmount;
        if (amount > 0) {
            Globals::status->changeCredits(amount);
            ndk_autosave();
            gb_android_offerwallCreditAmount = 0;
        }
    }
}

// Low-pass filtered gravity and its negation as handed to the engine.
static double gGravity[3];
static double gNegGravity[3];

extern "C" void ndk23_InitWithZip(const char *apkPath, const char *zipPath, // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
                                  int width, int height) {
    ndkEntrance = "ndkInit";
    glViewport(0, 0, width, height);
    gGravity[0] = 0.0;
    gGravity[1] = 0.0;
    gGravity[2] = 0.0;
    gRealHeight = width;
    gRealWidth = height;
    loadAPKAndZip(apkPath, zipPath);

    gEngine = new AbyssEngine::Engine();
    gEngine->str_0x3c = String("2.0.16", false);
    gEngine->InitGL(true, width, height);
    if (rootDirectory != nullptr)
        AEFile::SetAppRootDir(rootDirectory);
    gEngine->Initialize(&OnCreateApplication);
    gEngine->SetOnDestroyApp(&OnDestroyApplication);
    gEngine->appManager->paintCanvas->SetGameOrientation(AbyssEngine::LandscapeMode_2);
}

int rotateAccelValues;

extern "C" void ndk23_handleAcceleration(float x, float y, float z) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    double ax = x;
    double ay = y;
    double tiltA, tiltB;
    if (rotateAccelValues != 0) {
        tiltA = -ax;
        tiltB = ay;
    } else {
        tiltA = ay;
        tiltB = ax;
    }
    if (gEngine == nullptr)
        return;

    double az = z;
    double accelX = -tiltB;
    double accelY = -tiltA;
    double accelZ = -az;
    if (gEngine->appManager->paintCanvas->initialized) {
        gGravity[0] = gGravity[0] * 0.95 + tiltB * 0.05;
        gGravity[1] = gGravity[1] * 0.95 + tiltA * 0.05;
        gGravity[2] = gGravity[2] * 0.95 + az * 0.05;
    } else {
        gGravity[0] = tiltB * 0.95;
        gGravity[1] = tiltB * 0.95;
        gGravity[2] = tiltB * 0.95;
    }
    gNegGravity[0] = -gGravity[0];
    gNegGravity[1] = -gGravity[1];
    gNegGravity[2] = -gGravity[2];
    gEngine->SetAccelValue(accelX, accelY, accelZ);
    gEngine->SetGravValue(gNegGravity[0], gNegGravity[1], gNegGravity[2]);
}

extern "C" jint JNI_OnLoad(JavaVM *vm, void * /*reserved*/) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI) // lint: void_ptr (JNI_OnLoad reserved param, baked JNI ABI)
    // lint: void_ptr (JNI_OnLoad reserved param, baked JNI ABI)
    JNIEnv *env = nullptr;
    g_pVM = vm;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4) != JNI_OK) // lint: void_ptr (GetEnv requires void** out-param, JNI ABI)
        // lint: void_ptr (GetEnv requires void** out-param, JNI ABI)
        return -1;
    if (env->FindClass("net/fishlabs/gof2hdallandroid2012/GOF2HD2012") == nullptr)
        return -1;
    return JNI_VERSION_1_4;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_setEnvironmentVariables( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv *env, jclass clazz, jobject activity) {
    g_pClass = clazz;
    g_pEnv = env;
    g_pActivity = activity;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_setAPKPath( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv *env, jclass /*clazz*/, jstring path) {
    jboolean isCopy;
    const char *utf = env->GetStringUTFChars(path, &isCopy);
    ndk_APK_Path = static_cast<char *>(std::malloc(std::strlen(utf) + 1));
    if (isCopy) {
        std::strcpy(ndk_APK_Path, utf);
        env->ReleaseStringUTFChars(path, utf);
    }
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_setZIPPath( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv *env, jclass /*clazz*/, jstring path) {
    jboolean isCopy;
    const char *utf = env->GetStringUTFChars(path, &isCopy);
    ndk_ZIP_Path = static_cast<char *>(std::malloc(std::strlen(utf) + 1));
    if (isCopy) {
        std::strcpy(ndk_ZIP_Path, utf);
        env->ReleaseStringUTFChars(path, utf);
    }
}

char *dataDirectory;
char *zipDirectory;

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_SetDirectories( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv *env, jclass /*clazz*/, jstring rootDir, jstring zipDir) {
    jboolean rootCopied;
    const char *root = env->GetStringUTFChars(rootDir, &rootCopied);
    size_t rootLength = std::strlen(root) + 1;
    dataDirectory = static_cast<char *>(std::malloc(rootLength));
    flstat_rootPath = static_cast<char *>(std::malloc(rootLength));
    if (rootCopied) {
        std::strcpy(dataDirectory, root);
        std::strcpy(flstat_rootPath, root);
        ndk23_setRootDirectory(dataDirectory);
        __android_log_print(ANDROID_LOG_ERROR, "GOF2_NATIVE", "dataDirectory in main.c: %s", dataDirectory);
        __android_log_print(ANDROID_LOG_ERROR, "GOF2_NATIVE", "rootPath in main.c: %s", flstat_rootPath);
        env->ReleaseStringUTFChars(rootDir, root);
    }

    jboolean zipCopied;
    const char *zip = env->GetStringUTFChars(zipDir, &zipCopied);
    zipDirectory = static_cast<char *>(std::malloc(std::strlen(zip) + 1));
    if (zipCopied) {
        std::strcpy(zipDirectory, zip);
        ndk23_setZipDirectory(zipDirectory);
        __android_log_print(ANDROID_LOG_ERROR, "GOF2_NATIVE", "zipDirectory in main.c: %s", zipDirectory);
        env->ReleaseStringUTFChars(zipDir, zip);
    }
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_STARTUP( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_initialize( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/, jint width, jint height) {
    ndk23_InitWithZip(ndk_APK_Path, ndk_ZIP_Path, width, height);
    ndk23_renderstep(width, height);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_renderstep( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/, jint width, jint height) {
    ndk23_renderstep(width, height);
}

extern "C" jboolean Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getExitFlag( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return ndk23_getExitFlag() != 0;
}

extern "C" jboolean Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getScreenshotFlag( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return ndk23_getScreenshotFlag() != 0;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_resetScreenshotFlag( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    ndk23_resetScreenshotFlag();
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_sendPauseSignalToGame( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    ndk23_sendingPauseSignal();
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_sendResumeSignalToGame( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    ndk23_sendingResumeSignal();
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_setCountryCodeOfDevice( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/, jint code) {
    ndk23_setCountryCode(static_cast<unsigned int>(code));
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_handleAccelerometer( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/, jfloat x, jfloat y, jfloat z) {
    ndk23_handleAcceleration(-x, y, z);
}

extern "C" void ExitFunction() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    forceExit = -1;
}

extern "C" void ndk23_newrender(long long now) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    gEngine->appManager->SetExitCallback(&ExitFunction);

    int touchCount = GetTouchCount();
    for (int i = 0; i < touchCount; ++i) {
        // Touch record layout: {handle, phase, x, y}.
        Touch rec = GetTouch(i);
        void *touch = reinterpret_cast<void *>(rec.x); // lint: void_ptr (opaque touch handle for OnTouch* void* params, baked ABI)
        if (rec.y == 2) {
            gEngine->appManager->OnTouchMove(rec.id, rec.action, touch);
        } else if (rec.y == 1) {
            gEngine->appManager->OnTouchEnd(rec.id, rec.action, touch);
            gEngine->appManager->OnTouchEnd();
        } else if (rec.y == 0) {
            gEngine->appManager->OnTouchBegin(rec.id, rec.action, touch);
        }
    }

    if (g_android_back_button_pressed != 0) {
        keyPressed(gEngine, 0x35);
        keyReleased(gEngine, 0x35);
        g_android_back_button_pressed = 0;
    }

    RemoveTouches();
    gEngine->appManager->OnUpdate(now);

    char *appData = static_cast<char *>(gEngine->appManager->GetApplicationData());
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

extern "C" void ndk23_handleTouchPadEvent(jclass /*clazz*/, void *touch, int phase, // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI) // lint: void_ptr (ndk* touch handle param, baked ABI)
                                          float x, float y) {
    if (phase == 2) {
        gEngine->appManager->OnTouchMove(static_cast<int>(x), static_cast<int>(y), touch);
    } else if (phase == 1) {
        gEngine->appManager->OnTouchEnd(static_cast<int>(x), static_cast<int>(y), touch);
        gEngine->appManager->OnTouchEnd();
    } else if (phase == 0) {
        gEngine->appManager->OnTouchBegin(static_cast<int>(x), static_cast<int>(y), touch);
    }
}

extern "C" void ndk23_handleTouchScreenEvent(jclass clazz, void *touch, int phase, // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI) // lint: void_ptr (ndk* touch handle param, baked ABI)
                                             // lint: void_ptr (ndk* touch handle param, baked ABI)
                                             float x, float y) {
    ndk23_handleTouchPadEvent(clazz, touch, phase, x, y);
}

extern "C" void ndk23_ndkDone() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    if (gEngine != nullptr) {
        gEngine->Release();
        delete gEngine;
        gEngine = nullptr;
    }
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC1BOUGHT( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return ndk_getDLC_1_BOUGHT();
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC2BOUGHT( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return ndk_getDLC_2_BOUGHT();
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC1( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    ndk_iapBoughtPremium(0, 1);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC2( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    ndk_iapBoughtPremium(1, 1);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC3( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    ndk_iapBoughtPremium(2, 1);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC4( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    ndk_iapBoughtPremium(3, 1);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC5( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    ndk_iapBoughtPremium(4, 1);
}

int g_android_current_achievements[3];
int g_android_leaderboard_scores[8];
int g_android_link_game_gp;
int g_android_gp_is_linked;
int g_android_show_achievements;
int g_android_show_leaderboards;

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetAchievementId( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/, jint index) {
    return g_android_current_achievements[index];
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetLeaderboardScore( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/, jint index) {
    return g_android_leaderboard_scores[index];
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetLeaderboardScore( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/, jint index) {
    g_android_leaderboard_scores[index] = 0;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetAchievements( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    g_android_current_achievements[0] = 0;
    g_android_current_achievements[1] = 0;
    g_android_current_achievements[2] = 0;
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetShowAchievements( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return g_android_show_achievements;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetShowAchievements( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    g_android_show_achievements = 0;
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetLinkGameGP( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return g_android_link_game_gp;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetLinkGameGP( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    g_android_link_game_gp = 0;
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetShowLeaderboards( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return g_android_show_leaderboards;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetShowLeaderboards( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    g_android_show_leaderboards = 0;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_SetGPIsLinked( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/, jint linked) {
    g_android_gp_is_linked = linked;
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC3BOUGHT( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return ndk_getDLC_3_BOUGHT();
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC4BOUGHT( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return ndk_getDLC_4_BOUGHT();
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC5BOUGHT( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return ndk_getDLC_5_BOUGHT();
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getLogoShown( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return ndk_getLogoShown();
}

extern "C" jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_isInMainMenu( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return ndk_isInMainMenu();
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_resize( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/, jint width, jint height) {
    ndk23_renderstep(width, height);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_handleTouchEvent( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass clazz, jint touch, jint phase, jfloat x, jfloat y) {
    ndk23_handleTouchScreenEvent(clazz, reinterpret_cast<void *>(touch), phase, x, y); // lint: void_ptr (opaque touch handle for ndk* void* param, baked ABI)
    // lint: void_ptr (opaque touch handle for ndk* void* param, baked ABI)
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_testPurchase( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    ndk_iapBoughtConsumable(0);
}

extern "C" void Java_net_fishlabs_googleplay_ToJNI_iapBoughtConsumable( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/, jint consumable) {
    ndk_iapBoughtConsumable(static_cast<unsigned int>(consumable));
}

extern "C" void Java_net_fishlabs_googleplay_ToJNI_iapBoughtPremium( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/, jint pack, jint bought) {
    ndk_iapBoughtPremium(static_cast<unsigned int>(pack),
                         static_cast<unsigned int>(bought));
}

extern "C" void Java_net_fishlabs_googleplay_ToJNI_iapSetNativeItemInformationList( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv *env, jclass clazz, jobjectArray ids, jobjectArray names,
    jobjectArray descriptions, jobjectArray currencies, jobjectArray prices) {
    ndk_setNativeItemInformationList(env, clazz, ids, names, descriptions,
                                     currencies, prices);
}

extern "C" void Java_net_fishlabs_googleplay_ToJNI_iapResetNativeItemInformationList( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    ndk_resetNativeItemInformationList();
}

extern "C" jstring Java_net_fishlabs_googleplay_ToJNI_gof2hd2012getConsumableSKU( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv *env, jclass /*clazz*/, jint index) {
    switch (index) {
        case 0: return env->NewStringUTF("iap_managed_credits_100_000");
        case 1: return env->NewStringUTF("iap_managed_credits_300_000");
        case 2: return env->NewStringUTF("iap_managed_credits_1_000_000");
        case 3: return env->NewStringUTF("iap_managed_credits_3_000_000");
        case 4: return env->NewStringUTF("iap_managed_10_000_000");
    }
    return nullptr;
}

extern "C" jstring Java_net_fishlabs_googleplay_ToJNI_gof2hd2012getPremiumSKU( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv *env, jclass /*clazz*/, jint index) {
    switch (index) {
        case 0: return env->NewStringUTF("iap_managed_valkyrie");
        case 1: return env->NewStringUTF("iap_managed_kaamo_club");
        case 2: return env->NewStringUTF("iap_managed_supernova");
        case 3: return env->NewStringUTF("iap_managed_vip");
        case 4: return env->NewStringUTF("iap_managed_full_package");
    }
    return nullptr;
}

extern "C" jstring Java_net_fishlabs_googleplay_ToJNI_gof2hd2012apk( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv *env, jclass /*clazz*/) {
    return env->NewStringUTF("net.fishlabs.gof2hdallandroid2012");
}

extern "C" jint Java_net_fishlabs_playhaven_ToJNI_getCurrentApplicationModule( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/) {
    return ndk_getCurrentApplicationModule();
}

extern "C" void Java_net_fishlabs_tapjoy_ToJNI_spentAmountOfCredits( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv * /*env*/, jclass /*clazz*/, jint amount) {
    gb_android_offerwallCreditAmount = amount;
    ndk_checkPlaytimeAndSpendOfferwallCredits();
}

extern "C" int loadAPK(const char *path); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

extern "C" void ndk23_Init(const char *apkPath, int width, int height) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    ndkEntrance = "ndkInit";
    glViewport(0, 0, width, height);
    gGravity[0] = 0.0;
    gGravity[1] = 0.0;
    gGravity[2] = 0.0;
    gRealHeight = width;
    gRealWidth = height;
    loadAPK(apkPath);

    gEngine = new AbyssEngine::Engine();
    gEngine->str_0x3c = String("2.0.16", false);
    gEngine->InitGL(true, width, height);
    if (rootDirectory != nullptr)
        AEFile::SetAppRootDir(rootDirectory);
    gEngine->Initialize(&OnCreateApplication);
    gEngine->SetOnDestroyApp(&OnDestroyApplication);
    gEngine->appManager->paintCanvas->SetGameOrientation(AbyssEngine::LandscapeMode_2);
}

extern "C" void ndk23_resize(int width, int height) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    glViewport(0, 0, width, height);

    gRealWidth = width;
    gRealHeight = height;

    simulateTouch(gEngine);
}

extern "C" void ndk23_setDisplayHeightAndWidth(int height, int width) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    (void) height;
    (void) width;
}

extern "C" int ndk23_getCurrentFiredStatus() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    return 0;
}
