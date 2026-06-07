#include "class.h"

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
