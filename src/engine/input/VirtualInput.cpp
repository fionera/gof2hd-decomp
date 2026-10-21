

#include "engine/input/VirtualInput.h"

#include "engine/core/ApplicationManager.h"
#include "engine/math/AEMath.h"
#include "engine/core/NFC.h"
#include "engine/render/Engine.h"
#include "game/core/Globals.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <initializer_list>

using AbyssEngine::AEMath::VectorSignedToFloat;

struct VirtualKey {
    char name[0x14];
    int pressed;
    int keyCode;
    int altCode1;
    int altCode2;
    int altCode3;
    int *touchX;               // 0x28 screen position the key presses (nullptr: no touch)
    int *touchY;
    int touchOffsetX;
    int touchOffsetY;
    int lastTouchX;            // 0x38 position of the synthetic touch while held
    int lastTouchY;
    int touchWindowHandle;     // 0x40
    int moduleId;              // 0x44 (-1: any module)
    int requiredTouch;         // 0x48 only active while this touch window is focused
    int releaseTouch;          // 0x4c focus handed over on release
    int priority;              // 0x50 2 first ... 0 last
    int (*condition)(int key); // 0x54 extra predicate (IsDialogVisible & co.)
};

constexpr int kVirtualKeyCount = 48;

VirtualKey keys[kVirtualKeyCount];

int stackX;
int stackY;
int wheelStack;

int currentstate;
int stateChanged;
int sound_off;

namespace {
    // Key/mouse state of the virtual input layer.
    struct InputState {
        int mouseCaptured;   // pointer emulation active (CaptureMouse/HideMouse)
        int useJoystick;     // take the stick from the gamepad axes
        int comboProgress;   // cheat key sequence state
        int requiredTouch;   // focused touch window for keys with requiredTouch
    };
    InputState g_input;

    // Mouse configuration and the software stick (0.5 = centered).
    struct MouseState {
        int config;
        float inputX;
        float inputY;
        float easedX;
        float easedY;
        float gamepadX;
        float gamepadY;
    };
    MouseState g_mouse = {1, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f};

    int g_mousePointerLatch = 1;

    constexpr int kDelayedKeyMax = 0x400;
    int g_delayedKeyCount;
    int g_delayedKeys[kDelayedKeyMax];

    bool g_speedUp;
    bool g_slowMotion;

    unsigned char g_simEdgesInit;
    unsigned char g_simTouchActive;
}

extern "C" unsigned int S[4][256]; // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

// Blowfish round function over the S-boxes loaded by InitializeBlowfish.
extern "C" unsigned int F(unsigned int value) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    const unsigned int a = value >> 24;
    const unsigned int b = (value >> 16) & 0xff;
    const unsigned int c = (value >> 8) & 0xff;
    const unsigned int d = value & 0xff;

    return ((S[0][a] + S[1][b]) ^ S[2][c]) + S[3][d];
}

void MouseInput(int dx, int dy) {
    if (g_input.mouseCaptured == 0)
        return;
    if (g_mouse.config != 0) {
        stackX = dx;
    } else {
        stackX += dx;
        dy += stackY;
    }
    stackY = dy;
}

void LowerMouseWheel() {
    if (Globals::mouse_wheel > 0)
        Globals::mouse_wheel = std::max(static_cast<int>(Globals::mouse_wheel * 0.95), 0);
    else if (Globals::mouse_wheel < 0)
        Globals::mouse_wheel = std::min(static_cast<int>(Globals::mouse_wheel * 0.95), 0);

    if (Globals::mouse_wheelX > 0)
        Globals::mouse_wheelX = std::max(static_cast<int>(Globals::mouse_wheelX * 0.95), 0);
    else if (Globals::mouse_wheelX < 0)
        Globals::mouse_wheelX = std::min(static_cast<int>(Globals::mouse_wheelX * 0.95), 0);

    if (Globals::mouse_wheelY > 0)
        Globals::mouse_wheelY = std::max(static_cast<int>(Globals::mouse_wheelY * 0.95), 0);
    else if (Globals::mouse_wheelY < 0)
        Globals::mouse_wheelY = std::min(static_cast<int>(Globals::mouse_wheelY * 0.95), 0);
}

void MouseWheel(float delta, float residual) {
    if (delta < -4.0f && Globals::appManager->GetCurrentApplicationModule() == 2)
        Globals::keyboard_swipe = 1;
    if (delta > 4.0f && Globals::appManager->GetCurrentApplicationModule() == 2)
        Globals::keyboard_swipe = -1;
    wheelStack = static_cast<int>(delta + residual + static_cast<float>(wheelStack));
}

void keyReleasedWithDelay(AbyssEngine::Engine * /*engine*/, int key) {
    if (g_delayedKeyCount > kDelayedKeyMax)
        return;
    g_delayedKeys[g_delayedKeyCount] = key;
    ++g_delayedKeyCount;
}

void SendStoredKeyUpEvents(AbyssEngine::Engine *engine) {
    for (int i = 0; i < g_delayedKeyCount; ++i)
        keyReleased(engine, g_delayedKeys[i]);
    g_delayedKeyCount = 0;
}

void keyEventReleased(AbyssEngine::Engine *engine, char *name) {
    for (VirtualKey &key: keys) {
        if (std::strcmp(key.name, name) == 0)
            keyReleased(engine, key.keyCode);
    }
}

void keyEventPressed(AbyssEngine::Engine *engine, char *name) {
    for (VirtualKey &key: keys) {
        if (std::strcmp(key.name, name) == 0)
            keyPressed(engine, key.keyCode);
    }
}

void SetKeyCode(const char *name, int slot, int code) {
    if (name == nullptr)
        return;

    for (VirtualKey &key: keys) {
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

// Touch windows 0x579.. are the in-game sub menu buttons.
int IsSubMenuActive(int key) {
    const int handle = keys[key].touchWindowHandle;
    if (handle <= 0x578)
        return 0;
    if (handle - 0x579 >= Globals::sub_menu_button_count)
        return 0;
    if (Globals::is_menu_visible != 0)
        return Globals::topMenuIndex != 1 && Globals::topMenuIndex != 2;
    if (Globals::subMenuIndex != -1)
        return 1;
    return Globals::appManager->GetCurrentApplicationModule() == 5;
}

static inline bool keyMatches(const VirtualKey &k, int code) {
    return k.keyCode == code || k.altCode1 == code || k.altCode2 == code || k.altCode3 == code;
}

void keyReleased(AbyssEngine::Engine *engine, int key) {
    Globals::is_choice_window_visible = 0;
    Globals::is_dialogue_window_visible = 0;
    Globals::is_menu_visible = 0;
    Globals::is_hacking_visible = 0;
    if (engine == nullptr)
        return;

    Globals::appManager->GetCurrentApplicationModule();
    int released = -1;
    for (int i = 0; i < kVirtualKeyCount; ++i) {
        VirtualKey &k = keys[i];
        if (!keyMatches(k, key) || k.pressed != 1)
            continue;
        k.pressed = 0;
        if (i < 0xb)
            released = i;
        if (k.touchX == nullptr)
            continue;
        engine->appManager->OnTouchEnd(k.touchOffsetX + k.lastTouchX, k.touchOffsetY + k.lastTouchY,
                                       reinterpret_cast<void *>(k.touchWindowHandle)); // lint: void_ptr (opaque touch handle for OnTouchEnd ABI)
        engine->appManager->OnTouchEnd();
        engine->DrawQuad(k.lastTouchX, k.lastTouchY, 10, 10);
        if (k.releaseTouch != 0)
            g_input.requiredTouch = k.releaseTouch;
    }

    switch (released) {
        case 0:
            g_mouse.inputX = keys[1].pressed ? 0.1f : 0.5f;
            break;
        case 1:
            g_mouse.inputX = keys[0].pressed ? -0.1f : 0.5f;
            break;
        case 2:
            g_mouse.inputY = keys[3].pressed ? -0.1f : 0.5f;
            break;
        case 3:
            g_mouse.inputY = keys[2].pressed ? 0.1f : 0.5f;
            break;
        case 6:
        case 7:
            Globals::bankZ = 0.0f;
            break;
        default:
            break;
    }
}

void keyPressed(AbyssEngine::Engine *engine, int key) {
    if (engine == nullptr)
        return;

    // Cheat sequence: 14, 0x40000000, 1, 0x11, 14, 0xf, 14, 5, 5 unlocks speed control.
    int &combo = g_input.comboProgress;
    if (key == 14) {
        switch (combo) {
            case 4: combo = 5;
                break;
            case 6: combo = 7;
                break;
            case 9: break;
            default: combo = 1;
                break;
        }
    } else if (key == 0x40000000 && combo == 1) {
        combo = 2;
    } else if (key == 1 && combo == 2) {
        combo = 3;
    } else if (key == 0x11 && combo == 3) {
        combo = 4;
    } else if (key == 0xf && combo == 5) {
        combo = 6;
    } else if (key == 5 && combo == 7) {
        combo = 8;
    } else if (combo == 8 && key == 5) {
        combo = 9;
    } else if (combo != 9) {
        combo = 0;
    }

    const unsigned int module = Globals::appManager->GetCurrentApplicationModule();
    int pressed = -1;
    for (int priority = 2; priority >= 0; --priority) {
        for (int i = 0; i < kVirtualKeyCount; ++i) {
            VirtualKey &k = keys[i];
            if (k.priority != priority || !keyMatches(k, key) || k.pressed != 0)
                continue;
            if (static_cast<unsigned int>(k.moduleId) != module && k.moduleId != -1)
                continue;
            if (k.requiredTouch != 0 && g_input.requiredTouch != k.requiredTouch)
                continue;
            if (k.condition != nullptr && k.condition(i) == 0)
                continue;

            k.pressed = 1;
            if (i < 0xb)
                pressed = i;
            if (i == 19)
                pressed = 19;
            if (k.touchX != nullptr) {
                engine->appManager->OnTouchBegin(k.touchOffsetX + *k.touchX, k.touchOffsetY + *k.touchY,
                                                 reinterpret_cast<void *>(k.touchWindowHandle)); // lint: void_ptr (opaque touch handle for OnTouchBegin ABI)
                k.lastTouchX = *k.touchX;
                k.lastTouchY = *k.touchY;
            }
            if (priority != 0)
                goto done;
        }
    }
done:

    if (g_input.comboProgress == 9) {
        if (GetKeyState(const_cast<char *>("Accelerate")) != 0) {
            g_slowMotion = false;
            g_speedUp = true;
        } else if (GetKeyState(const_cast<char *>("Decelerate")) != 0) {
            g_speedUp = false;
            g_slowMotion = true;
        } else if (Globals::keys[42] != 0) {
            g_speedUp = false;
            g_slowMotion = false;
        }
    }

    switch (pressed) {
        case 0: g_mouse.inputX = -0.1f;
            break;
        case 1: g_mouse.inputX = 0.1f;
            break;
        case 2: g_mouse.inputY = 0.1f;
            break;
        case 3: g_mouse.inputY = -0.1f;
            break;
        case 4: Globals::keyboard_swipe = -1;
            break;
        case 5: Globals::keyboard_swipe = 1;
            break;
        case 6: Globals::bankZ = -0.01f;
            break;
        case 7: Globals::bankZ = 0.01f;
            break;
        case 8: engine->autoPilotEngaged = 1.0f;
            break;
        case 9: engine->autoPilotEngaged = -1.0f;
            break;
        case 19: Globals::showWingmanMenu = 1;
            break;
        default: break;
    }
}

int GetKeyState(int index) {
    if (static_cast<unsigned int>(index) > kVirtualKeyCount - 1)
        return 0;
    return keys[index].pressed;
}

int GetKeyState(char *name) {
    for (int i = 0; i < kVirtualKeyCount; ++i) {
        if (std::strcmp(keys[i].name, name) == 0 &&
            keys[i].pressed != 0)
            return 1;
    }
    return 0;
}

bool ArrowKeyPressed() {
    return keys[0].pressed != 0 || keys[1].pressed != 0 ||
           keys[2].pressed != 0 || keys[3].pressed != 0;
}

bool keyIsPressed() {
    return (keys[0].pressed | keys[1].pressed | keys[2].pressed | keys[3].pressed |
            g_input.mouseCaptured) != 0;
}

namespace {
    inline bool noBlockingOverlay() {
        return (Globals::is_dialogue_window_visible | Globals::is_choice_window_visible |
                Globals::is_hacking_visible) == 0;
    }
}

// Returns 1 when the mouse cursor was just hidden, -1 when it was just shown, 0 otherwise.
int ActualizeMouseVisibilty(int force) {
    if (Globals::mouseCursorActivated != 0) {
        bool arrows = ArrowKeyPressed();
        if ((g_mousePointerLatch != -1 || Globals::isCinematicModeActive != 0) &&
            Globals::appManager->GetCurrentApplicationModule() == 2) {
            bool steering;
            bool keep = false;
            if (IsInGameSubMenuNotActive(2)) {
                if ((Globals::is_dialogue_window_visible | Globals::is_choice_window_visible |
                     Globals::is_hacking_visible) == 0) {
                    steering = Globals::isCinematicModeActive != 0;
                    keep = true;
                }
            } else if (Globals::isCinematicModeActive != 0 &&
                       (Globals::is_dialogue_window_visible | Globals::is_choice_window_visible |
                        Globals::is_hacking_visible) == 0) {
                steering = true;
                keep = true;
            }
            if (keep && (steering | (Globals::is_menu_visible == 0)) == 1 && !Globals::isStarMapVisible &&
                force == 0 && Globals::showMouseDuringGameOver == 0) {
                if (!arrows || Globals::appManager->GetCurrentApplicationModule() != 2)
                    goto stillVisible;
            }
        }

        Globals::mouseCursorActivated = 0;
        if (force == 0)
            g_mousePointerLatch = Globals::is_hacking_visible == 0 && g_mousePointerLatch != -1;
        if (arrows || Globals::showMouseDuringGameOver != 0)
            g_mousePointerLatch = 0;
        return 1;
    }
stillVisible:

    if (Globals::mouseCursorActivated == 0 && (g_mousePointerLatch == 1 || Globals::isCinematicModeActive != 0)) {
        if (Globals::isCinematicModeActive != 0 ||
            (Globals::appManager->GetCurrentApplicationModule() == 2 && IsInGameSubMenuNotActive(2) &&
             (Globals::is_dialogue_window_visible | Globals::is_choice_window_visible |
              Globals::is_menu_visible) == 0 &&
             Globals::is_hacking_visible == 0 && (Globals::isStarMapVisible | Globals::isCinematicModeActive) == 0)) {
            g_mousePointerLatch = -(g_mousePointerLatch != 1 && Globals::isCinematicModeActive != 0);
            Globals::mouseCursorActivated = 1;
            return -1;
        }
    }

    if (g_mousePointerLatch != 0 && Globals::appManager->GetCurrentApplicationModule() == 5 &&
        Globals::keyBindings[4] != 0)
        g_mousePointerLatch = 0;
    return 0;
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

    inline bool keyHeld(int index) { return keys[index].pressed != 0; }

    inline float easeAxis(float t) {
        float eased;
        if (t <= 0.0f)
            eased = -(1.0f - (t + 1.0f) * (t + 1.0f));
        else
            eased = 1.0f - (t - 1.0f) * (t - 1.0f);
        return eased + 0.5f;
    }
}

void simulateTouch(AbyssEngine::Engine *engine) {
    if (*reinterpret_cast<int *>(reinterpret_cast<char *>(Globals::appManager) + 0xbc) != 0)
        AbyssEngine::Engine::EnablePostEffect = !AbyssEngine::Engine::EnablePostEffect;

    if (g_simEdgesInit == 0) {
        Globals::resetKeyboard = 10;
        Globals::right_edge = static_cast<int>(engine->GetDisplayWidth()) - 10;
        Globals::left_edge = 10;
        Globals::bottom_edge = static_cast<int>(engine->GetDisplayHeight()) - 10;
        g_simEdgesInit = 1;
    }

    if (g_mouse.config != 0) {
        g_mouse.easedX = 0.5f;
        g_mouse.easedY = 0.5f;
        g_input.mouseCaptured = 0;
        g_mouse.config = 0;
        Globals::keyboard_swipe = 0;
    }

    if (keyHeld(8))
        engine->autoPilotEngaged = -1.0f;
    if (keyHeld(9))
        engine->autoPilotEngaged = 1.0f;

    if (keyHeld(11)) {
        Globals::rotateShipInStation = floatToBits(keyHeld(14) ? -40.0f : -20.0f);
    } else if (keyHeld(12)) {
        Globals::rotateShipInStation = floatToBits(keyHeld(13) ? 40.0f : 20.0f);
    } else if (keyHeld(13)) {
        Globals::rotateShipInStation = floatToBits(20.0f);
    } else if (keyHeld(14)) {
        Globals::rotateShipInStation = floatToBits(-20.0f);
    }

    if ((Globals::is_choice_window_visible | Globals::is_menu_visible |
         Globals::is_dialogue_window_visible) == 0) {
        float steer = bitsToFloat(Globals::rotateShipInStation) +
                      VectorSignedToFloat(wheelStack, 0);
        wheelStack = static_cast<int>(VectorSignedToFloat(wheelStack, 0) * 0.9f);
        if (steer > 40.0f)
            steer = 40.0f;
        else if (steer < -40.0f)
            steer = -40.0f;
        Globals::rotateShipInStation = floatToBits(steer);
    } else {
        wheelStack = 0;
        Globals::rotateShipInStation = floatToBits(0.0f);
    }

    if (keyHeld(11) || keyHeld(15))
        Globals::translateStarMapInXDirection = floatToBits(10.0f);
    else if (keyHeld(12) || keyHeld(16))
        Globals::translateStarMapInXDirection = floatToBits(-10.0f);
    else
        Globals::translateStarMapInXDirection = floatToBits(0.0f);

    if (keyHeld(13) || keyHeld(17))
        Globals::translateStarMapInYDirection = floatToBits(-10.0f);
    else if (keyHeld(14) || keyHeld(18))
        Globals::translateStarMapInYDirection = floatToBits(10.0f);
    else
        Globals::translateStarMapInYDirection = floatToBits(0.0f);

    if (g_input.mouseCaptured != 0) {
        int px = stackX;
        int stepX = 0;
        if (px < 0) { g_mouse.easedX = 0.0f; } else if (px != 0) {
            stepX = 1;
            g_mouse.easedX = 1.0f;
        }

        int py = stackY;
        int stepY = 0;
        if (py < 0) { g_mouse.easedY = 0.0f; } else if (py != 0) {
            stepY = 1;
            g_mouse.easedY = 1.0f;
        }

        if (stepX != 0 || (px - stepX) < 0) {
            int dx = px - stepX;
            stackX = dx - (dx >> 31);
        }
        if (stepY != 0 || (py - stepY) < 0) {
            int dy = py - stepY;
            stackY = dy - (dy >> 31);
        }

        if (g_mouse.easedX > 1.0f) g_mouse.easedX = 1.0f;
        else if (g_mouse.easedX < 0.0f) g_mouse.easedX = 0.0f;
        if (g_mouse.easedY > 1.0f) g_mouse.easedY = 1.0f;
        else if (g_mouse.easedY < 0.0f) g_mouse.easedY = 0.0f;
    }

    if (g_mouse.inputX == 0.5f) {
        if (g_mouse.easedX != 0.5f)
            g_mouse.easedX = static_cast<float>(static_cast<double>(g_mouse.easedX) +
                                                (g_mouse.easedX < 0.5f ? 0.05 : -0.05));
        if (static_cast<double>(g_mouse.easedX) > 0.4 &&
            static_cast<double>(g_mouse.easedX) < 0.6)
            g_mouse.easedX = 0.5f;
    } else {
        float v = std::min(g_mouse.inputX + g_mouse.easedX, 1.0f);
        g_mouse.easedX = v;
        if (v < 0.0f) g_mouse.easedX = 0.0f;
    }

    if (g_mouse.inputY == 0.5f) {
        if (g_mouse.easedY != 0.5f)
            g_mouse.easedY = static_cast<float>(static_cast<double>(g_mouse.easedY) +
                                                (g_mouse.easedY < 0.5f ? 0.05 : -0.05));
        if (static_cast<double>(g_mouse.easedY) > 0.4 &&
            static_cast<double>(g_mouse.easedY) < 0.6)
            g_mouse.easedY = 0.5f;
    } else {
        float v = std::min(g_mouse.inputY + g_mouse.easedY, 1.0f);
        g_mouse.easedY = v;
        if (v < 0.0f) g_mouse.easedY = 0.0f;
    }

    if (g_input.mouseCaptured != 0) {
        float magX = std::min(std::fabs(g_mouse.easedX), 500.0f);
        float mag = std::max(std::fabs(g_mouse.easedY), magX);

        float nx = std::max(std::min(VectorSignedToFloat(stackX, 0) / mag, 1.0f), -1.0f);
        float ny = std::max(std::min(VectorSignedToFloat(stackY, 0) / (mag * 0.5f), 1.0f), -1.0f);

        float ex = easeAxis(nx);
        float ey = easeAxis(ny);
        g_mouse.easedX = ex;
        g_mouse.easedY = ey;
        if (ex < 0.0f) { g_mouse.easedX = 0.0f; }
        if (ey < 0.0f) { g_mouse.easedY = 0.0f; }
        if (ex > 1.0f) g_mouse.easedX = 1.0f;
        if (ey > 1.0f) g_mouse.easedY = 1.0f;
    }

    if (g_input.useJoystick != 0) {
        g_mouse.easedX = g_mouse.gamepadX;
        g_mouse.easedY = g_mouse.gamepadY;
    }

    if (Globals::appManager->GetCurrentApplicationModule() == 2) {
        const float dim = VectorSignedToFloat(Globals::smallButton_dim, 0);
        const float cy = VectorSignedToFloat(Globals::touch_stick_y, 0);
        const float cx = VectorSignedToFloat(Globals::touch_stick_x, 0);
        const float ex = g_mouse.easedX;
        const float ey = g_mouse.easedY;

        const int down = keyIsPressed();
        unsigned char active = g_simTouchActive;
        const bool inactive = (active ^ 1) != 0;

        if (down != 0 || !inactive) {
            const int touchX = static_cast<int>((cx - dim) + (dim + dim) * ex);
            const int touchY = static_cast<int>((cy - dim) + (dim + dim) * ey);
            AbyssEngine::ApplicationManager *mgr = engine->appManager;

            if (down == 0 && !inactive) {
                if (ex == 0.5f && ey == 0.5f) {
                    mgr->OnTouchEnd(touchX, touchY, reinterpret_cast<void *>(0xe8b)); // lint: void_ptr (synthetic touch handle for OnTouchEnd ABI)
                    // lint: void_ptr (synthetic touch handle for OnTouchEnd ABI)
                    g_simTouchActive = 0;
                } else {
                    mgr->OnTouchMove(touchX, touchY, reinterpret_cast<void *>(0xe8b)); // lint: void_ptr (synthetic touch handle for OnTouchMove ABI)
                    // lint: void_ptr (synthetic touch handle for OnTouchMove ABI)
                }
            } else if (down == 1 && !inactive) {
                mgr->OnTouchMove(touchX, touchY, reinterpret_cast<void *>(0xe8b)); // lint: void_ptr (synthetic touch handle for OnTouchMove ABI)
                // lint: void_ptr (synthetic touch handle for OnTouchMove ABI)
            } else if (active == 0 && down == 1) {
                mgr->OnTouchBegin(touchX, touchY, reinterpret_cast<void *>(0xe8b)); // lint: void_ptr (synthetic touch handle for OnTouchBegin ABI)
                // lint: void_ptr (synthetic touch handle for OnTouchBegin ABI)
                mgr->OnTouchMove(touchX, touchY, reinterpret_cast<void *>(0xe8b)); // lint: void_ptr (synthetic touch handle for OnTouchBegin ABI)
                // lint: void_ptr (synthetic touch handle for OnTouchMove ABI)
                g_simTouchActive = 1;
            }
        }
    }

    if (g_mouse.config == 0) {
        stackX = static_cast<int>(VectorSignedToFloat(stackX, 0) * 0.96f);
        stackY = static_cast<int>(VectorSignedToFloat(stackY, 0) * 0.96f);
    }
}
