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
#include "engine/render/Engine.h"             // Engine (appManager, DrawQuad, field_0x360)

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
    // +0x40 touch handle while driving a synthetic touch; the sub-menu gate
    // instead reads it as a biased touch-window handle (see IsSubMenuActive), so
    // the slot is modelled as a union of both views.
    union {
        void *touch;
        int   touchWindowHandle;
    };
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
// Touch-window / sub-menu gating (read by IsSubMenuActive).
// ---------------------------------------------------------------------------
// A virtual key's `touch` slot holds, while it is driving a synthetic touch, a
// 1-based window handle biased by kTouchWindowBase: handle h is stored as
// kTouchWindowBase + 1 + h. IsSubMenuActive maps the handle back and asks the
// touch-window system whether that window is a blocking sub-menu/dialog.
constexpr int kTouchWindowBase = 0x578;

// Number of registered touch windows; a handle past this is stale -> not active.
// Modelled, like the rest of this group, as a pointer to the live subsystem
// global the binary reaches through its GOT slot.
int *g_touchWindowCount;

// The window subsystem's current dialog/menu state. `g_touchWindowMode` is the
// active window kind (0 = none); `g_touchWindowFlags` is a sub-state. When a
// window kind is active the gate only fires for kinds outside {1,2}.
int *g_touchWindowMode;
int *g_touchWindowFlags;

// Index of the focused list/menu entry, or -1 when nothing is focused.
int *g_touchWindowFocus;

// ---------------------------------------------------------------------------
// 32-bit hash finaliser table.
// ---------------------------------------------------------------------------
// Four 256-entry columns laid out contiguously (column c at byte offset
// c * 0x400). Resolved at load time by the asset loader; null until then.
const unsigned int *const *g_hashTable;

// ---------------------------------------------------------------------------
// Steering / pointer-recentre globals written by keyReleased.
// ---------------------------------------------------------------------------
// keyReleased recentres the relevant steering axis when a directional key is
// let go: if the opposite-direction arrow key is still held the axis is nudged
// back by one step (+/-0.1), otherwise it snaps to its 0.5 midpoint. Each axis
// lives at a fixed offset inside its own module-private steering struct.
constexpr float kAxisCenter = 0.5f;          // 0x3f000000
constexpr float kAxisStep   = 0.1f;          // 0x3dcccccd

struct SteeringAxis {
    int   field_0x00;
    float axis1;   // +0x04 written by release cases 0 and 1
    float axis2;   // +0x08 written by release cases 2 and 3
};
// Two direct module-private steering structs (the binary addresses them through
// PC-relative GOT-free literals, i.e. plain globals rather than indirections).
SteeringAxis g_steeringX;   // case 0/1 recentre target
SteeringAxis g_steeringY;   // case 2/3 recentre target

// The four reset pointers keyReleased clears on every call (pending wheel/zoom
// requests and the firing flag) and the follow-up action sink.
int *g_pendingZoom;
int *g_pendingWheelA;
int *g_pendingWheelB;
int *g_pendingFire;

// Follow-up action posted when a key with a non-zero `action` is released; a
// direct module-private global the game module consumes next frame.
int g_followUpAction;

// A secondary firing/aim flag cleared for the boost/brake keys (release cases
// 6 and 7).
int *g_aimAssist;

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
// Key-code binding.
// ---------------------------------------------------------------------------
// Binds `code` onto every virtual key whose label matches `name`. `slot` 0 is
// the primary game key code; slots 1..3 are the three alternates.
void SetKeyCode(const char *name, int slot, int code)
{
    if (name == nullptr)
        return;

    for (VirtualKey &key : g_virtualKeys) {
        if (std::strcmp(key.name, name) != 0)
            continue;
        switch (slot) {
        case 0: key.keyCode  = code; break;
        case 1: key.altCode1 = code; break;
        case 2: key.altCode2 = code; break;
        case 3: key.altCode3 = code; break;
        }
    }
}

// ---------------------------------------------------------------------------
// Sub-menu / dialog gating.
// ---------------------------------------------------------------------------
// While a directional virtual key drives a synthetic touch, its `touch` slot
// holds a biased window handle. This reports whether that window is a blocking
// sub-menu/dialog (so flight steering should be suppressed). A handle at or
// below the bias means "no window" -> not active.
int IsSubMenuActive(int player)
{
    const int handle = g_virtualKeys[player].touchWindowHandle;
    if (handle <= kTouchWindowBase)
        return false;

    const int window = handle - (kTouchWindowBase + 1);
    if (window >= *g_touchWindowCount)
        return false;

    if (*g_touchWindowMode != 0) {
        // An active window kind blocks unless it is one of the two pass-through
        // kinds (1 or 2).
        const unsigned int mode = static_cast<unsigned int>(*g_touchWindowFlags - 1);
        return mode > 1u ? 1 : 0;
    }

    if (*g_touchWindowFocus != -1)
        return true;

    // Nothing focused: only the store module (id 5) counts as a sub-menu here.
    return gAppManager->currentModuleId == 5 ? 1 : 0;
}

// ---------------------------------------------------------------------------
// Virtual-key release.
// ---------------------------------------------------------------------------
// Clears the key's pressed flag, ends the synthetic touch it was driving, draws
// the release feedback quad, runs the key's follow-up action and recentres the
// affected steering axis. The four pending wheel/zoom/fire requests are reset on
// every call.
void keyReleased(AbyssEngine::Engine *engine, int key)
{
    *g_pendingZoom = 0;
    *g_pendingWheelA = 0;
    *g_pendingWheelB = 0;
    *g_pendingFire = 0;

    if (engine == nullptr)
        return;

    // Index of the first low-numbered key (< 0xb) that was released; selects the
    // steering recentre below.
    int releasedIndex = -1;

    for (int i = 0; i < kVirtualKeyCount; ++i) {
        VirtualKey &vk = g_virtualKeys[i];
        if (vk.keyCode != key && vk.altCode1 != key && vk.altCode2 != key &&
            vk.altCode3 != key)
            continue;

        if (vk.pressed != 1)
            continue;
        vk.pressed = 0;

        if (i < 0xb)
            releasedIndex = i;

        if (vk.hasTouch == 0)
            continue;

        engine->appManager->OnTouchEnd(vk.baseX + vk.offsetX, vk.baseY + vk.offsetY,
                                       vk.touch);
        engine->appManager->OnTouchEndSimple();
        engine->DrawQuad(vk.offsetX, vk.offsetY, 10, 10);
        if (vk.action != 0)
            g_followUpAction = vk.action;
    }

    // Steering recentre. Each released directional key snaps its axis to the
    // midpoint unless the opposite arrow key is still held, in which case the axis
    // is nudged one step toward that direction instead.
    switch (releasedIndex) {
    case 0:  // released "up": fall back toward "down" if still held
        g_steeringX.axis1 = (g_virtualKeys[1].pressed == 0) ? kAxisCenter : kAxisStep;
        break;
    case 1:  // released "down": fall back toward "up" if still held
        g_steeringX.axis1 = (g_virtualKeys[0].pressed == 0) ? kAxisCenter : -kAxisStep;
        break;
    case 2:  // released "left": fall back toward "right" if still held
        g_steeringY.axis2 = (g_virtualKeys[3].pressed == 0) ? kAxisCenter : -kAxisStep;
        break;
    case 3:  // released "right": fall back toward "left" if still held
        g_steeringY.axis2 = (g_virtualKeys[2].pressed == 0) ? kAxisCenter : kAxisStep;
        break;
    case 6:
    case 7:
        *g_aimAssist = 0;
        break;
    default:
        break;
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
