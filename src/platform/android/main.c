/* main.c -- JNI entry layer of the Android build (original TU at JNI_OnLoad, .text 0x6b550..0x6ba90).
 *
 * This TU is C, not C++ (binary evidence):
 *  - its log strings name it ("dataDirectory in main.c: %s");
 *  - its uninitialized globals (g_pVM, g_pEnv, g_pClass, AccIndex, flstat_rootPath, g_android_*) are
 *    COMMON symbols: the linker placed them at the very end of .bss, sorted by size/name, which only
 *    C tentative definitions produce;
 *  - the Java_* wrappers call the ndk23_* layer through main.c's own prototypes, which disagree with
 *    the NdkHooks.cpp definitions (renderstep hands two ints to ndk23_newrender(long long), the touch
 *    wrapper drops the clazz slot).
 *
 * Every Java_* wrapper forwards to the ndk23_* / ndk_* layer in NdkHooks.cpp.
 */
#include <jni.h>
#include <android/log.h>

#include <stdlib.h>
#include <string.h>

/* main.c's view of the ndk layer (see the header note for the two prototypes that differ). */
void ndk23_setCountryCode(unsigned int code);
void ndk23_setRootDirectory(const char *path);
void ndk23_setZipDirectory(const char *path);
void ndk23_InitWithZip(const char *apkPath, const char *zipPath, int width, int height, float scale);
void ndk23_setDisplayHeightAndWidth(int height, int width);
void ndk23_newrender(int width, int height);
int ndk23_getExitFlag(void);
int ndk23_getScreenshotFlag(void);
void ndk23_resetScreenshotFlag(void);
void ndk23_sendingPauseSignal(void);
void ndk23_sendingResumeSignal(void);
void ndk23_handleAcceleration(float x, float y, float z);
void ndk23_resize(int width, int height);
void ndk23_handleTouchScreenEvent(int touch, int phase, float x, float y);
int ndk_getDLC_1_BOUGHT(void);
int ndk_getDLC_2_BOUGHT(void);
int ndk_getDLC_3_BOUGHT(void);
int ndk_getDLC_4_BOUGHT(void);
int ndk_getDLC_5_BOUGHT(void);
void ndk_iapBoughtPremium(unsigned int pack, unsigned int bought);
void ndk_iapBoughtConsumable(unsigned int consumable);
void ndk_setNativeItemInformationList(JNIEnv *env, jclass clazz, jobjectArray ids, jobjectArray names,
                                      jobjectArray descriptions, jobjectArray currencies,
                                      jobjectArray prices);
void ndk_resetNativeItemInformationList(void);
void ndk_checkPlaytimeAndSpendOfferwallCredits(void);
int ndk_getCurrentApplicationModule(void);
int ndk_getLogoShown(void);
int ndk_isInMainMenu(void);

extern int gb_android_offerwallCreditAmount;

JavaVM *g_pVM;

JNIEnv *g_pEnv;
jobject g_pClass;
jobject g_pActivity = NULL;

char *ndk_APK_Path = NULL;
char *ndk_ZIP_Path = NULL;

char *flstat_rootPath;

int AccIndex;

/* Raised by the NFC back key, consumed by ndk23_newrender. */
int g_android_back_button_pressed;

jint JNI_OnLoad(JavaVM *vm, void *reserved) { // lint: void_ptr (JNI_OnLoad reserved param, baked JNI ABI)
    JNIEnv *env = NULL;
    g_pVM = vm;
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_4) != JNI_OK) // lint: void_ptr (GetEnv requires void** out-param, JNI ABI)
        return -1;
    if ((*env)->FindClass(env, "net/fishlabs/gof2hdallandroid2012/GOF2HD2012") == NULL)
        return -1;
    return JNI_VERSION_1_4;
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_setEnvironmentVariables(JNIEnv *env, jclass clazz,
                                                                          jobject activity) {
    g_pClass = clazz;
    g_pEnv = env;
    g_pActivity = activity;
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_setAPKPath(JNIEnv *env, jclass clazz, jstring path) {
    jboolean isCopy;
    const char *utf = (*env)->GetStringUTFChars(env, path, &isCopy);
    ndk_APK_Path = (char *) malloc(strlen(utf) + 1);
    if (isCopy) {
        strcpy(ndk_APK_Path, utf);
        (*env)->ReleaseStringUTFChars(env, path, utf);
    }
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_setZIPPath(JNIEnv *env, jclass clazz, jstring path) {
    jboolean isCopy;
    const char *utf = (*env)->GetStringUTFChars(env, path, &isCopy);
    ndk_ZIP_Path = (char *) malloc(strlen(utf) + 1);
    if (isCopy) {
        strcpy(ndk_ZIP_Path, utf);
        (*env)->ReleaseStringUTFChars(env, path, utf);
    }
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_setCountryCodeOfDevice(JNIEnv *env, jclass clazz,
                                                                         jint code) {
    ndk23_setCountryCode((unsigned int) code);
}

char *dataDirectory = NULL;
char *zipDirectory = NULL;

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_SetDirectories(JNIEnv *env, jclass clazz,
                                                                 jstring rootDir, jstring zipDir) {
    jboolean rootCopied;
    const char *root = (*env)->GetStringUTFChars(env, rootDir, &rootCopied);
    size_t rootLength = strlen(root) + 1;
    dataDirectory = (char *) malloc(rootLength);
    flstat_rootPath = (char *) malloc(rootLength);
    if (rootCopied) {
        strcpy(dataDirectory, root);
        strcpy(flstat_rootPath, root);
        ndk23_setRootDirectory(dataDirectory);
        __android_log_print(ANDROID_LOG_ERROR, "GOF2_NATIVE", "dataDirectory in main.c: %s", dataDirectory);
        __android_log_print(ANDROID_LOG_ERROR, "GOF2_NATIVE", "rootPath in main.c: %s", flstat_rootPath);
        (*env)->ReleaseStringUTFChars(env, rootDir, root);
    }

    jboolean zipCopied;
    const char *zip = (*env)->GetStringUTFChars(env, zipDir, &zipCopied);
    zipDirectory = (char *) malloc(strlen(zip) + 1);
    if (zipCopied) {
        strcpy(zipDirectory, zip);
        ndk23_setZipDirectory(zipDirectory);
        __android_log_print(ANDROID_LOG_ERROR, "GOF2_NATIVE", "zipDirectory in main.c: %s", zipDirectory);
        (*env)->ReleaseStringUTFChars(env, zipDir, zip);
    }
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_initialize(JNIEnv *env, jclass clazz, jint width,
                                                             jint height) {
    ndk23_InitWithZip(ndk_APK_Path, ndk_ZIP_Path, width, height, 1.0f);
    ndk23_setDisplayHeightAndWidth(width, height);
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_renderstep(JNIEnv *env, jclass clazz, jint width,
                                                             jint height) {
    ndk23_newrender(width, height);
}

jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getExitFlag(JNIEnv *env, jclass clazz) {
    return ndk23_getExitFlag();
}

jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getScreenshotFlag(JNIEnv *env, jclass clazz) {
    return ndk23_getScreenshotFlag();
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_resetScreenshotFlag(JNIEnv *env, jclass clazz) {
    ndk23_resetScreenshotFlag();
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_sendPauseSignalToGame(JNIEnv *env, jclass clazz) {
    ndk23_sendingPauseSignal();
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_sendResumeSignalToGame(JNIEnv *env, jclass clazz) {
    ndk23_sendingResumeSignal();
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_STARTUP(JNIEnv *env, jclass clazz) {
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_handleAccelerometer(JNIEnv *env, jclass clazz, jfloat x,
                                                                      jfloat y, jfloat z) {
    ndk23_handleAcceleration(-x, y, z);
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_resize(JNIEnv *env, jclass clazz, jint width, jint height) {
    ndk23_resize(width, height);
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_handleTouchEvent(JNIEnv *env, jclass clazz, jint touch,
                                                                   jint phase, jfloat x, jfloat y) {
    ndk23_handleTouchScreenEvent(touch, phase, x, y);
}

jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC1BOUGHT(JNIEnv *env, jclass clazz) {
    return ndk_getDLC_1_BOUGHT();
}

jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC2BOUGHT(JNIEnv *env, jclass clazz) {
    return ndk_getDLC_2_BOUGHT();
}

jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC3BOUGHT(JNIEnv *env, jclass clazz) {
    return ndk_getDLC_3_BOUGHT();
}

jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC4BOUGHT(JNIEnv *env, jclass clazz) {
    return ndk_getDLC_4_BOUGHT();
}

jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getDLC5BOUGHT(JNIEnv *env, jclass clazz) {
    return ndk_getDLC_5_BOUGHT();
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC1(JNIEnv *env, jclass clazz) {
    ndk_iapBoughtPremium(0, 1);
}

int g_android_current_achievements[3];
int g_android_leaderboard_scores[8];
int g_android_link_game_gp;
int g_android_gp_is_linked;
int g_android_show_achievements;
int g_android_show_leaderboards;

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC2(JNIEnv *env, jclass clazz) {
    ndk_iapBoughtPremium(1, 1);
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC3(JNIEnv *env, jclass clazz) {
    ndk_iapBoughtPremium(2, 1);
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC4(JNIEnv *env, jclass clazz) {
    ndk_iapBoughtPremium(3, 1);
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_correctBoughtDLC5(JNIEnv *env, jclass clazz) {
    ndk_iapBoughtPremium(4, 1);
}

void Java_net_fishlabs_gof2hdallandroid2012_ToJNI_testPurchase(JNIEnv *env, jclass clazz) {
    ndk_iapBoughtConsumable(0);
}

jstring Java_net_fishlabs_googleplay_ToJNI_gof2hd2012apk(JNIEnv *env, jclass clazz) {
    /* Google Play licensing public key (base64 RSA SubjectPublicKeyInfo) of the shipped APK. */
    return (*env)->NewStringUTF(env,
        "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAiSfBidMj3fOEJih9J4/2"
        "RUjqwsjbcl1O1gbHSVB9LpKh5G5ds0rLsVGw52j44szFre/CinNDST/OjnDBkMlr"
        "ewUxZwXPrYycO3JTE3BjS9Fl8kADtN2aU4LvdVGZBSk32TVgqBLLGDNUFfda3fv+"
        "MTQmcXDCFmTRbZwjHFhGOd56L3RO35KUszkak9ST5UL2nNJL9yOqcQ2BWkT+oI3a"
        "hALETTUCE6VdHEoliW9Mb+E9BaU2hOmowkJIWQhkbw+d4d44Y9dqVSwNS7N453IV"
        "wifLXANv7MY+9nIO7qHtGTNQC1VKgcHEs/slbpqahUBcsifJP3xgD6/pNbOzGHpY"
        "VQIDAQAB");
}

jstring Java_net_fishlabs_googleplay_ToJNI_gof2hd2012getPremiumSKU(JNIEnv *env, jclass clazz, jint index) {
    switch (index) {
        case 0: return (*env)->NewStringUTF(env, "iap_managed_valkyrie");
        case 1: return (*env)->NewStringUTF(env, "iap_managed_kaamo_club");
        case 2: return (*env)->NewStringUTF(env, "iap_managed_supernova");
        case 3: return (*env)->NewStringUTF(env, "iap_managed_vip");
        case 4: return (*env)->NewStringUTF(env, "iap_managed_full_package");
    }
    return NULL;
}

jstring Java_net_fishlabs_googleplay_ToJNI_gof2hd2012getConsumableSKU(JNIEnv *env, jclass clazz,
                                                                      jint index) {
    switch (index) {
        case 0: return (*env)->NewStringUTF(env, "iap_managed_credits_100_000");
        case 1: return (*env)->NewStringUTF(env, "iap_managed_credits_300_000");
        case 2: return (*env)->NewStringUTF(env, "iap_managed_credits_1_000_000");
        case 3: return (*env)->NewStringUTF(env, "iap_managed_credits_3_000_000");
        case 4: return (*env)->NewStringUTF(env, "iap_managed_10_000_000");
    }
    return NULL;
}

void Java_net_fishlabs_googleplay_ToJNI_iapBoughtPremium(JNIEnv *env, jclass clazz, jint pack, jint bought) {
    ndk_iapBoughtPremium((unsigned int) pack, (unsigned int) bought);
}

void Java_net_fishlabs_googleplay_ToJNI_iapBoughtConsumable(JNIEnv *env, jclass clazz, jint consumable) {
    ndk_iapBoughtConsumable((unsigned int) consumable);
}

void Java_net_fishlabs_googleplay_ToJNI_iapSetNativeItemInformationList(JNIEnv *env, jclass clazz,
                                                                        jobjectArray ids,
                                                                        jobjectArray names,
                                                                        jobjectArray descriptions,
                                                                        jobjectArray currencies,
                                                                        jobjectArray prices) {
    ndk_setNativeItemInformationList(env, clazz, ids, names, descriptions, currencies, prices);
}

void Java_net_fishlabs_googleplay_ToJNI_iapResetNativeItemInformationList(JNIEnv *env, jclass clazz) {
    ndk_resetNativeItemInformationList();
}

void Java_net_fishlabs_tapjoy_ToJNI_spentAmountOfCredits(JNIEnv *env, jclass clazz, jint amount) {
    gb_android_offerwallCreditAmount = amount;
    ndk_checkPlaytimeAndSpendOfferwallCredits();
}

jint Java_net_fishlabs_playhaven_ToJNI_getCurrentApplicationModule(JNIEnv *env, jclass clazz) {
    return ndk_getCurrentApplicationModule();
}

jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_getLogoShown(JNIEnv *env, jclass clazz) {
    return ndk_getLogoShown();
}

jint Java_net_fishlabs_gof2hdallandroid2012_ToJNI_isInMainMenu(JNIEnv *env, jclass clazz) {
    return ndk_isInMainMenu();
}

jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetLeaderboardScore(JNIEnv *env, jclass clazz,
                                                                           jint index) {
    return g_android_leaderboard_scores[index];
}

void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetLeaderboardScore(JNIEnv *env, jclass clazz,
                                                                             jint index) {
    g_android_leaderboard_scores[index] = 0;
}

jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetAchievementId(JNIEnv *env, jclass clazz, jint index) {
    return g_android_current_achievements[index];
}

void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetAchievements(JNIEnv *env, jclass clazz) {
    g_android_current_achievements[0] = 0;
    g_android_current_achievements[1] = 0;
    g_android_current_achievements[2] = 0;
}

jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetShowAchievements(JNIEnv *env, jclass clazz) {
    return g_android_show_achievements;
}

void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetShowAchievements(JNIEnv *env, jclass clazz) {
    g_android_show_achievements = 0;
}

jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetLinkGameGP(JNIEnv *env, jclass clazz) {
    return g_android_link_game_gp;
}

void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetLinkGameGP(JNIEnv *env, jclass clazz) {
    g_android_link_game_gp = 0;
}

jint Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetShowLeaderboards(JNIEnv *env, jclass clazz) {
    return g_android_show_leaderboards;
}

void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ResetShowLeaderboards(JNIEnv *env, jclass clazz) {
    g_android_show_leaderboards = 0;
}

void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_SetGPIsLinked(JNIEnv *env, jclass clazz, jint linked) {
    g_android_gp_is_linked = linked;
}
