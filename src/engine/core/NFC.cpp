#include "gof2/engine/core/NFC.h"




// ---- iap_buy_dlc_full_package_69744.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern int *nfc_purchase_flag __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::iap_buy_dlc_full_package()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    int *flag = nfc_purchase_flag;
    void **class_slot = nfc_class_slot;
    *flag = 1;
    void *table = *(void **)env;
    void *cls = ((NFC_FindClassFn)*(void **)((char *)table + 0x18))(env, *class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- isPad_69ccc.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern char nfc_class_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

bool NFC::isPad()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    void *cls = nfc_find_class(env, nfc_class_name);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    int value = NFC_CallStaticBooleanMethod(*env_slot, cls, method);
    NFC_DeleteLocalRef(*env_slot);
    return value != 0;
}

// ---- free_credits_rateGame_69bd0.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::free_credits_rateGame()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- free_credits_subscribeToYoutubeChannel_69a80.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::free_credits_subscribeToYoutubeChannel()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- free_credits_likeGOF2OnFacebook_699d8.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::free_credits_likeGOF2OnFacebook()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- iap_buy_credits_300_000_69858.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern int *nfc_purchase_flag __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::iap_buy_credits_300_000()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    int *flag = nfc_purchase_flag;
    void **class_slot = nfc_class_slot;
    *flag = 1;
    void *table = *(void **)env;
    void *cls = ((NFC_FindClassFn)*(void **)((char *)table + 0x18))(env, *class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- free_credits_followOnTwitter_69ad4.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::free_credits_followOnTwitter()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- showMoreGames_69b28.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::showMoreGames()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- iap_buy_dlc_vip_696e4.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern int *nfc_purchase_flag __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::iap_buy_dlc_vip()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    int *flag = nfc_purchase_flag;
    void **class_slot = nfc_class_slot;
    *flag = 1;
    void *table = *(void **)env;
    void *cls = ((NFC_FindClassFn)*(void **)((char *)table + 0x18))(env, *class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- iap_buy_credits_1_000_000_698b8.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern int *nfc_purchase_flag __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::iap_buy_credits_1_000_000()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    int *flag = nfc_purchase_flag;
    void **class_slot = nfc_class_slot;
    *flag = 1;
    void *table = *(void **)env;
    void *cls = ((NFC_FindClassFn)*(void **)((char *)table + 0x18))(env, *class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- iap_buy_dlc_supernova_69684.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern int *nfc_purchase_flag __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::iap_buy_dlc_supernova()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    int *flag = nfc_purchase_flag;
    void **class_slot = nfc_class_slot;
    *flag = 1;
    void *table = *(void **)env;
    void *cls = ((NFC_FindClassFn)*(void **)((char *)table + 0x18))(env, *class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- iap_buy_dlc_kaamo_club_69624.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern int *nfc_purchase_flag __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::iap_buy_dlc_kaamo_club()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    int *flag = nfc_purchase_flag;
    void **class_slot = nfc_class_slot;
    *flag = 1;
    void *table = *(void **)env;
    void *cls = ((NFC_FindClassFn)*(void **)((char *)table + 0x18))(env, *class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- iap_buy_dlc_valkyrie_6958c.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern int *nfc_purchase_flag __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::iap_buy_dlc_valkyrie()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    int *flag = nfc_purchase_flag;
    void **class_slot = nfc_class_slot;
    *flag = 1;
    void *table = *(void **)env;
    void *cls = ((NFC_FindClassFn)*(void **)((char *)table + 0x18))(env, *class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- free_credits_likeFishlabsOnFacebook_69a2c.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::free_credits_likeFishlabsOnFacebook()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- getWidth_69d7c.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern char nfc_class_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

int NFC::getWidth()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    void *cls = nfc_find_class(env, nfc_class_name);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    int value = NFC_CallStaticIntMethod(*env_slot, cls, method);
    NFC_DeleteLocalRef(*env_slot);
    return value;
}

// ---- getHeight_69e08.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern char nfc_class_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

int NFC::getHeight()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    void *cls = nfc_find_class(env, nfc_class_name);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    int value = NFC_CallStaticIntMethod(*env_slot, cls, method);
    NFC_DeleteLocalRef(*env_slot);
    return value;
}

// ---- iap_restore_purchases_697a4.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::iap_restore_purchases()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- rateGame_69b7c.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::rateGame()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- openTermsOfService_69c78.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::openTermsOfService()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- iap_buy_credits_10_000_000_69978.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern int *nfc_purchase_flag __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::iap_buy_credits_10_000_000()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    int *flag = nfc_purchase_flag;
    void **class_slot = nfc_class_slot;
    *flag = 1;
    void *table = *(void **)env;
    void *cls = ((NFC_FindClassFn)*(void **)((char *)table + 0x18))(env, *class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- iap_buy_credits_100_000_697f8.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern int *nfc_purchase_flag __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::iap_buy_credits_100_000()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    int *flag = nfc_purchase_flag;
    void **class_slot = nfc_class_slot;
    *flag = 1;
    void *table = *(void **)env;
    void *cls = ((NFC_FindClassFn)*(void **)((char *)table + 0x18))(env, *class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- iap_buy_credits_3_000_000_69918.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern int *nfc_purchase_flag __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::iap_buy_credits_3_000_000()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    int *flag = nfc_purchase_flag;
    void **class_slot = nfc_class_slot;
    *flag = 1;
    void *table = *(void **)env;
    void *cls = ((NFC_FindClassFn)*(void **)((char *)table + 0x18))(env, *class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}

// ---- openPrivacyPolicy_69c24.cpp ----
extern void **nfc_env __attribute__((visibility("hidden")));
extern void **nfc_class_slot __attribute__((visibility("hidden")));
extern const char nfc_method_name[] __attribute__((visibility("hidden")));
extern const char nfc_method_sig[] __attribute__((visibility("hidden")));

void NFC::openPrivacyPolicy()
{
    void **env_slot = nfc_env;
    void *env = *env_slot;
    if (env == 0)
        return;

    void *cls = nfc_find_class(env, *nfc_class_slot);
    void *method = nfc_get_static_method(*env_slot, cls, nfc_method_name, nfc_method_sig);
    return NFC_CallStaticVoidMethod(*env_slot, cls, method);
}
