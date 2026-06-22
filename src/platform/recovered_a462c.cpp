

#include "platform/recovered_a462c.h"

#include <cstdlib>
#include <cstring>

#include "game/mission/Status.h"
#include "game/mission/RecordHandler.h"
#include "engine/core/ApplicationManager.h"
#include "engine/core/IApplicationModule.h"
#include "engine/render/Engine.h"
#include "engine/audio/FModSound.h"

extern Status *Globals_status asm("_ZN7Globals6statusE");
extern unsigned char Globals_options[100] asm("_ZN7Globals7optionsE");

extern FModSound *Globals_sound asm("_ZN7Globals5soundE");
extern int Globals_logoIsShown asm("_ZN7Globals11logoIsShownE");
extern int Globals_isInMainMenu asm("_ZN7Globals12isInMainMenuE");

extern "C" AbyssEngine::Engine **g_pEngine;

extern "C" int gi_iap_buy_dlc1_pressed;
extern "C" int gi_iap_buy_dlc2_pressed;
extern "C" int gi_iap_buy_dlc3_pressed;
extern "C" int gi_iap_buy_dlc4_pressed;
extern "C" int gi_iap_buy_dlc5_pressed;

extern char *Globals_cItemListID_00 asm("_ZN7Globals14cItemListID_00E");
extern char *Globals_cItemListID_01 asm("_ZN7Globals14cItemListID_01E");
extern char *Globals_cItemListID_02 asm("_ZN7Globals14cItemListID_02E");
extern char *Globals_cItemListID_03 asm("_ZN7Globals14cItemListID_03E");
extern char *Globals_cItemListID_04 asm("_ZN7Globals14cItemListID_04E");
extern char *Globals_cItemListName_00 asm("_ZN7Globals16cItemListName_00E");
extern char *Globals_cItemListName_01 asm("_ZN7Globals16cItemListName_01E");
extern char *Globals_cItemListName_02 asm("_ZN7Globals16cItemListName_02E");
extern char *Globals_cItemListName_03 asm("_ZN7Globals16cItemListName_03E");
extern char *Globals_cItemListName_04 asm("_ZN7Globals16cItemListName_04E");
extern char *Globals_cItemListDescription_00 asm("_ZN7Globals23cItemListDescription_00E");
extern char *Globals_cItemListDescription_01 asm("_ZN7Globals23cItemListDescription_01E");
extern char *Globals_cItemListDescription_02 asm("_ZN7Globals23cItemListDescription_02E");
extern char *Globals_cItemListDescription_03 asm("_ZN7Globals23cItemListDescription_03E");
extern char *Globals_cItemListDescription_04 asm("_ZN7Globals23cItemListDescription_04E");
extern char *Globals_cItemListCurrency_00 asm("_ZN7Globals20cItemListCurrency_00E");
extern char *Globals_cItemListCurrency_01 asm("_ZN7Globals20cItemListCurrency_01E");
extern char *Globals_cItemListCurrency_02 asm("_ZN7Globals20cItemListCurrency_02E");
extern char *Globals_cItemListCurrency_03 asm("_ZN7Globals20cItemListCurrency_03E");
extern char *Globals_cItemListCurrency_04 asm("_ZN7Globals20cItemListCurrency_04E");
extern char *Globals_cItemListPrice_00 asm("_ZN7Globals17cItemListPrice_00E");
extern char *Globals_cItemListPrice_01 asm("_ZN7Globals17cItemListPrice_01E");
extern char *Globals_cItemListPrice_02 asm("_ZN7Globals17cItemListPrice_02E");
extern char *Globals_cItemListPrice_03 asm("_ZN7Globals17cItemListPrice_03E");
extern char *Globals_cItemListPrice_04 asm("_ZN7Globals17cItemListPrice_04E");

extern "C" int gi_iap_buy_credit_pack1_pressed;
extern "C" int gi_iap_buy_credit_pack2_pressed;
extern "C" int gi_iap_buy_credit_pack3_pressed;
extern "C" int gi_iap_buy_credit_pack4_pressed;
extern "C" int gi_iap_buy_credit_pack5_pressed;

extern "C" void ndk_autosave();

int setBaughtCredits(int amount) {
    int *packFlag;
    if (amount == 10000000) {
        Globals_status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack1_pressed;
    } else if (amount == 300000) {
        Globals_status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack2_pressed;
    } else if (amount == 1000000) {
        Globals_status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack3_pressed;
    } else if (amount == 3000000) {
        Globals_status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack4_pressed;
    } else if (amount == 100000) {
        Globals_status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack5_pressed;
    } else {
        return 0;
    }

    if (*packFlag != 0) {
        *packFlag = 0;
        checkFirstCreditPackBoughtWriteAction();
    }
    ndk_autosave();
    return 1;
}

void checkFirstCreditPackBoughtWriteAction() {
    Globals_status->getCurrentCampaignMission();
    if (Globals_options[0x62] == 0) {
        Globals_options[0x62] = 1;
        RecordHandler handler;
        handler.saveOptions();
    }
}

const char *getStringUTFChars(JNIEnv *env, jstring str) {
    if (env != nullptr && str != nullptr) {
        return reinterpret_cast<const char *(*)(JNIEnv *, jstring)>(
            env->functions->GetStringUTFChars)(env, str);
    }
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
        operator delete(reinterpret_cast<void *>(str));
    }
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

void ndk23_sendingPauseSignal() {
    if (*g_pEngine == nullptr)
        return;
    Globals_sound->pauseAll();

    static_cast<IApplicationModule *>(gAppManager->currentModule)->OnSuspend();
}

void ndk23_sendingResumeSignal() {
    if (*g_pEngine == nullptr)
        return;
    Globals_sound->resumeAll();
}

void ndk_autosave() {
    RecordHandler *handler = new RecordHandler;
    handler->recordStoreWrite(0);
    handler->recordStoreWritePreview(0);
    delete handler;
}

int ndk_getCurrentApplicationModule() {
    if (*g_pEngine == nullptr)
        return -1;
    return static_cast<int>(gAppManager->currentModuleId);
}

int ndk_getLogoShown() {
    return Globals_logoIsShown;
}

int ndk_isInMainMenu() {
    return Globals_isInMainMenu;
}

bool ndk_getDLC_1_BOUGHT() {
    return *g_pEngine != nullptr && Globals_options[0x35] != 0;
}

bool ndk_getDLC_2_BOUGHT() {
    return *g_pEngine != nullptr && Globals_options[0x36] != 0;
}

bool ndk_getDLC_3_BOUGHT() {
    return *g_pEngine != nullptr && Globals_options[0x37] != 0;
}

bool ndk_getDLC_4_BOUGHT() {
    return *g_pEngine != nullptr && Globals_options[0x38] != 0;
}

bool ndk_getDLC_5_BOUGHT() {
    return *g_pEngine != nullptr && Globals_options[0x39] != 0;
}

void ndk_iapBoughtConsumable(unsigned int consumable) {
    if (consumable > 4 || *g_pEngine == nullptr)
        return;

    static const int kCreditPackValue[5] = {
        100000, 300000, 1000000, 3000000, 10000000
    };
    setBaughtCredits(kCreditPackValue[consumable]);
}

void ndk_iapBoughtPremium(unsigned int pack, unsigned int bought) {
    if (*g_pEngine == nullptr || pack > 4)
        return;

    int *pendingFlags[5] = {
        &gi_iap_buy_dlc1_pressed, &gi_iap_buy_dlc2_pressed,
        &gi_iap_buy_dlc3_pressed, &gi_iap_buy_dlc4_pressed,
        &gi_iap_buy_dlc5_pressed
    };
    if (*pendingFlags[pack] != 0)
        *pendingFlags[pack] = 0;

    Globals_options[0x35 + pack] = (bought == 1) ? 1 : 0;
}

void ndk_setNativeItemInformationList(JNIEnv *env, jclass /*clazz*/,
                                      jobjectArray ids, jobjectArray names,
                                      jobjectArray descriptions,
                                      jobjectArray currencies,
                                      jobjectArray prices) {
    if (env == nullptr)
        return;

    jstring elems[5][5];
    jobjectArray arrays[5] = {ids, names, descriptions, currencies, prices};
    for (int col = 0; col < 5; ++col)
        for (int row = 0; row < 5; ++row)
            elems[col][row] =
                    static_cast<jstring>(env->GetObjectArrayElement(arrays[col], row));

    const char *utf[5][5];
    for (int col = 0; col < 5; ++col)
        for (int row = 0; row < 5; ++row)
            utf[col][row] = env->GetStringUTFChars(elems[col][row], nullptr);

    char **slots[5][5] = {
        {
            &Globals_cItemListID_00, &Globals_cItemListID_01, &Globals_cItemListID_02,
            &Globals_cItemListID_03, &Globals_cItemListID_04
        },
        {
            &Globals_cItemListName_00, &Globals_cItemListName_01, &Globals_cItemListName_02,
            &Globals_cItemListName_03, &Globals_cItemListName_04
        },
        {
            &Globals_cItemListDescription_00, &Globals_cItemListDescription_01,
            &Globals_cItemListDescription_02, &Globals_cItemListDescription_03,
            &Globals_cItemListDescription_04
        },
        {
            &Globals_cItemListCurrency_00, &Globals_cItemListCurrency_01,
            &Globals_cItemListCurrency_02, &Globals_cItemListCurrency_03,
            &Globals_cItemListCurrency_04
        },
        {
            &Globals_cItemListPrice_00, &Globals_cItemListPrice_01, &Globals_cItemListPrice_02,
            &Globals_cItemListPrice_03, &Globals_cItemListPrice_04
        }
    };
    for (int col = 0; col < 5; ++col)
        for (int row = 0; row < 5; ++row)
            *slots[col][row] = pConstToNonConst(utf[col][row]);

    for (int col = 0; col < 5; ++col)
        for (int row = 0; row < 5; ++row)
            env->ReleaseStringUTFChars(elems[col][row], utf[col][row]);
}
