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

}

#endif // GOF2_RECOVERED_A462C_H
