#include "class.h"

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
