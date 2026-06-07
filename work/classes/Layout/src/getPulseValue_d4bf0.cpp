#include "class.h"

// Status singleton (PC-rel double-indirect). getPlayingTime returns a 64-bit value
// (r0/r1), converted to float via __aeabi_l2f, scaled by the arg, fed to Sinf.
struct Status;
__attribute__((visibility("hidden"))) extern Status **gStatus;  // ldr [0xe4c5c]
extern "C" long long Status_getPlayingTime(Status *st);  // blx 0x71710
extern "C" float __aeabi_l2f(long long);
extern "C" float Sinf(float);   // AbyssEngine::AEMath::Sinf -> 0x6f1a8

// Layout::getPulseValue(float speed). The holder (gStatus's value) is cached; the
// actual Status* (*holder) is reloaded for each getPlayingTime call.
extern "C" float Layout_getPulseValue(Layout *self, float speed) {
    Status **holder = gStatus;
    float a = Sinf(__aeabi_l2f(Status_getPlayingTime(*holder)) * speed);
    float b = Sinf(__aeabi_l2f(Status_getPlayingTime(*holder)) * speed);
    return a > 0.0f ? b : -b;
}
