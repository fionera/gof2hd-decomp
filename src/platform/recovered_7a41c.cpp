

#include "platform/recovered_7a41c.h"

#include "engine/core/ApplicationManager.h"   // gAppManager, current module id
#include "engine/math/AEMath.h"                // VectorSignedToFloat (vertex dequantizer)
#include "engine/core/NFC.h"                   // IsInGameSubMenuNotActive
#include "engine/render/Engine.h"             // Engine (appManager, DrawQuad, field_0x360)
#include "game/core/Globals.h"                // Globals UI-state flags / mouse toggles

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <initializer_list>

namespace {
    struct VirtualKey {
        char name[0x14]; // +0x00 key label, matched by keyEventReleased
        int pressed; // +0x14 1 while held
        int keyCode; // +0x18 game key code delivered on release
        int altCode1; // +0x1c
        int altCode2; // +0x20
        int altCode3; // +0x24
        int hasTouch; // +0x28 currently driving a synthetic touch
        int field_0x2c;
        int baseX; // +0x30
        int baseY; // +0x34
        int offsetX; // +0x38
        int offsetY; // +0x3c
        // +0x40 touch handle while driving a synthetic touch; the sub-menu gate
        // instead reads it as a biased touch-window handle (see IsSubMenuActive), so
        // the slot is modelled as a union of both views.
        union {
            void *touch;
            int touchWindowHandle;
        };

        int field_0x44;
        int field_0x48;
        int action; // +0x4c follow-up action code
        int field_0x50;
        int field_0x54;
    };

    constexpr int kVirtualKeyCount = 48;
    VirtualKey g_virtualKeys[kVirtualKeyCount];

    VirtualKey *const g_arrowKeys = g_virtualKeys;

    int g_actionKeyState;

    constexpr int kDelayedKeyMax = 0x401;
    int g_delayedKeys[kDelayedKeyMax];
    int g_delayedKeyCount;

    constexpr double kWheelDecay = 0.78;

    int g_wheelAccumA;
    int g_wheelAccumB;
    int g_wheelAccumC;

    int g_zoomRequest;

    inline bool inFlightModule() {
        return gAppManager->currentModuleId == 2;
    }

    int g_pointerEmulationEnabled;
    int g_pointerCaptured;
    int g_pointerX;
    int g_pointerY;
    int g_pointerBaseY;
    int g_pointerCaptureX;

    constexpr int kTouchWindowBase = 0x578;

    int *g_touchWindowCount;

    int *g_touchWindowMode;
    int *g_touchWindowFlags;

    int *g_touchWindowFocus;

    const unsigned int *const *g_hashTable;

    constexpr float kAxisCenter = 0.5f; // 0x3f000000
    constexpr float kAxisStep = 0.1f; // 0x3dcccccd

    struct SteeringAxis {
        int field_0x00;
        float axis1; // +0x04 written by release cases 0 and 1
        float axis2; // +0x08 written by release cases 2 and 3
    };

    SteeringAxis g_steeringX; // case 0/1 recentre target
    SteeringAxis g_steeringY; // case 2/3 recentre target

    int *g_pendingZoom;
    int *g_pendingWheelA;
    int *g_pendingWheelB;
    int *g_pendingFire;

    int g_followUpAction;

    int *g_aimAssist;

    int *g_pressWheelRequest; // boost/brake nudge target (+/-1, then +/-0.01)
    int *g_pressFireFlag; // single-shot fire flag raised on a press

    struct ModuleInputState {
        int field_0x00;
        int field_0x04;
        int comboProgress; // +0x08 recognised-sequence progress (0..9)
        int requiredTouch; // +0x0c touch handle a matched key must report
    };

    ModuleInputState *const *g_moduleInputState;

    char *g_accelHeld;
    char *g_decelHeld;

    int g_mousePointerLatch = 1;

    struct SoftwareStick {
        float inputX; // 0x2250a4 raw centre X (0.5 = neutral)
        float inputY; // 0x2250a8 raw centre Y (0.5 = neutral)
        float easedX; // 0x2250ac eased output X in [0,1]
        float easedY; // 0x2250b0 eased output Y in [0,1]
        float savedX; // 0x2250b4 saved output X (joystick hold)
        float savedY; // 0x2250b8 saved output Y (joystick hold)
    };

    SoftwareStick g_stick = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f};

    int g_useJoystick;

    int g_simEdgesInit;

    unsigned char g_simTouchActive;
} // namespace

extern "C" unsigned int F(unsigned int value) {
    const unsigned int *table = *g_hashTable;
    const unsigned int *col0 = table; // +0x000
    const unsigned int *col1 = table + 0x100; // +0x400
    const unsigned int *col2 = table + 0x200; // +0x800
    const unsigned int *col3 = table + 0x300; // +0xc00

    const unsigned int byte0 = value & 0xff;
    const unsigned int byte1 = (value >> 8) & 0xff;
    const unsigned int byte2 = (value >> 16) & 0xff;
    const unsigned int byte3 = value >> 24;

    return (col2[byte1] ^ (col0[byte3] + col1[byte2])) + col3[byte0];
}

void MouseInput(int dx, int dy) {
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

void LowerMouseWheel() {
    for (int *accum: {&g_wheelAccumA, &g_wheelAccumB, &g_wheelAccumC}) {
        if (*accum > 0) {
            int scaled = static_cast<int>(static_cast<double>(*accum) * kWheelDecay);
            *accum = scaled < 0 ? 0 : scaled; // clamp negatives to 0
        } else if (*accum < 0) {
            int scaled = static_cast<int>(static_cast<double>(*accum) * kWheelDecay);
            *accum = scaled > 0 ? 0 : scaled; // clamp positives to 0
        }
        // *accum == 0 stays 0
    }
}

void MouseWheel(float delta, float residual) {
    if (delta < -4.0f && inFlightModule())
        g_zoomRequest = 1;

    if (delta > 4.0f && inFlightModule())
        g_zoomRequest = -1;

    g_wheelAccumA = static_cast<int>(static_cast<float>(g_wheelAccumA) + delta + residual);
}

void keyReleasedWithDelay(AbyssEngine::Engine * /*engine*/, int key) {
    if (g_delayedKeyCount > kDelayedKeyMax - 1)
        return;
    g_delayedKeys[g_delayedKeyCount++] = key;
}

void SendStoredKeyUpEvents(AbyssEngine::Engine *engine) {
    for (int i = 0; i < g_delayedKeyCount; ++i)
        keyReleased(engine, g_delayedKeys[i]);
    g_delayedKeyCount = 0;
}

void keyEventReleased(AbyssEngine::Engine *engine, char *name) {
    for (int i = 0; i < kVirtualKeyCount; ++i) {
        VirtualKey &key = g_virtualKeys[i];
        if (std::strcmp(key.name, name) == 0)
            keyReleased(engine, key.keyCode);
    }
}

void keyEventPressed(AbyssEngine::Engine *engine, char *name) {
    for (int i = 0; i < kVirtualKeyCount; ++i) {
        VirtualKey &key = g_virtualKeys[i];
        if (std::strcmp(key.name, name) == 0)
            keyPressed(engine, key.keyCode);
    }
}

void SetKeyCode(const char *name, int slot, int code) {
    if (name == nullptr)
        return;

    for (VirtualKey &key: g_virtualKeys) {
        if (std::strcmp(key.name, name) != 0)
            continue;
        switch (slot) {
            case 0: key.keyCode = code;
                break;
            case 1: key.altCode1 = code;
                break;
            case 2: key.altCode2 = code;
                break;
            case 3: key.altCode3 = code;
                break;
        }
    }
}

int IsSubMenuActive(int player) {
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

void keyReleased(AbyssEngine::Engine *engine, int key) {
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
        engine->appManager->OnTouchEnd();
        engine->DrawQuad(vk.offsetX, vk.offsetY, 10, 10);
        if (vk.action != 0)
            g_followUpAction = vk.action;
    }

    // Steering recentre. Each released directional key snaps its axis to the
    // midpoint unless the opposite arrow key is still held, in which case the axis
    // is nudged one step toward that direction instead.
    switch (releasedIndex) {
        case 0: // released "up": fall back toward "down" if still held
            g_steeringX.axis1 = (g_virtualKeys[1].pressed == 0) ? kAxisCenter : kAxisStep;
            break;
        case 1: // released "down": fall back toward "up" if still held
            g_steeringX.axis1 = (g_virtualKeys[0].pressed == 0) ? kAxisCenter : -kAxisStep;
            break;
        case 2: // released "left": fall back toward "right" if still held
            g_steeringY.axis2 = (g_virtualKeys[3].pressed == 0) ? kAxisCenter : -kAxisStep;
            break;
        case 3: // released "right": fall back toward "left" if still held
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

void keyPressed(AbyssEngine::Engine *engine, int key) {
    if (engine == nullptr)
        return;

    ModuleInputState *module = *g_moduleInputState;

    // --- Step 1: advance the recognised key sequence. -----------------------
    // Each transition fires only when the expected key follows the current
    // progress value; key 14 (re)starts the sequence, and a completed sequence
    // (progress 9) is left untouched.
    int progress = module->comboProgress;
    if (key == 14) {
        switch (progress) {
            case 4: progress = 5;
                break;
            case 6: progress = 7;
                break;
            case 9: break; // already complete: leave as-is
            default: progress = 1;
                break;
        }
    } else if (key == 0x40000000 && progress == 1) {
        progress = 2;
    } else if (key == 1 && progress == 2) {
        progress = 3;
    } else if (key == 0x11 && progress == 3) {
        progress = 4;
    } else if (key == 0xf && progress == 5) {
        progress = 6;
    } else if (key == 5 && progress == 7) {
        progress = 8;
    } else if (key == 5 && progress == 8) {
        progress = 9;
    } else if (progress != 9) {
        progress = 0;
    }
    if (progress != 9 || module->comboProgress != 9)
        module->comboProgress = progress;

    // --- Step 2: drive the matching on-screen key. --------------------------
    const int currentModule = static_cast<int>(gAppManager->currentModuleId);
    int matchedIndex = -1;

    for (int pass = 2; pass >= 0; --pass) {
        for (int i = 0; i < kVirtualKeyCount; ++i) {
            VirtualKey &vk = g_virtualKeys[i];
            if (vk.field_0x50 != pass)
                continue;
            if (vk.keyCode != key && vk.altCode1 != key && vk.altCode2 != key &&
                vk.altCode3 != key)
                continue;
            if (vk.pressed != 0)
                continue;
            // The key only fires inside its bound module (-1 == any module).
            if (vk.field_0x44 != currentModule && vk.field_0x44 + 1 != 0)
                continue;
            // ...and, if it carries a required touch handle, only while that
            // handle is the module's active one.
            if (vk.field_0x48 != 0 && module->requiredTouch != vk.field_0x48)
                continue;
            // Optional per-key predicate gate.
            if (vk.action != 0) {
                auto predicate = reinterpret_cast<int (*)(int)>(vk.action);
                if (predicate(i) == 0)
                    continue;
            }

            vk.pressed = 1;
            if (matchedIndex < 0xb)
                matchedIndex = i;
            if (i == 19) // the dedicated fire slot
                matchedIndex = 0x13;

            if (vk.hasTouch != 0) {
                engine->appManager->OnTouchBegin(vk.baseX + vk.offsetX,
                                                 vk.baseY + vk.offsetY, vk.touch);
                vk.offsetX = vk.baseX;
                vk.offsetY = vk.baseY;
            }
            goto matched;
        }
    }

matched:
    // --- Step 3: latch thrust + apply the matched key's side-effect. --------
    if (module->comboProgress == 9) {
        if (GetKeyState(const_cast<char *>("Accelerate")) != 0) {
            *g_decelHeld = 0;
            *g_accelHeld = 1;
        } else if (GetKeyState(const_cast<char *>("Decelerate")) != 0) {
            *g_accelHeld = 0;
            *g_decelHeld = 1;
        } else if (gAppManager->engine == nullptr) {
            *g_accelHeld = 0;
            *g_decelHeld = 0;
        }
    }

    switch (matchedIndex) {
        case 0: g_steeringX.axis1 = -kAxisStep;
            break; // left
        case 1: g_steeringX.axis1 = kAxisStep;
            break; // right
        case 2: g_steeringY.axis2 = kAxisStep;
            break; // up
        case 3: g_steeringY.axis2 = -kAxisStep;
            break; // down
        case 4: *g_pressWheelRequest = -1;
            break;
        case 5: *g_pressWheelRequest = 1;
            break;
        case 6: *reinterpret_cast<float *>(g_aimAssist) = -0.01f;
            break;
        case 7: *reinterpret_cast<float *>(g_aimAssist) = 0.01f;
            break;
        case 8: engine->field_0x360 = 0x3f800000;
            break; // throttle  1.0f
        case 9: engine->field_0x360 = 0xbf800000;
            break; // throttle -1.0f
        case 0x13: *g_pressFireFlag = 1;
            break;
        default: break;
    }
}

int GetKeyState(int index) {
    if (static_cast<unsigned int>(index) > kVirtualKeyCount - 1)
        return 0;
    return g_virtualKeys[index].pressed;
}

int GetKeyState(char *name) {
    for (int i = 0; i < kVirtualKeyCount; ++i) {
        if (std::strcmp(g_virtualKeys[i].name, name) == 0 &&
            g_virtualKeys[i].pressed != 0)
            return 1;
    }
    return 0;
}

bool ArrowKeyPressed() {
    return g_arrowKeys[0].pressed != 0 || g_arrowKeys[1].pressed != 0 ||
           g_arrowKeys[2].pressed != 0 || g_arrowKeys[3].pressed != 0;
}

bool keyIsPressed() {
    return ArrowKeyPressed() || g_actionKeyState != 0;
}

namespace {
    inline bool currentModuleIs(int id) {
        return reinterpret_cast<std::intptr_t>(
                   gAppManager->GetCurrentApplicationModule()) == id;
    }

    inline bool noBlockingOverlay() {
        return (Globals::is_dialogue_window_visible | Globals::is_choice_window_visible |
                Globals::is_hacking_visible) == 0;
    }
} // namespace

static inline __attribute__ ((always_inline))

int amvHideAndReportChange(bool rewindLatch, bool arrowPressed) {
    Globals::mouseCursorActivated = 0;
    if (rewindLatch) {
        const bool latchArmed = (g_mousePointerLatch + 1 != 0);
        g_mousePointerLatch = (Globals::is_hacking_visible == 0 && latchArmed) ? 1 : 0;
    }
    if (arrowPressed || Globals::showMouseDuringGameOver != 0)
        g_mousePointerLatch = 0;
    return 1;
}

static inline __attribute__ ((always_inline))

int amvReportNoChange() {
    if (g_mousePointerLatch != 0 && currentModuleIs(5) && Globals::keyBindings[4] != 0)
        g_mousePointerLatch = 0;
    return 0;
}

int ActualizeMouseVisibilty(int force) {
    const bool arrowPressed = ArrowKeyPressed();

    if (Globals::mouseCursorActivated != 0) {
        // -- Pointer currently shown: decide whether it stays up. ------------
        // The whole gate is bypassed (-> hide) once the latch has wound past -1
        // and cinematic mode is off.
        if ((g_mousePointerLatch + 1 != 0 || Globals::isCinematicModeActive != 0) &&
            currentModuleIs(2)) {
            bool keep = false;
            bool steerActive = false; // r0 entering the convergence test (0x93e)
            if (IsInGameSubMenuNotActive(2)) {
                if (noBlockingOverlay()) {
                    keep = true;
                    steerActive = Globals::isCinematicModeActive != 0;
                }
            } else if (noBlockingOverlay()) {
                keep = true;
                steerActive = true;
            }

            if (keep) {
                const bool menuHidden = Globals::is_menu_visible == 0;
                if ((static_cast<int>(steerActive) | static_cast<int>(menuHidden)) == 1 &&
                    Globals::isStarMapVisible == 0 && force == 0 &&
                    Globals::showMouseDuringGameOver == 0) {
                    // Gates passed: the pointer stays shown. It is only released
                    // here if an arrow is held while flying; otherwise nothing
                    // changes this frame.
                    if (arrowPressed && currentModuleIs(2))
                        return amvHideAndReportChange(true, arrowPressed);
                    return amvReportNoChange();
                }
            }
        }

        // -- Gate failed: hide the pointer this frame. -----------------------
        return amvHideAndReportChange(force == 0, arrowPressed);
    }

    // -- Pointer currently hidden: decide whether to (re)show it. ------------
    bool reshow = false;
    if (g_mousePointerLatch == 1 || Globals::isCinematicModeActive != 0) {
        if (Globals::isCinematicModeActive != 0) {
            reshow = true;
        } else if (currentModuleIs(2) && IsInGameSubMenuNotActive(2) &&
                   (Globals::is_dialogue_window_visible | Globals::is_choice_window_visible |
                    Globals::is_menu_visible) == 0 &&
                   Globals::is_hacking_visible == 0 && Globals::isStarMapVisible == 0) {
            reshow = true;
        }
    }

    if (reshow) {
        g_mousePointerLatch = -static_cast<int>(
            (g_mousePointerLatch - 1 != 0) && Globals::isCinematicModeActive != 0);
        Globals::mouseCursorActivated = 1;
        return -1;
    }

    return amvReportNoChange();
}

void KeyboardAnimationTimer(AbyssEngine::Engine * /*engine*/) {
}

void actualizeButtonPositions(AbyssEngine::Engine * /*engine*/) {
}

namespace {
    inline float bitsToFloat(int v) {
        float f;
        std::memcpy(&f, &v, sizeof f);
        return f;
    }

    inline int floatToBits(float f) {
        int v;
        std::memcpy(&v, &f, sizeof v);
        return v;
    }

    inline bool keyHeld(int index) { return g_virtualKeys[index].pressed != 0; }

    inline float easeAxis(float t) {
        float eased;
        if (t <= 0.0f)
            eased = -(1.0f - (t + 1.0f) * (t + 1.0f));
        else
            eased = 1.0f - (t - 1.0f) * (t - 1.0f);
        return eased + 0.5f;
    }
} // namespace

void simulateTouch(AbyssEngine::Engine *engine) {
    // -- Post-effect toggle while the loading/post pass owns the screen. -------
    if (*reinterpret_cast<int *>(reinterpret_cast<char *>(gAppManager) + 0xbc) != 0)
        AbyssEngine::Engine::EnablePostEffect = !AbyssEngine::Engine::EnablePostEffect;

    // -- One-shot: seed the four screen edges from the GL display extents. -----
    if (g_simEdgesInit == 0) {
        Globals::resetKeyboard = 10;
        Globals::right_edge = static_cast<int>(engine->GetDisplayWidth()) - 10;
        Globals::left_edge = 10;
        Globals::bottom_edge = static_cast<int>(engine->GetDisplayHeight()) - 10;
        g_simEdgesInit = 1;
    }

    // -- While the pointer is captured, reset the eased stick to neutral. ------
    if (g_pointerCaptured != 0) {
        g_stick.easedX = 0.5f;
        g_stick.easedY = 0.5f;
        g_pointerEmulationEnabled = 0;
        g_pointerCaptured = 0;
        g_zoomRequest = 0;
    }

    // -- Roll output from the two fire keys (engine->field_0x360, a float). -----
    if (keyHeld(8))
        engine->field_0x360 = static_cast<uint32_t>(floatToBits(-1.0f));
    if (keyHeld(9))
        engine->field_0x360 = static_cast<uint32_t>(floatToBits(1.0f));

    // -- Steering value (rotateShipInStation) set from the turn keys. ----------
    // key11 left / key12 right / key13 up / key14 down. A held key drives the
    // axis to +/-20, escalated to +/-40 when the opposite key is also held.
    if (keyHeld(11)) {
        Globals::rotateShipInStation = floatToBits(keyHeld(14) ? -40.0f : -20.0f);
    } else if (keyHeld(12)) {
        Globals::rotateShipInStation = floatToBits(keyHeld(13) ? 40.0f : 20.0f);
    } else if (keyHeld(13)) {
        Globals::rotateShipInStation = floatToBits(20.0f);
    } else if (keyHeld(14)) {
        Globals::rotateShipInStation = floatToBits(-20.0f);
    }

    // -- Integrate the mouse-wheel accumulator into the steering value. --------
    // Blocked while a menu / dialogue / choice overlay is up (then both are
    // zeroed); otherwise the wheel folds into the steering value, the wheel
    // decays by 0.9, and the steering value is clamped to [-40, 40].
    if ((Globals::is_choice_window_visible | Globals::is_menu_visible |
         Globals::is_dialogue_window_visible) == 0) {
        float steer = bitsToFloat(Globals::rotateShipInStation) +
                      VectorSignedToFloat(g_wheelAccumA, 0);
        g_wheelAccumA = static_cast<int>(VectorSignedToFloat(g_wheelAccumA, 0) * 0.9f);
        if (steer > 40.0f)
            steer = 40.0f;
        else if (steer < -40.0f)
            steer = -40.0f;
        Globals::rotateShipInStation = floatToBits(steer);
    } else {
        g_wheelAccumA = 0;
        Globals::rotateShipInStation = floatToBits(0.0f);
    }

    // -- Star-map pan X (translateStarMapInXDirection), a float bit pattern. ----
    // +10 while a "left"/zoom-out key (11 or 15) is held, -10 while only a
    // "right"/zoom-in key (12 or 16) is held, 0 otherwise.
    if (keyHeld(11) || keyHeld(15))
        Globals::translateStarMapInXDirection = floatToBits(10.0f);
    else if (keyHeld(12) || keyHeld(16))
        Globals::translateStarMapInXDirection = floatToBits(-10.0f);
    else
        Globals::translateStarMapInXDirection = floatToBits(0.0f);

    // -- Star-map pan Y (translateStarMapInYDirection). ------------------------
    if (keyHeld(13) || keyHeld(17))
        Globals::translateStarMapInYDirection = floatToBits(-10.0f);
    else if (keyHeld(14) || keyHeld(18))
        Globals::translateStarMapInYDirection = floatToBits(10.0f);
    else
        Globals::translateStarMapInYDirection = floatToBits(0.0f);

    // -- Pointer-driven step into the eased stick outputs. ---------------------
    // Only runs while pointer emulation is active; each pointer accumulator
    // contributes a unit step toward 0/1 and is then decayed by that step
    // (same-sign saturate). With emulation off the eased outputs are left as-is.
    if (g_pointerEmulationEnabled != 0) {
        int px = g_pointerX;
        int stepX = 0;
        if (px < 0) { g_stick.easedX = 0.0f; } else if (px != 0) {
            stepX = 1;
            g_stick.easedX = 1.0f;
        }

        int py = g_pointerY;
        int stepY = 0;
        if (py < 0) { g_stick.easedY = 0.0f; } else if (py != 0) {
            stepY = 1;
            g_stick.easedY = 1.0f;
        }

        // The binary stores the decayed accumulator only when a step was taken
        // or the subtraction went negative (i.e. not the "idle and stayed
        // non-negative" case): store when stepX != 0 || (px - stepX) < 0.
        if (stepX != 0 || (px - stepX) < 0) {
            int dx = px - stepX;
            g_pointerX = dx - (dx >> 31);
        }
        if (stepY != 0 || (py - stepY) < 0) {
            int dy = py - stepY;
            g_pointerY = dy - (dy >> 31);
        }

        if (g_stick.easedX > 1.0f) g_stick.easedX = 1.0f;
        else if (g_stick.easedX < 0.0f) g_stick.easedX = 0.0f;
        if (g_stick.easedY > 1.0f) g_stick.easedY = 1.0f;
        else if (g_stick.easedY < 0.0f) g_stick.easedY = 0.0f;
    }

    // -- Ease stick X toward its rest (0.5). -----------------------------------
    if (g_stick.inputX == 0.5f) {
        // At rest: drift easedX by +/-0.05 toward 0.5, then snap inside (0.4,0.6).
        if (g_stick.easedX != 0.5f)
            g_stick.easedX = static_cast<float>(static_cast<double>(g_stick.easedX) +
                                                (g_stick.easedX < 0.5f ? 0.05 : -0.05));
        if (static_cast<double>(g_stick.easedX) > 0.4 &&
            static_cast<double>(g_stick.easedX) < 0.6)
            g_stick.easedX = 0.5f;
    } else {
        // Driven: integrate the input, clamped to [0,1].
        float v = std::min(g_stick.inputX + g_stick.easedX, 1.0f);
        g_stick.easedX = v;
        if (v < 0.0f) g_stick.easedX = 0.0f;
    }

    // -- Ease stick Y toward its rest (0.5). -----------------------------------
    if (g_stick.inputY == 0.5f) {
        if (g_stick.easedY != 0.5f)
            g_stick.easedY = static_cast<float>(static_cast<double>(g_stick.easedY) +
                                                (g_stick.easedY < 0.5f ? 0.05 : -0.05));
        if (static_cast<double>(g_stick.easedY) > 0.4 &&
            static_cast<double>(g_stick.easedY) < 0.6)
            g_stick.easedY = 0.5f;
    } else {
        float v = std::min(g_stick.inputY + g_stick.easedY, 1.0f);
        g_stick.easedY = v;
        if (v < 0.0f) g_stick.easedY = 0.0f;
    }

    // -- Magnitude shaping while pointer emulation is active. ------------------
    // The larger absolute axis (X capped at 500) sets a common magnitude; the
    // pointer accumulators are normalised by it (Y by half), clamped to [-1,1],
    // run through the ease curve and clamped to [0,1].
    if (g_pointerEmulationEnabled != 0) {
        float magX = std::min(std::fabs(g_stick.easedX), 500.0f);
        float mag = std::max(std::fabs(g_stick.easedY), magX);

        float nx = std::max(std::min(VectorSignedToFloat(g_pointerX, 0) / mag, 1.0f), -1.0f);
        float ny = std::max(std::min(VectorSignedToFloat(g_pointerY, 0) / (mag * 0.5f), 1.0f), -1.0f);

        float ex = easeAxis(nx);
        float ey = easeAxis(ny);
        g_stick.easedX = ex;
        g_stick.easedY = ey;
        if (ex < 0.0f) { g_stick.easedX = 0.0f; }
        if (ey < 0.0f) { g_stick.easedY = 0.0f; }
        if (ex > 1.0f) g_stick.easedX = 1.0f;
        if (ey > 1.0f) g_stick.easedY = 1.0f;
    }

    // -- "Use raw joystick" hold: restore the saved eased outputs. -------------
    if (g_useJoystick != 0) {
        g_stick.easedX = g_stick.savedX;
        g_stick.easedY = g_stick.savedY;
    }

    // -- Map the eased stick to a screen point and post the synthetic touch. ---
    // Only fires inside the flight module (module id 2).
    if (reinterpret_cast<std::intptr_t>(gAppManager->GetCurrentApplicationModule()) == 2) {
        const float dim = VectorSignedToFloat(Globals::smallButton_dim, 0);
        const float cy = VectorSignedToFloat(Globals::touch_stick_y, 0);
        const float cx = VectorSignedToFloat(Globals::touch_stick_x, 0);
        const float ex = g_stick.easedX;
        const float ey = g_stick.easedY;

        const int down = keyIsPressed();
        unsigned char active = g_simTouchActive;
        const bool inactive = (active ^ 1) != 0;

        // Skip entirely when neither pressed nor already driving a touch.
        if (down != 0 || !inactive) {
            const int touchX = static_cast<int>((cx - dim) + (dim + dim) * ex);
            const int touchY = static_cast<int>((cy - dim) + (dim + dim) * ey);
            AbyssEngine::ApplicationManager *mgr = engine->appManager;

            if (down == 0 && !inactive) {
                // Released while a touch is in flight: end it once the stick has
                // recentred, otherwise keep dragging.
                if (ex == 0.5f && ey == 0.5f) {
                    mgr->OnTouchEnd(touchX, touchY, reinterpret_cast<void *>(0xe8b));
                    g_simTouchActive = 0;
                } else {
                    mgr->OnTouchMove(touchX, touchY, reinterpret_cast<void *>(0xe8b));
                }
            } else if (down == 1 && !inactive) {
                // Held with a touch already active: drag.
                mgr->OnTouchMove(touchX, touchY, reinterpret_cast<void *>(0xe8b));
            } else if (active == 0 && down == 1) {
                // First press of a frame: begin then immediately move.
                mgr->OnTouchBegin(touchX, touchY, reinterpret_cast<void *>(0xe8b));
                mgr->OnTouchMove(touchX, touchY, reinterpret_cast<void *>(0xe8b));
                g_simTouchActive = 1;
            }
        }
    }

    // -- Decay the pointer accumulators by 0.96 each frame while uncaptured. ----
    if (g_pointerCaptured == 0) {
        g_pointerX = static_cast<int>(VectorSignedToFloat(g_pointerX, 0) * 0.96f);
        g_pointerY = static_cast<int>(VectorSignedToFloat(g_pointerY, 0) * 0.96f);
    }
}
