#include "engine/core/NFC.h"

static _JNIEnv **nfc_env;
static int *nfc_purchase_flag;
static _jclass **nfc_class_slot;
static char nfc_class_name[1];
static const char nfc_method_name[1] = {0};
static const char nfc_method_sig[1] = {0};

NFC::NFC() {
}

void NFC::iap_buy_dlc_full_package() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_dlc_vip() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_dlc_supernova() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_dlc_kaamo_club() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_dlc_valkyrie() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_credits_100_000() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_credits_300_000() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_credits_1_000_000() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_credits_3_000_000() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_credits_10_000_000() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_restore_purchases() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::free_credits_rateGame() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::free_credits_subscribeToYoutubeChannel() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::free_credits_likeGOF2OnFacebook() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::free_credits_likeFishlabsOnFacebook() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::free_credits_followOnTwitter() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::showMoreGames() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::rateGame() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::openTermsOfService() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::openPrivacyPolicy() {
    _JNIEnv *env = *nfc_env;
    if (env == nullptr)
        return;

    _jclass *cls = nfc_find_class(env, *nfc_class_slot);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

bool NFC::isPad() {
    _JNIEnv *env = *nfc_env;
    _jclass *cls = nfc_find_class(env, (_jclass *) nfc_class_name);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    int value = NFC_CallStaticBooleanMethod(env, cls, method);
    NFC_DeleteLocalRef(env);
    return value != 0;
}

int NFC::getWidth() {
    _JNIEnv *env = *nfc_env;
    _jclass *cls = nfc_find_class(env, (_jclass *) nfc_class_name);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    int value = NFC_CallStaticIntMethod(env, cls, method);
    NFC_DeleteLocalRef(env);
    return value;
}

int NFC::getHeight() {
    _JNIEnv *env = *nfc_env;
    _jclass *cls = nfc_find_class(env, (_jclass *) nfc_class_name);
    _jmethodID *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    int value = NFC_CallStaticIntMethod(env, cls, method);
    NFC_DeleteLocalRef(env);
    return value;
}

static int is_dialogue_window_visible;
static int is_choice_window_visible;
static int is_menu_visible;
static bool isStarMapVisible;
static int subMenuIndex;
static int topMenuIndex;
static int menu_touch_window_type;
int g_android_back_button_pressed = 0;

bool IsDialogVisible(int) {
    return is_dialogue_window_visible != 0 || is_choice_window_visible != 0;
}

bool IsDialogNotVisible(int) {
    return is_dialogue_window_visible == 0 && is_choice_window_visible == 0;
}

bool IsDialogNotVisible2(int) {
    return is_dialogue_window_visible == 0 && is_choice_window_visible == 0 &&
           is_menu_visible == 0;
}

bool IsStarMapNotVisible(int) {
    return !isStarMapVisible;
}

bool IsInGameSubMenuNotActive(int) {
    if (subMenuIndex == -1 && topMenuIndex == 0)
        return is_menu_visible == 0;
    return false;
}

bool IsInGameSubMenuActive(int) {
    return subMenuIndex != -1;
}

bool IsInPrimaryMenu(int) {
    if (is_menu_visible == 0)
        return false;
    if (topMenuIndex != 0 || menu_touch_window_type != 0 ||
        is_dialogue_window_visible != 0 || is_choice_window_visible != 0)
        return false;
    return !isStarMapVisible;
}

static int g_mouseHidden;
static int g_mouseCaptured;
static int g_mouseConfig;
static int g_useJoystick;

int HideMouse() {
    return g_mouseHidden;
}

void CaptureMouse(int capture) {
    g_mouseCaptured = capture;
}

int SwitchToOtherMouseConifguration(int config) {
    g_mouseConfig = config;
    return 0;
}

void UseJoystick(int use) {
    g_useJoystick = use;
}

int GetUseJoystick() {
    return g_useJoystick;
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    g_android_back_button_pressed = 1;
}

const char *NFC::interface_path;
