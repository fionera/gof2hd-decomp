#include "engine/core/NFC.h"
#include "game/core/Globals.h"
#include "platform/android/NdkHooks.h"

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    g_android_back_button_pressed = 1;
}

NFC::NFC() {
}

void NFC::iap_buy_dlc_valkyrie() {
    if (g_pEnv == nullptr)
        return;

    gi_iap_buy_dlc1_pressed = 1;
    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "iap_buy_dlc_valkyrie", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::iap_buy_dlc_kaamo_club() {
    if (g_pEnv == nullptr)
        return;

    gi_iap_buy_dlc2_pressed = 1;
    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "iap_buy_dlc_kaamo_club", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::iap_buy_dlc_supernova() {
    if (g_pEnv == nullptr)
        return;

    gi_iap_buy_dlc3_pressed = 1;
    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "iap_buy_dlc_supernova", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::iap_buy_dlc_vip() {
    if (g_pEnv == nullptr)
        return;

    gi_iap_buy_dlc4_pressed = 1;
    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "iap_buy_dlc_vip", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::iap_buy_dlc_full_package() {
    if (g_pEnv == nullptr)
        return;

    gi_iap_buy_dlc5_pressed = 1;
    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "iap_buy_dlc_full_package", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::iap_restore_purchases() {
    if (g_pEnv == nullptr)
        return;

    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "iap_restore_purchases", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::iap_buy_credits_100_000() {
    if (g_pEnv == nullptr)
        return;

    gi_iap_buy_credit_pack1_pressed = 1;
    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "iap_buy_credits_100_000", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::iap_buy_credits_300_000() {
    if (g_pEnv == nullptr)
        return;

    gi_iap_buy_credit_pack2_pressed = 1;
    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "iap_buy_credits_300_000", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::iap_buy_credits_1_000_000() {
    if (g_pEnv == nullptr)
        return;

    gi_iap_buy_credit_pack3_pressed = 1;
    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "iap_buy_credits_1_000_000", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::iap_buy_credits_3_000_000() {
    if (g_pEnv == nullptr)
        return;

    gi_iap_buy_credit_pack4_pressed = 1;
    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "iap_buy_credits_3_000_000", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::iap_buy_credits_10_000_000() {
    if (g_pEnv == nullptr)
        return;

    gi_iap_buy_credit_pack5_pressed = 1;
    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "iap_buy_credits_10_000_000", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::free_credits_likeGOF2OnFacebook() {
    if (g_pEnv == nullptr)
        return;

    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "free_credits_likeGOF2OnFacebook", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::free_credits_likeFishlabsOnFacebook() {
    if (g_pEnv == nullptr)
        return;

    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "free_credits_likeFishlabsOnFacebook", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::free_credits_subscribeToYoutubeChannel() {
    if (g_pEnv == nullptr)
        return;

    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "free_credits_subscribeToYoutubeChannel", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::free_credits_followOnTwitter() {
    if (g_pEnv == nullptr)
        return;

    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "free_credits_followOnTwitter", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::showMoreGames() {
    if (g_pEnv == nullptr)
        return;

    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "showMoreGames", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::rateGame() {
    if (g_pEnv == nullptr)
        return;

    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "rateGame", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::free_credits_rateGame() {
    if (g_pEnv == nullptr)
        return;

    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "free_credits_rateGame", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

void NFC::openPrivacyPolicy() {
    if (g_pEnv == nullptr)
        return;

    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "openPrivacyPolicy", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

// Clears a pending Java exception left behind by a DeviceInfo query.
static void clearPendingException(JNIEnv *env) {
    if (env->ExceptionCheck())
        env->ExceptionClear();
}

void NFC::openTermsOfService() {
    if (g_pEnv == nullptr)
        return;

    jclass cls = g_pEnv->FindClass(interface_path);
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "openTermsOfService", "()V");
    g_pEnv->CallStaticVoidMethod(cls, method);
}

bool NFC::isPad() {
    jclass cls = g_pEnv->FindClass("net/fishlabs/ow_utils/DeviceInfo");
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "isPad", "()Z");
    jboolean value = g_pEnv->CallStaticBooleanMethod(cls, method);
    clearPendingException(g_pEnv);
    return value != 0;
}

int NFC::getWidth() {
    jclass cls = g_pEnv->FindClass("net/fishlabs/ow_utils/DeviceInfo");
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "getDisplayWidth", "()I");
    jint value = g_pEnv->CallStaticIntMethod(cls, method);
    clearPendingException(g_pEnv);
    return value;
}

int g_android_back_button_pressed = 0;

int NFC::getHeight() {
    jclass cls = g_pEnv->FindClass("net/fishlabs/ow_utils/DeviceInfo");
    jmethodID method = g_pEnv->GetStaticMethodID(cls, "getDisplayHeight", "()I");
    jint value = g_pEnv->CallStaticIntMethod(cls, method);
    clearPendingException(g_pEnv);
    return value;
}

const char *NFC::interface_path;
