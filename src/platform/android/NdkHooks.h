#ifndef GOF2_NDKHOOKS_H
#define GOF2_NDKHOOKS_H

#include <jni.h>

// JNI environment of the render thread (set by ToJNI_setEnvironmentVariables, defined in main.c).
extern JNIEnv *g_pEnv;

// Purchase-in-flight flags raised by the NFC store buttons.
extern int gi_iap_buy_dlc1_pressed;
extern int gi_iap_buy_dlc2_pressed;
extern int gi_iap_buy_dlc3_pressed;
extern int gi_iap_buy_dlc4_pressed;
extern int gi_iap_buy_dlc5_pressed;

extern int gi_iap_buy_credit_pack1_pressed;
extern int gi_iap_buy_credit_pack2_pressed;
extern int gi_iap_buy_credit_pack3_pressed;
extern int gi_iap_buy_credit_pack4_pressed;
extern int gi_iap_buy_credit_pack5_pressed;

// Device language code handed in from Java (read by OnCreateApplication).
extern unsigned int hrwhs_country_code;

// Offerwall credits waiting to be paid out (set by the tapjoy wrapper).
extern int gb_android_offerwallCreditAmount;

extern "C" { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
void ExitFunction();

int ndk23_getScreenshotFlag();

void ndk23_resetScreenshotFlag();

int ndk23_getExitFlag();

void ndk23_setDisplayHeightAndWidth(int height, int width);

int ndk23_getCurrentFiredStatus();

void ndk23_Init(const char *apkPath, int width, int height);

void ndk23_InitWithZip(const char *apkPath, const char *zipPath, int width, int height, float scale);

void ndk23_resize(int width, int height);

void ndk23_ndkDone();

void ndk23_handleAcceleration(float x, float y, float z);

void ndk23_newrender(long long now);

void ndk_checkPlaytimeAndSpendOfferwallCredits();

void ndk23_handleTouchPadEvent(jclass clazz, void *touch, int phase, float x, float y); // lint: void_ptr (ndk* touch handle param, baked ABI)

void ndk23_setRootDirectory(const char *path);

void ndk23_setZipDirectory(const char *path);

void ndk23_setCountryCode(unsigned int code);

int loadAPK(const char *path);

int loadAPKAndZip(const char *apkPath, const char *patchPath);

int setBaughtCredits(int amount);

void checkFirstCreditPackBoughtWriteAction();

const char *getStringUTFChars(JNIEnv *env, jstring str, jboolean *isCopy);

void releaseStringUTFChars(JNIEnv *env, jstring str, const char *chars);

char *pConstToNonConst(const char *s);

void ndk23_sendingPauseSignal();

void ndk23_sendingResumeSignal();

void ndk_autosave();

int ndk_getCurrentApplicationModule();

int ndk_getLogoShown();

int ndk_isInMainMenu();

int ndk_getDLC_1_BOUGHT();

int ndk_getDLC_2_BOUGHT();

int ndk_getDLC_3_BOUGHT();

int ndk_getDLC_4_BOUGHT();

int ndk_getDLC_5_BOUGHT();

void ndk_iapBoughtConsumable(unsigned int consumable);

void ndk_iapBoughtPremium(unsigned int pack, unsigned int bought);

void ndk_setNativeItemInformationList(JNIEnv *env, jclass clazz,
                                      jobjectArray ids, jobjectArray names,
                                      jobjectArray descriptions,
                                      jobjectArray currencies,
                                      jobjectArray prices);
}

#endif
