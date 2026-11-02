#include "platform/android/NdkHooks.h"

#include <GLES2/gl2.h>

#include <cstdlib>
#include <cstdint>
#include <cstring>

#include "game/mission/Status.h"
#include "game/mission/RecordHandler.h"
#include "engine/core/ApplicationManager.h"
#include "engine/core/GameData.h"
#include "engine/core/IApplicationModule.h"
#include "engine/render/Engine.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/LODManager.h"
#include "engine/audio/FModSound.h"
#include "engine/file/AEFile.h"
#include "engine/file/FileInterfaceAndroid.h"
#include "engine/input/VirtualInput.h"
#include "engine/core/NFC.h"
#include "game/core/Globals.h"

// Android NDK entry layer (original TU starting at GetEngine, .text 0x93c88..0x950dc): the
// ndk23_* lifecycle/input hooks the Java_* wrappers in main.c tail-call, and the IAP glue.

using AbyssEngine::Engine;

// .bss order is the original's (nm -D witnesses at 0x217b20..0x217b58): the fourteen 4-byte
// globals first, then the two double[3] gravity tables.  The gi_iap_* flags live in
// GOF2HD2012.c (original .bss 0x2179b4..0x2179dc, between main.c and FileInterfaceAndroid).
int sound_off;

AbyssEngine::Engine *gEngine = nullptr;

int rotateAccelValues;

struct zip *APKArchive = nullptr;
struct zip *ZIPArchive = nullptr;

unsigned int hrwhs_country_code;

int gRealWidth;
int gRealHeight;

char *rootDirectory;
char *ZIPDirectory;

int forceExit;

int stateChanged;
int currentstate;

int gb_android_offerwallCreditAmount;

const char *ndkEntrance = "0";


// Low-pass filtered gravity and its negation as handed to the engine.
static double gGravity[3];
static double gNegGravity[3];

// Free accessor for the global engine instance (original TU starts here at .text 0x93c88).
Engine *GetEngine() {
    return gEngine;
}

extern "C" void ExitFunction() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    forceExit = -1;
}

extern "C" int ndk23_getScreenshotFlag() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    return 0;
}

extern "C" void ndk23_resetScreenshotFlag() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
}

extern "C" int ndk23_getExitFlag() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    return forceExit;
}

extern "C" int loadAPK(const char *path) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    struct zip *za = zip_open(path, 0, 0);
    APKArchive = za;
    if (za == 0)
        return 0;

    int count = zip_get_num_files(za);
    for (int i = 0; i < count; ++i) {
        if (zip_get_name(APKArchive, i, 0) == 0)
            break;
    }
    return 1;
}

extern "C" int loadAPKAndZip(const char *apkPath, const char *patchPath) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    APKArchive = zip_open(apkPath, 0, 0);
    ZIPArchive = zip_open(patchPath, 0, 0);
    return APKArchive != 0 && ZIPArchive != 0;
}

extern "C" void ndk23_setDisplayHeightAndWidth(int height, int width) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    (void) height;
    (void) width;
}

extern "C" int ndk23_getCurrentFiredStatus() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    return 0;
}

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

extern "C" void ndk23_InitWithZip(const char *apkPath, const char *zipPath, // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
                                  int width, int height, float /*scale*/) {
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

extern "C" void ndk23_resize(int width, int height) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    glViewport(0, 0, width, height);

    gRealWidth = width;
    gRealHeight = height;

    simulateTouch(gEngine);
}

extern "C" void ndk23_ndkDone() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    if (gEngine != nullptr) {
        gEngine->Release();
        delete gEngine;
        gEngine = nullptr;
    }
}

// The original forwards its four core-register arguments unchanged to AddTouch (b.w AddTouch).
extern "C" void ndk23_handleTouchScreenEvent(int x, int y, int id, int action) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    AddTouch(x, y, id, action);
}

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
    double az = z;
    double accelX = -tiltB;
    double accelY = -tiltA;
    double accelZ = -az;
    if (gEngine == nullptr)
        return;
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

extern "C" void ndk23_newrender(long long now) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    gEngine->appManager->SetExitCallback(&ExitFunction);

    int touchCount = GetTouchCount();
    for (int i = 0; i < touchCount; ++i) {
        // Queued touch: Touch::x carries the handle, Touch::y the phase, id/action the position.
        Touch rec = GetTouch(i);
        const int x = rec.id;
        const int y = rec.action;
        void *touch = reinterpret_cast<void *>(rec.x); // lint: void_ptr (opaque touch handle for OnTouch* void* params, baked ABI)
        if (rec.y == 0) {
            gEngine->appManager->OnTouchBegin(x, y, touch);
        } else if (rec.y == 1) {
            gEngine->appManager->OnTouchEnd(x, y, touch);
            gEngine->appManager->OnTouchEnd();
        } else if (rec.y == 2) {
            gEngine->appManager->OnTouchMove(x, y, touch);
        }
    }

    if (g_android_back_button_pressed != 0) {
        keyPressed(gEngine, 0x35);
        keyReleased(gEngine, 0x35);
        g_android_back_button_pressed = 0;
    }

    RemoveTouches();
    gEngine->appManager->OnUpdate(now);

    GameData *data = static_cast<GameData *>(gEngine->appManager->GetApplicationData());
    if (data->field_0x3d != 0) {
        data->field_0x3d = 0;
        data->field_0x40 = 1;
    } else if (data->field_0x3c != 0) {
        data->field_0x3c = 0;
    } else if (data->field_0x3e != 0) {
        data->field_0x3e = 0;
    } else if (data->field_0x3f != 0) {
        data->field_0x3f = 0;
    } else if (data->field_0xa0 != 0) {
        data->field_0xa0 = 0;
    } else if (data->field_0xa1 != 0) {
        data->field_0xa1 = 0;
    } else if (data->field_0xa2 != 0) {
        data->field_0xa2 = 0;
    } else if (data->field_0xa3 != 0) {
        data->field_0xa3 = 0;
    }

    ndk_checkPlaytimeAndSpendOfferwallCredits();
}

extern "C" void ndk23_handleTouchPadEvent(jclass /*clazz*/, void *touch, int phase, // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI) // lint: void_ptr (ndk* touch handle param, baked ABI)
                                          float x, float y) {
    // phase: 0 = down, 1 = up, 2 = move.
    if (phase == 0) {
        gEngine->appManager->OnTouchBegin(static_cast<int>(x), static_cast<int>(y), touch);
    } else if (phase == 1) {
        gEngine->appManager->OnTouchEnd(static_cast<int>(x), static_cast<int>(y), touch);
        gEngine->appManager->OnTouchEnd();
    } else if (phase == 2) {
        gEngine->appManager->OnTouchMove(static_cast<int>(x), static_cast<int>(y), touch);
    }
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

extern "C" { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
void ndk23_sendingPauseSignal() {
    if (gEngine == nullptr)
        return;
    Globals::sound->pauseAll();
    gEngine->appManager->currentModule->OnSuspend();
}

void ndk23_sendingResumeSignal() {
    if (gEngine == nullptr)
        return;
    Globals::sound->resumeAll();
}

void ndk_iapBoughtPremium(unsigned int pack, unsigned int bought) {
    if (gEngine == nullptr)
        return;

    bool owned = false;
    if (pack > 4 || bought > 1)
        return;
    if (bought == 1)
        owned = true;

    switch (pack) {
    case 0: // Valkyrie
        if (gi_iap_buy_dlc1_pressed != 0)
            gi_iap_buy_dlc1_pressed = 0;
        Globals::options[0x35] = owned;
        Globals::status->setSystemVisibility(25, true);
        break;
    case 1: // Kaamo Club
        if (gi_iap_buy_dlc2_pressed != 0)
            gi_iap_buy_dlc2_pressed = 0;
        Globals::options[0x36] = owned;
        Globals::status->field_114 = 3;
        break;
    case 2: // Supernova
        if (gi_iap_buy_dlc3_pressed != 0)
            gi_iap_buy_dlc3_pressed = 0;
        Globals::options[0x37] = owned;
        Globals::status->setSystemVisibility(25, true);
        break;
    case 3: // VIP
        if (gi_iap_buy_dlc4_pressed != 0)
            gi_iap_buy_dlc4_pressed = 0;
        Globals::options[0x38] = owned;
        break;
    case 4: // full package
        if (gi_iap_buy_dlc5_pressed != 0)
            gi_iap_buy_dlc5_pressed = 0;
        Globals::options[0x39] = owned;
        Globals::options[0x35] = owned;
        Globals::options[0x37] = owned;
        Globals::status->setSystemVisibility(25, true);
        break;
    }
}

int ndk_getDLC_1_BOUGHT() {
    return gEngine != nullptr ? Globals::options[0x35] : 0;
}

int ndk_getDLC_2_BOUGHT() {
    return gEngine != nullptr ? Globals::options[0x36] : 0;
}

int ndk_getDLC_3_BOUGHT() {
    return gEngine != nullptr ? Globals::options[0x37] : 0;
}

int ndk_getDLC_4_BOUGHT() {
    return gEngine != nullptr ? Globals::options[0x38] : 0;
}

int ndk_getDLC_5_BOUGHT() {
    return gEngine != nullptr ? Globals::options[0x39] : 0;
}

void ndk_iapBoughtConsumable(unsigned int consumable) {
    if (consumable > 4 || gEngine == nullptr)
        return;

    // setBaughtCredits returns bool: the zeroext return keeps these calls out of tail position,
    // so the five cases share one call.
    switch (consumable) {
    case 0:
        setBaughtCredits(100000);
        break;
    case 1:
        setBaughtCredits(300000);
        break;
    case 2:
        setBaughtCredits(1000000);
        break;
    case 3:
        setBaughtCredits(3000000);
        break;
    case 4:
        setBaughtCredits(10000000);
        break;
    }
}

bool setBaughtCredits(int amount) {
    int *packFlag;
    if (amount == 100000) {
        Globals::status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack1_pressed;
    } else if (amount == 300000) {
        Globals::status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack2_pressed;
    } else if (amount == 1000000) {
        Globals::status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack3_pressed;
    } else if (amount == 3000000) {
        Globals::status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack4_pressed;
    } else if (amount == 10000000) {
        Globals::status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack5_pressed;
    } else {
        return false;
    }

    if (*packFlag != 0) {
        *packFlag = 0;
        checkFirstCreditPackBoughtWriteAction();
    }
    ndk_autosave();
    return true;
}

void checkFirstCreditPackBoughtWriteAction() {
    Globals::status->getCurrentCampaignMission();
    if (Globals::options[0x62] == 0) {
        Globals::options[0x62] = 1;
        RecordHandler handler;
        handler.saveOptions();
    }
}

// The original NdkHooks TU owns both checkFirstCreditPackBoughtWriteAction and ndk_autosave:
// its .dynsym first-mention order is saveOptions, recordStoreWrite, recordStoreWritePreview,
// C1, D1.  ndk_checkPlaytime... is defined after ndk_autosave so clang emits ndk_autosave at
// its definition (0x94770, after checkFirst), not at its first reference (0x942aa).
void ndk_autosave() {
    RecordHandler *handler = new RecordHandler;
    handler->recordStoreWrite(0);
    handler->recordStoreWritePreview(0);
    delete handler;
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

int ndk_getCurrentApplicationModule() {
    if (gEngine == nullptr)
        return -1;
    return static_cast<int>(Globals::appManager->GetCurrentApplicationModule());
}

int ndk_getLogoShown() {
    return Globals::logoIsShown;
}

int ndk_isInMainMenu() {
    return Globals::isInMainMenu;
}
}
