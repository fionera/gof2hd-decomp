// In-app-purchase credit-pack handling and JNI string glue, recovered from the
// Android binary (ghidra_addr 0xa462c..0xa482e). The symbols are all unmangled
// C-linkage helpers shared by the purchase callbacks and the Android JNI bridge.
#include "platform/recovered_a462c.h"

#include <cstdlib>
#include <cstring>

#include "game/mission/Status.h"
#include "game/mission/RecordHandler.h"

// Globals::status is the canonical Status singleton (binary .bss 0x2281b0) under
// its real mangled name; Globals::options is the inline 100-byte game-options
// record (binary .bss 0x2281e8) whose byte at offset 0x62 is the one-shot
// "first credit pack bought" flag. Neither is declared in Globals.h, so reach
// them here through their exact mangled symbols.
extern Status *Globals_status asm("_ZN7Globals6statusE");
extern unsigned char Globals_options[100] asm("_ZN7Globals7optionsE");

// The five "credit pack purchase pending" flags set by the IAP UI when a pack
// button is pressed; setBaughtCredits clears the one that was just fulfilled.
extern "C" int gi_iap_buy_credit_pack1_pressed;
extern "C" int gi_iap_buy_credit_pack2_pressed;
extern "C" int gi_iap_buy_credit_pack3_pressed;
extern "C" int gi_iap_buy_credit_pack4_pressed;
extern "C" int gi_iap_buy_credit_pack5_pressed;

// Host callback that persists the game after a purchase.
extern "C" void ndk_autosave();

int setBaughtCredits(int amount)
{
    int *packFlag;
    if (amount == 10000000) {
        Globals_status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack1_pressed;
    } else if (amount == 300000) {
        Globals_status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack2_pressed;
    } else if (amount == 1000000) {
        Globals_status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack3_pressed;
    } else if (amount == 3000000) {
        Globals_status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack4_pressed;
    } else if (amount == 100000) {
        Globals_status->changeCredits(amount);
        packFlag = &gi_iap_buy_credit_pack5_pressed;
    } else {
        return 0;
    }
    // On first fulfilment of a still-pending pack, run the one-shot
    // first-purchase bookkeeping; then persist the game.
    if (*packFlag != 0) {
        *packFlag = 0;
        checkFirstCreditPackBoughtWriteAction();
    }
    ndk_autosave();
    return 1;
}

void checkFirstCreditPackBoughtWriteAction()
{
    // Return value is intentionally discarded: the call is kept for parity with
    // the original, which reads the campaign mission as a side effect anchor.
    Globals_status->getCurrentCampaignMission();
    if (Globals_options[0x62] == 0) {
        Globals_options[0x62] = 1;
        RecordHandler handler;
        handler.saveOptions();
    }
}

const char *getStringUTFChars(JNIEnv *env, jstring str)
{
    if (env != nullptr && str != nullptr) {
        // Tail-jump straight to the GetStringUTFChars dispatch slot (index 169,
        // byte offset 0x2a4) leaving env/str in place; the original never passes
        // an isCopy argument.
        return reinterpret_cast<const char *(*)(JNIEnv *, jstring)>(
            env->functions->GetStringUTFChars)(env, str);
    }
    return nullptr;
}

void releaseStringUTFChars(JNIEnv *env, jstring str, const char *chars)
{
    if (env != nullptr && str != nullptr && chars != nullptr) {
        env->ReleaseStringUTFChars(str, chars);
        return;
    }
    if (chars != nullptr) {
        operator delete(const_cast<char *>(chars));
    }
    if (str != nullptr) {
        operator delete(reinterpret_cast<void *>(str));
    }
}

char *pConstToNonConst(const char *s)
{
    if (s != nullptr) {
        char *copy = static_cast<char *>(std::malloc(std::strlen(s) + 1));
        if (copy != nullptr) {
            std::strcpy(copy, s);
            return copy;
        }
    }
    return nullptr;
}
