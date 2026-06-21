#ifndef GOF2_NFC_H
#define GOF2_NFC_H

// JNI function-table slot signatures used to reach the Java side.
typedef void *(*NFC_FindClassFn)(void *, void *);
typedef void *(*NFC_GetStaticMethodFn)(void *, void *, const char *, const char *);

// Read a function pointer out of the JNIEnv method table at the given byte offset.
static inline void *nfc_jni_slot(void *env, unsigned offset)
{
    return *(void **)((char *)*(void **)env + offset);
}

static inline void *nfc_find_class(void *env, void *name)
{
    return ((NFC_FindClassFn)nfc_jni_slot(env, 0x18))(env, name);
}

static inline void *nfc_get_static_method(void *env, void *cls, const char *name, const char *sig)
{
    return ((NFC_GetStaticMethodFn)nfc_jni_slot(env, 0x1c4))(env, cls, name, sig);
}

extern "C" void NFC_CallStaticVoidMethod(void *env, void *cls, void *method);
extern "C" int NFC_CallStaticBooleanMethod(void *env, void *cls, void *method);
extern "C" int NFC_CallStaticIntMethod(void *env, void *cls, void *method);
extern "C" void NFC_DeleteLocalRef(void *env);

// NFC — thin JNI bridge to the Java side (in-app purchases, store links, screen
// metrics). Every entry point operates on global JNI slots (nfc_env / nfc_class_slot /
// nfc_method_name / nfc_method_sig); the bridge carries no instance state of its own.
class NFC {
public:
    void iap_buy_dlc_full_package();
    void iap_buy_dlc_vip();
    void iap_buy_dlc_supernova();
    void iap_buy_dlc_kaamo_club();
    void iap_buy_dlc_valkyrie();
    void iap_buy_credits_100_000();
    void iap_buy_credits_300_000();
    void iap_buy_credits_1_000_000();
    void iap_buy_credits_3_000_000();
    void iap_buy_credits_10_000_000();
    void iap_restore_purchases();

    void free_credits_rateGame();
    void free_credits_subscribeToYoutubeChannel();
    void free_credits_likeGOF2OnFacebook();
    void free_credits_likeFishlabsOnFacebook();
    void free_credits_followOnTwitter();

    void showMoreGames();
    void rateGame();
    void openTermsOfService();
    void openPrivacyPolicy();

    bool isPad();
    int getWidth();
    int getHeight();
};

// ---------------------------------------------------------------------------
// Menu / dialog visibility predicates.
//
// These free functions are the small state queries the input layer uses to
// decide how a navigation gesture should be routed (back button, sub-menu
// focus, star-map overlay, ...). Each takes an unused player/context index so
// they all share one dispatch-table signature, and reads a handful of global
// UI-state flags. They carry no instance state and live outside any class,
// matching their plain global C++ symbols.
bool IsDialogVisible(int);
bool IsDialogNotVisible(int);
bool IsDialogNotVisible2(int);
bool IsStarMapNotVisible(int);
bool IsInGameSubMenuNotActive(int);
bool IsInGameSubMenuActive(int);
bool IsInPrimaryMenu(int);

// Android hardware back-button handler: raises the pending-back flag the main
// loop polls each frame. Exported under its JNI name so the Java activity can
// invoke it directly.
extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed();

#endif
