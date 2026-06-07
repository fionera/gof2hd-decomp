#include "class.h"

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
