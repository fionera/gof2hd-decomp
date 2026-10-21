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

int gRealWidth;
int gRealHeight;

char *rootDirectory;
char *ZIPDirectory;

int forceExit;

unsigned int hrwhs_country_code;

int gb_android_offerwallCreditAmount;

const char *ndkEntrance = "0";

int rotateAccelValues;

int gi_iap_buy_dlc1_pressed;
int gi_iap_buy_dlc2_pressed;
int gi_iap_buy_dlc3_pressed;
int gi_iap_buy_dlc4_pressed;
int gi_iap_buy_dlc5_pressed;

int gi_iap_buy_credit_pack1_pressed;
int gi_iap_buy_credit_pack2_pressed;
int gi_iap_buy_credit_pack3_pressed;
int gi_iap_buy_credit_pack4_pressed;
int gi_iap_buy_credit_pack5_pressed;

// Low-pass filtered gravity and its negation as handed to the engine.
static double gGravity[3];
static double gNegGravity[3];

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

void ndk_autosave() {
    RecordHandler *handler = new RecordHandler;
    handler->recordStoreWrite(0);
    handler->recordStoreWritePreview(0);
    delete handler;
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

char *pConstToNonConst(const char *s) {
    if (s != nullptr) {
        char *copy = static_cast<char *>(std::malloc(std::strlen(s) + 1));
        if (copy != nullptr) {
            std::strcpy(copy, s);
            return copy;
        }
    }
    return nullptr;
}

const char *getStringUTFChars(JNIEnv *env, jstring str, jboolean *isCopy) {
    if (env != nullptr && str != nullptr)
        return env->GetStringUTFChars(str, isCopy);
    return nullptr;
}

void releaseStringUTFChars(JNIEnv *env, jstring str, const char *chars) {
    if (env != nullptr && str != nullptr && chars != nullptr) {
        env->ReleaseStringUTFChars(str, chars);
        return;
    }
    if (chars != nullptr) {
        operator delete(const_cast<char *>(chars));
    }
    if (str != nullptr) {
        operator delete(reinterpret_cast<void *>(str)); // lint: void_ptr NDK boundary, operator delete ABI requires void*
        // lint: void_ptr NDK boundary, operator delete ABI requires void*
    }
}

void ndk_setNativeItemInformationList(JNIEnv *env, jclass /*clazz*/,
                                      jobjectArray ids, jobjectArray names,
                                      jobjectArray descriptions,
                                      jobjectArray currencies,
                                      jobjectArray prices) {
    if (env == nullptr)
        return;

    jstring id0 = static_cast<jstring>(env->GetObjectArrayElement(ids, 0));
    jstring id1 = static_cast<jstring>(env->GetObjectArrayElement(ids, 1));
    jstring id2 = static_cast<jstring>(env->GetObjectArrayElement(ids, 2));
    jstring id3 = static_cast<jstring>(env->GetObjectArrayElement(ids, 3));
    jstring id4 = static_cast<jstring>(env->GetObjectArrayElement(ids, 4));

    jstring name0 = static_cast<jstring>(env->GetObjectArrayElement(names, 0));
    jstring name1 = static_cast<jstring>(env->GetObjectArrayElement(names, 1));
    jstring name2 = static_cast<jstring>(env->GetObjectArrayElement(names, 2));
    jstring name3 = static_cast<jstring>(env->GetObjectArrayElement(names, 3));
    jstring name4 = static_cast<jstring>(env->GetObjectArrayElement(names, 4));

    jstring description0 = static_cast<jstring>(env->GetObjectArrayElement(descriptions, 0));
    jstring description1 = static_cast<jstring>(env->GetObjectArrayElement(descriptions, 1));
    jstring description2 = static_cast<jstring>(env->GetObjectArrayElement(descriptions, 2));
    jstring description3 = static_cast<jstring>(env->GetObjectArrayElement(descriptions, 3));
    jstring description4 = static_cast<jstring>(env->GetObjectArrayElement(descriptions, 4));

    jstring currency0 = static_cast<jstring>(env->GetObjectArrayElement(currencies, 0));
    jstring currency1 = static_cast<jstring>(env->GetObjectArrayElement(currencies, 1));
    jstring currency2 = static_cast<jstring>(env->GetObjectArrayElement(currencies, 2));
    jstring currency3 = static_cast<jstring>(env->GetObjectArrayElement(currencies, 3));
    jstring currency4 = static_cast<jstring>(env->GetObjectArrayElement(currencies, 4));

    jstring price0 = static_cast<jstring>(env->GetObjectArrayElement(prices, 0));
    jstring price1 = static_cast<jstring>(env->GetObjectArrayElement(prices, 1));
    jstring price2 = static_cast<jstring>(env->GetObjectArrayElement(prices, 2));
    jstring price3 = static_cast<jstring>(env->GetObjectArrayElement(prices, 3));
    jstring price4 = static_cast<jstring>(env->GetObjectArrayElement(prices, 4));

    const char *idUtf0 = env->GetStringUTFChars(id0, nullptr);
    const char *idUtf1 = env->GetStringUTFChars(id1, nullptr);
    const char *idUtf2 = env->GetStringUTFChars(id2, nullptr);
    const char *idUtf3 = env->GetStringUTFChars(id3, nullptr);
    const char *idUtf4 = env->GetStringUTFChars(id4, nullptr);

    const char *nameUtf0 = env->GetStringUTFChars(name0, nullptr);
    const char *nameUtf1 = env->GetStringUTFChars(name1, nullptr);
    const char *nameUtf2 = env->GetStringUTFChars(name2, nullptr);
    const char *nameUtf3 = env->GetStringUTFChars(name3, nullptr);
    const char *nameUtf4 = env->GetStringUTFChars(name4, nullptr);

    const char *descriptionUtf0 = env->GetStringUTFChars(description0, nullptr);
    const char *descriptionUtf1 = env->GetStringUTFChars(description1, nullptr);
    const char *descriptionUtf2 = env->GetStringUTFChars(description2, nullptr);
    const char *descriptionUtf3 = env->GetStringUTFChars(description3, nullptr);
    const char *descriptionUtf4 = env->GetStringUTFChars(description4, nullptr);

    const char *currencyUtf0 = env->GetStringUTFChars(currency0, nullptr);
    const char *currencyUtf1 = env->GetStringUTFChars(currency1, nullptr);
    const char *currencyUtf2 = env->GetStringUTFChars(currency2, nullptr);
    const char *currencyUtf3 = env->GetStringUTFChars(currency3, nullptr);
    const char *currencyUtf4 = env->GetStringUTFChars(currency4, nullptr);

    const char *priceUtf0 = env->GetStringUTFChars(price0, nullptr);
    const char *priceUtf1 = env->GetStringUTFChars(price1, nullptr);
    const char *priceUtf2 = env->GetStringUTFChars(price2, nullptr);
    const char *priceUtf3 = env->GetStringUTFChars(price3, nullptr);
    const char *priceUtf4 = env->GetStringUTFChars(price4, nullptr);

    Globals::cItemListID_00 = pConstToNonConst(idUtf0);
    Globals::cItemListID_01 = pConstToNonConst(idUtf1);
    Globals::cItemListID_02 = pConstToNonConst(idUtf2);
    Globals::cItemListID_03 = pConstToNonConst(idUtf3);
    Globals::cItemListID_04 = pConstToNonConst(idUtf4);

    Globals::cItemListName_00 = pConstToNonConst(nameUtf0);
    Globals::cItemListName_01 = pConstToNonConst(nameUtf1);
    Globals::cItemListName_02 = pConstToNonConst(nameUtf2);
    Globals::cItemListName_03 = pConstToNonConst(nameUtf3);
    // The shipped code copies the fourth name into the fifth slot as well.
    Globals::cItemListName_04 = pConstToNonConst(nameUtf3);

    Globals::cItemListDescription_00 = pConstToNonConst(descriptionUtf0);
    Globals::cItemListDescription_01 = pConstToNonConst(descriptionUtf1);
    Globals::cItemListDescription_02 = pConstToNonConst(descriptionUtf2);
    Globals::cItemListDescription_03 = pConstToNonConst(descriptionUtf3);
    Globals::cItemListDescription_04 = pConstToNonConst(descriptionUtf4);

    Globals::cItemListCurrency_00 = pConstToNonConst(currencyUtf0);
    Globals::cItemListCurrency_01 = pConstToNonConst(currencyUtf1);
    Globals::cItemListCurrency_02 = pConstToNonConst(currencyUtf2);
    Globals::cItemListCurrency_03 = pConstToNonConst(currencyUtf3);
    Globals::cItemListCurrency_04 = pConstToNonConst(currencyUtf4);

    Globals::cItemListPrice_00 = pConstToNonConst(priceUtf0);
    Globals::cItemListPrice_01 = pConstToNonConst(priceUtf1);
    Globals::cItemListPrice_02 = pConstToNonConst(priceUtf2);
    Globals::cItemListPrice_03 = pConstToNonConst(priceUtf3);
    Globals::cItemListPrice_04 = pConstToNonConst(priceUtf4);

    releaseStringUTFChars(env, id0, idUtf0);
    releaseStringUTFChars(env, id1, idUtf1);
    releaseStringUTFChars(env, id2, idUtf2);
    releaseStringUTFChars(env, id3, idUtf3);
    releaseStringUTFChars(env, id4, idUtf4);
    releaseStringUTFChars(env, name0, nameUtf0);
    releaseStringUTFChars(env, name1, nameUtf1);
    releaseStringUTFChars(env, name2, nameUtf2);
    releaseStringUTFChars(env, name3, nameUtf3);
    releaseStringUTFChars(env, name4, nameUtf4);
    releaseStringUTFChars(env, description0, descriptionUtf0);
    releaseStringUTFChars(env, description1, descriptionUtf1);
    releaseStringUTFChars(env, description2, descriptionUtf2);
    releaseStringUTFChars(env, description3, descriptionUtf3);
    releaseStringUTFChars(env, description4, descriptionUtf4);
    releaseStringUTFChars(env, currency0, currencyUtf0);
    releaseStringUTFChars(env, currency1, currencyUtf1);
    releaseStringUTFChars(env, currency2, currencyUtf2);
    releaseStringUTFChars(env, currency3, currencyUtf3);
    releaseStringUTFChars(env, currency4, currencyUtf4);
    releaseStringUTFChars(env, price0, priceUtf0);
    releaseStringUTFChars(env, price1, priceUtf1);
    releaseStringUTFChars(env, price2, priceUtf2);
    releaseStringUTFChars(env, price3, priceUtf3);
    releaseStringUTFChars(env, price4, priceUtf4);
}
}
