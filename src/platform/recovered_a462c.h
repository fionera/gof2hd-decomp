#ifndef GOF2_RECOVERED_A462C_H
#define GOF2_RECOVERED_A462C_H

// In-app-purchase / JNI string glue recovered from the Android binary
// (ghidra_addr 0xa462c..0xa482e). These are the C-linkage helpers the credit-pack
// purchase callback and the Android JNI bridge call into; the symbols are
// unmangled, so they live in this header under `extern "C"`.
#include <jni.h>

extern "C" {

// Credit-pack purchase handler. `amount` is the credit value of the pack that was
// bought (one of the five fixed pack sizes); awards that many credits, clears the
// matching gi_iap_buy_credit_packN_pressed flag and, on the first ever purchase,
// records the one-shot "first credit pack bought" option. Returns 1 on a known
// pack, 0 otherwise.
int setBaughtCredits(int amount);

// One-shot bookkeeping for the very first credit-pack purchase: flips the
// "first credit pack bought" option byte in Globals::options and persists the
// options record. A no-op on every later call.
void checkFirstCreditPackBoughtWriteAction();

// Thin JNI helpers. Each guards against null arguments before forwarding to the
// matching JNINativeInterface slot.
const char *getStringUTFChars(JNIEnv *env, jstring str);
void releaseStringUTFChars(JNIEnv *env, jstring str, const char *chars);

// strdup-style copy of a C string into a freshly malloc'd, mutable buffer
// (the name reflects its const-to-non-const purpose). Returns nullptr on a null
// input or allocation failure.
char *pConstToNonConst(const char *s);

// ---------------------------------------------------------------------------
// NDK frame/state hooks called from the Java side. Each one short-circuits when
// the engine has not been brought up yet (the *g_pEngine slot is still null).
// ---------------------------------------------------------------------------

// Suspend/resume the audio engine and (on suspend) the running application
// module. Sent by the Android activity's onPause/onResume.
void ndk23_sendingPauseSignal();
void ndk23_sendingResumeSignal();

// Persist the current game and its preview slot (slot 0). Called after the host
// completes a purchase or otherwise needs a checkpoint.
void ndk_autosave();

// Index of the currently active application module, or -1 if the engine is not
// up yet.
int ndk_getCurrentApplicationModule();

// Whether the boot logo has finished and whether the title/main menu is showing.
int ndk_getLogoShown();
int ndk_isInMainMenu();

// Per-DLC "already bought" flags, read back by the store UI. Each returns the
// matching Globals::options byte (false while the engine is down).
bool ndk_getDLC_1_BOUGHT();
bool ndk_getDLC_2_BOUGHT();
bool ndk_getDLC_3_BOUGHT();
bool ndk_getDLC_4_BOUGHT();
bool ndk_getDLC_5_BOUGHT();

// In-app-purchase completion callbacks from the Android billing bridge.
// `consumable` maps a credit-pack index (0..4) to its credit value and awards it.
// `premium` records the bought/refunded state of DLC pack `pack` (0..4).
void ndk_iapBoughtConsumable(unsigned int consumable);
void ndk_iapBoughtPremium(unsigned int pack, unsigned int bought);

// Populate the cached store item list (id/name/description/currency/price for the
// five products) from the Java string arrays handed to the native method.
void ndk_setNativeItemInformationList(JNIEnv *env, jclass clazz,
                                      jobjectArray ids, jobjectArray names,
                                      jobjectArray descriptions,
                                      jobjectArray currencies,
                                      jobjectArray prices);

}

#endif // GOF2_RECOVERED_A462C_H
