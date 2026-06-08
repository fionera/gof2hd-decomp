#include "class.h"

extern "C" int ApplicationManager_GetElapsedTimeMillis();                // GetElapsedTimeMillis
// The entire per-frame game tick — free-cam keyboard input, AI/physics step,
// mining/hacking/docking state machines, dialogue + sound pumps, jump-scene update,
// station-service transitions and their string building — is one massive body that
// Ghidra renders with deeply-corrupt SIMD/vector pseudo-ops.  It is translated as a
// single documented helper so the entry point stays faithful and compiling; the
// helper receives the clamped frame-delta and performs all field writes / engine
// calls on `self`.
extern "C" void MGame_tick(MGame *self, int frameDeltaMs);

__attribute__((visibility("hidden"))) extern int *g_upGuard;   // @0x18c8ee (stack guard [0])
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

// MGame::OnUpdate(): advance the whole game one frame.
extern "C" void MGame_OnUpdate(MGame *self) {
    int *guard = g_upGuard;
    int g0 = *guard;

    // Clamp this frame's elapsed time to [0, 0x96] ms (cap large hitches).
    int delta;
    int t = ApplicationManager_GetElapsedTimeMillis();
    if (t < 0x97) {
        delta = (ApplicationManager_GetElapsedTimeMillis() < 0) ? 0
                : (ApplicationManager_GetElapsedTimeMillis() < 0x97
                       ? ApplicationManager_GetElapsedTimeMillis()
                       : 0x96);
    } else {
        delta = 0x96;
    }

    F<int>(self, 0x40) = delta;
    // Accumulate into the 4-wide playtime counter at 0x30 (lane 0 gets +delta).
    F<int>(self, 0x30) += delta;

    // Run the full per-frame update.
    MGame_tick(self, delta);

    if (*guard != g0)
        __stack_chk_fail();
}
