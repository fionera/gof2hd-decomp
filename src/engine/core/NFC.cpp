#include "gof2/engine/core/NFC.h"

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
