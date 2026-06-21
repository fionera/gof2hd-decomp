// Virtual on-screen keyboard / mouse-emulation glue recovered from the Android
// binary (ghidra_addr 0x7a41c..0x7b2d4). See recovered_7a41c.h for the overview.
//
// The desktop-style input layer keeps a private table of on-screen "virtual
// keys" plus a software pointer and three mouse-wheel accumulators. Everything
// here is module-private state, so it lives as file-scope statics shared by the
// helpers below; only the helpers themselves are exported (under their original
// global symbols).
#include "platform/recovered_7a41c.h"

#include "engine/core/ApplicationManager.h"   // gAppManager, current module id

#include <cstring>
#include <initializer_list>

namespace {

// ---------------------------------------------------------------------------
// Virtual-key table.
// ---------------------------------------------------------------------------
// One entry per on-screen key (button stride 0x58 in the binary). Only the
// fields touched by the recovered helpers are named; the binary layout is
// preserved so the table can be shared with the (deferred) press/draw paths.
struct VirtualKey {
    char name[0x14];   // +0x00 key label, matched by keyEventReleased
    int pressed;       // +0x14 1 while held
    int keyCode;       // +0x18 game key code delivered on release
    int altCode1;      // +0x1c
    int altCode2;      // +0x20
    int altCode3;      // +0x24
    int hasTouch;      // +0x28 currently driving a synthetic touch
    int field_0x2c;
    int baseX;         // +0x30
    int baseY;         // +0x34
    int offsetX;       // +0x38
    int offsetY;       // +0x3c
    void *touch;       // +0x40 touch handle
    int field_0x44;
    int field_0x48;
    int action;        // +0x4c follow-up action code
    int field_0x50;
    int field_0x54;
};

// 48 on-screen keys (0x1080 bytes / 0x58 stride).
constexpr int kVirtualKeyCount = 48;
VirtualKey g_virtualKeys[kVirtualKeyCount];

// The four directional keys live at the front of the table (entries 0..3); the
// state queries reach them through this base pointer, which the layout code
// points at g_virtualKeys.
VirtualKey *const g_arrowKeys = g_virtualKeys;

// The action/fire key's pressed flag (a separate single-key global in the
// binary).
int g_actionKeyState;

// ---------------------------------------------------------------------------
// Delayed key-up queue (drained by SendStoredKeyUpEvents).
// ---------------------------------------------------------------------------
constexpr int kDelayedKeyMax = 0x401;
int g_delayedKeys[kDelayedKeyMax];
int g_delayedKeyCount;

// ---------------------------------------------------------------------------
// Software pointer + mouse-wheel state.
// ---------------------------------------------------------------------------
// Three wheel accumulators decayed each frame by LowerMouseWheel; the factor is
// stored as a double constant in the binary (~0.78f) and rounds toward zero.
constexpr double kWheelDecay = 0.78;

int g_wheelAccumA;
int g_wheelAccumB;
int g_wheelAccumC;

// In-game zoom request driven by large wheel notches: +1 on a strong scroll
// down, -1 on a strong scroll up, consumed by the game module on the next frame.
int g_zoomRequest;

// True while the running module is the in-game flight module (module id 2). The
// software wheel only drives zoom while the player is actually flying.
inline bool inFlightModule()
{
    return gAppManager->currentModuleId == 2;
}

// Software pointer. The binary reaches each axis through its own indirection,
// so they are kept as separate ints behind module-private pointers; modelled
// here as plain ints (their addresses are what the rest of the input layer
// shares). `g_pointerEmulationEnabled` gates all pointer motion, and while
// `g_pointerCaptured` is set the x axis is written absolutely instead of being
// integrated.
int g_pointerEmulationEnabled;
int g_pointerCaptured;
int g_pointerX;
int g_pointerY;
int g_pointerBaseY;
int g_pointerCaptureX;

// ---------------------------------------------------------------------------
// 32-bit hash finaliser table.
// ---------------------------------------------------------------------------
// Four 256-entry columns laid out contiguously (column c at byte offset
// c * 0x400). Resolved at load time by the asset loader; null until then.
const unsigned int *const *g_hashTable;

} // namespace

// ---------------------------------------------------------------------------
// F: per-byte S-box mix of a 32-bit word.
// ---------------------------------------------------------------------------
extern "C" unsigned int F(unsigned int value)
{
    const unsigned int *table = *g_hashTable;
    const unsigned int *col0 = table;            // +0x000
    const unsigned int *col1 = table + 0x100;    // +0x400
    const unsigned int *col2 = table + 0x200;    // +0x800
    const unsigned int *col3 = table + 0x300;    // +0xc00

    const unsigned int byte0 = value & 0xff;
    const unsigned int byte1 = (value >> 8) & 0xff;
    const unsigned int byte2 = (value >> 16) & 0xff;
    const unsigned int byte3 = value >> 24;

    return (col2[byte1] ^ (col0[byte3] + col1[byte2])) + col3[byte0];
}

// ---------------------------------------------------------------------------
// Software-pointer move.
// ---------------------------------------------------------------------------
void MouseInput(int dx, int dy)
{
    if (!g_pointerEmulationEnabled)
        return;

    if (g_pointerCaptured) {
        g_pointerCaptureX = dx;
    } else {
        g_pointerX += dx;
        dy += g_pointerBaseY;
    }
    g_pointerY = dy;
}

// ---------------------------------------------------------------------------
// Per-frame wheel decay.
// ---------------------------------------------------------------------------
// Each accumulator decays toward zero by kWheelDecay, with the scaled value
// clamped so it never crosses zero (matches the binary's same-sign rounding).
void LowerMouseWheel()
{
    for (int *accum : {&g_wheelAccumA, &g_wheelAccumB, &g_wheelAccumC}) {
        if (*accum > 0) {
            int scaled = static_cast<int>(static_cast<double>(*accum) * kWheelDecay);
            *accum = scaled < 0 ? 0 : scaled;   // clamp negatives to 0
        } else if (*accum < 0) {
            int scaled = static_cast<int>(static_cast<double>(*accum) * kWheelDecay);
            *accum = scaled > 0 ? 0 : scaled;   // clamp positives to 0
        }
        // *accum == 0 stays 0
    }
}

// ---------------------------------------------------------------------------
// Mouse-wheel notch.
// ---------------------------------------------------------------------------
// A strong notch (|delta| > 4) while flying posts a one-shot zoom request, and
// the whole notch (`delta` + the fractional `residual`) is folded into the wheel
// accumulator that the rest of the input layer reads.
void MouseWheel(float delta, float residual)
{
    if (delta < -4.0f && inFlightModule())
        g_zoomRequest = 1;

    if (delta > 4.0f && inFlightModule())
        g_zoomRequest = -1;

    g_wheelAccumA = static_cast<int>(static_cast<float>(g_wheelAccumA) + delta + residual);
}

// ---------------------------------------------------------------------------
// Virtual-key release helpers.
// ---------------------------------------------------------------------------
void keyReleasedWithDelay(AbyssEngine::Engine * /*engine*/, int key)
{
    if (g_delayedKeyCount > kDelayedKeyMax - 1)
        return;
    g_delayedKeys[g_delayedKeyCount++] = key;
}

void SendStoredKeyUpEvents(AbyssEngine::Engine *engine)
{
    for (int i = 0; i < g_delayedKeyCount; ++i)
        keyReleased(engine, g_delayedKeys[i]);
    g_delayedKeyCount = 0;
}

void keyEventReleased(AbyssEngine::Engine *engine, char *name)
{
    for (int i = 0; i < kVirtualKeyCount; ++i) {
        VirtualKey &key = g_virtualKeys[i];
        if (std::strcmp(key.name, name) == 0)
            keyReleased(engine, key.keyCode);
    }
}

void keyEventPressed(AbyssEngine::Engine *engine, char *name)
{
    for (int i = 0; i < kVirtualKeyCount; ++i) {
        VirtualKey &key = g_virtualKeys[i];
        if (std::strcmp(key.name, name) == 0)
            keyPressed(engine, key.keyCode);
    }
}

// ---------------------------------------------------------------------------
// Keyboard / mouse state queries.
// ---------------------------------------------------------------------------
bool ArrowKeyPressed()
{
    return g_arrowKeys[0].pressed != 0 || g_arrowKeys[1].pressed != 0 ||
           g_arrowKeys[2].pressed != 0 || g_arrowKeys[3].pressed != 0;
}

bool keyIsPressed()
{
    return ArrowKeyPressed() || g_actionKeyState != 0;
}

// ---------------------------------------------------------------------------
// No-op input-table hooks (the Android build has a fixed on-screen layout and
// no keyboard animation, so the originals are empty bodies).
// ---------------------------------------------------------------------------
void KeyboardAnimationTimer(AbyssEngine::Engine * /*engine*/)
{
}

void actualizeButtonPositions(AbyssEngine::Engine * /*engine*/)
{
}
