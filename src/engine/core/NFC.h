#ifndef GOF2_NFC_H
#define GOF2_NFC_H

#include <jni.h>

class NFC {
public:
    NFC();

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

    static bool isPad();

    static int getWidth();

    static int getHeight();

    static const char *interface_path;
};

extern int g_android_back_button_pressed;

int IsDialogVisible(int);

int IsDialogNotVisible(int);

int IsDialogNotVisible2(int);

int IsStarMapNotVisible(int);

int IsInGameSubMenuNotActive(int);

int IsInGameSubMenuActive(int);

int IsInPrimaryMenu(int);

int HideMouse();

void CaptureMouse(int capture);

int SwitchToOtherMouseConifguration(int config);

void UseJoystick(int use);

int GetUseJoystick();

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed(); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

#endif
