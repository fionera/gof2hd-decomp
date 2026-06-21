#include "engine/core/NFC.h"

// Global JNI slots shared by every bridge entry point.
extern void **nfc_env __attribute__((visibility("hidden")));
extern int *nfc_purchase_flag __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern char nfc_class_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

// ---- in-app purchases -----------------------------------------------------------------------
// Each purchase sets nfc_purchase_flag before dispatching to the Java side.

void NFC::iap_buy_dlc_full_package()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_dlc_vip()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_dlc_supernova()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_dlc_kaamo_club()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_dlc_valkyrie()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_credits_100_000()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_credits_300_000()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_credits_1_000_000()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_credits_3_000_000()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_buy_credits_10_000_000()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    *nfc_purchase_flag = 1;
    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::iap_restore_purchases()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

// ---- free-credit social actions -------------------------------------------------------------

void NFC::free_credits_rateGame()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::free_credits_subscribeToYoutubeChannel()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::free_credits_likeGOF2OnFacebook()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::free_credits_likeFishlabsOnFacebook()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::free_credits_followOnTwitter()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

// ---- store / legal links --------------------------------------------------------------------

void NFC::showMoreGames()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::rateGame()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::openTermsOfService()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

void NFC::openPrivacyPolicy()
{
    void *env = *nfc_env;
    if (env == nullptr)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    NFC_CallStaticVoidMethod(env, cls, method);
}

// ---- screen / device metrics ----------------------------------------------------------------

bool NFC::isPad()
{
    void *env = *nfc_env;
    void *cls = nfc_find_class(env, nfc_class_name);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    int value = NFC_CallStaticBooleanMethod(env, cls, method);
    NFC_DeleteLocalRef(env);
    return value != 0;
}

int NFC::getWidth()
{
    void *env = *nfc_env;
    void *cls = nfc_find_class(env, nfc_class_name);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    int value = NFC_CallStaticIntMethod(env, cls, method);
    NFC_DeleteLocalRef(env);
    return value;
}

int NFC::getHeight()
{
    void *env = *nfc_env;
    void *cls = nfc_find_class(env, nfc_class_name);
    void *method = nfc_get_static_method(env, cls, nfc_method_name, nfc_method_sig);
    int value = NFC_CallStaticIntMethod(env, cls, method);
    NFC_DeleteLocalRef(env);
    return value;
}

// ---- menu / dialog visibility predicates ----------------------------------------------------
//
// Global UI-state flags shared with the rest of the engine. The input layer
// only ever reads them here; the windows that own each flag set it elsewhere.
extern "C" int is_dialogue_window_visible __attribute__((visibility("hidden")));
extern "C" int is_choice_window_visible __attribute__((visibility("hidden")));
extern "C" int is_menu_visible __attribute__((visibility("hidden")));
extern "C" bool isStarMapVisible __attribute__((visibility("hidden")));
extern "C" int subMenuIndex __attribute__((visibility("hidden")));
extern "C" int topMenuIndex __attribute__((visibility("hidden")));
extern "C" int menu_touch_window_type __attribute__((visibility("hidden")));
extern "C" int g_android_back_button_pressed __attribute__((visibility("hidden")));

// True while either the narrative dialogue box or its yes/no choice box is up.
bool IsDialogVisible(int)
{
    return is_dialogue_window_visible != 0 || is_choice_window_visible != 0;
}

// Inverse of IsDialogVisible — no dialogue or choice box on screen.
bool IsDialogNotVisible(int)
{
    return is_dialogue_window_visible == 0 && is_choice_window_visible == 0;
}

// Like IsDialogNotVisible but also requires the menu overlay to be closed.
bool IsDialogNotVisible2(int)
{
    return is_dialogue_window_visible == 0 && is_choice_window_visible == 0 &&
           is_menu_visible == 0;
}

// True while the galaxy star-map overlay is not being shown.
bool IsStarMapNotVisible(int)
{
    return !isStarMapVisible;
}

// True while no in-game sub-menu is active: the top menu is at its root
// (subMenuIndex == -1, topMenuIndex == 0) and the menu overlay is hidden.
bool IsInGameSubMenuNotActive(int)
{
    if (subMenuIndex == -1 && topMenuIndex == 0)
        return is_menu_visible == 0;
    return false;
}

// True while the player sits on a primary (top-level) menu page: the menu
// overlay is up, nothing deeper has focus (no sub-page, no touch window, no
// dialogue/choice box) and the star-map overlay is closed.
bool IsInPrimaryMenu(int)
{
    if (is_menu_visible == 0)
        return false;
    if (topMenuIndex != 0 || menu_touch_window_type != 0 ||
        is_dialogue_window_visible != 0 || is_choice_window_visible != 0)
        return false;
    return !isStarMapVisible;
}

// ---- Android hardware back button -----------------------------------------------------------

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed()
{
    g_android_back_button_pressed = 1;
}
