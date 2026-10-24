

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

// Field order: name, pressed, keyCode, altCode1..3, touchX/Y, touchOffsetX/Y, lastTouchX/Y,
// touchWindowHandle, moduleId, requiredTouch, releaseTouch, priority, condition.
VirtualKey keys[kVirtualKeyCount] = {
    {"Left", 0, 97, 63234, -100, -13, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, IsInGameSubMenuNotActive},
    {"Right", 0, 100, 63235, -200, -14, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, IsInGameSubMenuNotActive},
    {"Down", 0, 119, 63232, -300, -11, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, IsInGameSubMenuNotActive},
    {"Up", 0, 115, 63233, -400, -12, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, IsInGameSubMenuNotActive},
    {"SwipeLeft", 0, 121, 0, -4, 0, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, nullptr},
    {"SwipeRight", 0, 120, 0, -5, 0, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, nullptr},
    {"RotateLeft", 0, 44, 0, 0, 0, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, nullptr},
    {"RotateRight", 0, 46, 0, 0, 0, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, nullptr},
    {"Accelerate", 0, 43, 0, 0, 0, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, nullptr},
    {"Decelerate", 0, 45, 0, 0, 0, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, nullptr},
    {"Break", 0, 1, 0, -2, 0, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 1, IsInGameSubMenuNotActive},
    {"Left", 0, 97, 63234, -100, -13, nullptr, nullptr, 0, 0, 0, 0, 0, 5, 0, -1, 0, nullptr},
    {"Right", 0, 100, 63235, -200, -14, nullptr, nullptr, 0, 0, 0, 0, 0, 5, 0, -1, 0, nullptr},
    {"Down", 0, 119, 63232, -300, -11, nullptr, nullptr, 0, 0, 0, 0, 0, 5, 0, -1, 0, nullptr},
    {"Up", 0, 115, 63233, -400, -12, nullptr, nullptr, 0, 0, 0, 0, 0, 5, 0, -1, 0, nullptr},
    {"Left", 0, 97, 63234, -100, -13, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, nullptr},
    {"Right", 0, 100, 63235, -200, -14, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, nullptr},
    {"Down", 0, 119, 63232, -300, -11, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, nullptr},
    {"Up", 0, 115, 63233, -400, -12, nullptr, nullptr, 0, 0, 0, 0, 0, 2, 0, -1, 0, nullptr},
    {"Wingmen", 0, 0, 0, 0, 0, nullptr, nullptr, 0, 0, 0, 0, 213, 2, 0, -1, 1, nullptr},
    {"Boost", 0, 101, 98, -1, 0, &Globals::boost_x, &Globals::boost_y, 0, 0, 0, 0, 100, 2, 0, -1, 0, nullptr},
    {"Dock", 0, 102, 60, -3, 0, &Globals::fire_x, &Globals::fire_y, 4, 4, 0, 0, 1111, 2, 0, -1, 0, IsDialogNotVisible2},
    {"Fire", 0, 49, 13, -10000, 0, &Globals::fire_x, &Globals::fire_y, 4, 4, 0, 0, 23, 2, 0, -1, 0, IsDialogNotVisible2},
    {"Fire", 0, 49, 13, -10000, 0, &Globals::fire_x, &Globals::fire_y, 4, 4, 0, 0, 26, 1, 0, -1, 0, nullptr},
    {"Fire", 0, 49, 13, -10000, 0, &Globals::right_edge, &Globals::bottom_edge, 4, 4, 0, 0, 24, 5, 0, -1, 0, IsDialogNotVisible2},
    {"Fire", 0, 49, 13, -10000, 0, Globals::other_buttons_x, Globals::other_buttons_y, 4, 4, 0, 0, 25, -1, 0, -1, 1, IsDialogVisible},
    {"SecondaryFire", 0, 114, 118, 0, 0, &Globals::sec_fire_x, &Globals::sec_fire_y, 0, 0, 0, 0, 99, 2, 0, -1, 0, nullptr},
    {"TurretView", 0, 103, 0, 0, 0, &Globals::turret_view_x, &Globals::turret_view_y, 0, 0, 0, 0, 4, 2, 0, -1, 0, nullptr},
    {"ActionMenu", 0, 109, 0, 0, 0, &Globals::action_menu_x, &Globals::action_menu_y, 0, 0, 0, 0, 109, 2, 0, 2, 0, nullptr},
    {"Pause", 0, 53, 0, -8, 0, &Globals::pause_x, &Globals::pause_y, 0, 0, 0, 0, 108, 2, 0, -1, 0, IsStarMapNotVisible},
    {"AutoPilot", 0, 113, 110, -10, 0, &Globals::autopilot_x, &Globals::autopilot_y, 0, 0, 0, 0, 102, 2, 0, 1, 0, nullptr},
    {"FastForward", 0, 9, 0, 0, 0, &Globals::fast_forward_x, &Globals::fast_forward_y, 0, 0, 0, 0, 103, 2, 0, -1, 0, nullptr},
    {"Back", 0, 53, 0, -9, 0, Globals::other_buttons_x, Globals::other_buttons_y, 0, 0, 0, 0, 201, -1, 0, -1, 0, IsDialogNotVisible},
    {"Back", 0, 53, 0, -9, 0, Globals::other_buttons_x, Globals::other_buttons_y, 0, 0, 0, 0, 202, -1, 0, -1, 1, IsDialogVisible},
    {"Back", 0, 53, 0, -9, 0, &Globals::pause_x, &Globals::pause_y, 0, 0, 0, 0, 203, -1, 0, -1, 0, IsInGameSubMenuNotActive},
    {"Back", 0, 53, 0, -9, 0, &Globals::left_edge, &Globals::bottom_edge, 0, 0, 0, 0, 204, -1, 0, -1, 1, IsInPrimaryMenu},
    {"KhadorDrive", 0, 40, 0, 0, 0, nullptr, nullptr, 0, 0, 0, 0, 211, 2, 0, -1, 1, IsInGameSubMenuNotActive},
    {"Cloak", 0, 99, 0, 0, 0, nullptr, nullptr, 0, 0, 0, 0, 212, 2, 0, -1, 1, IsInGameSubMenuNotActive},
    {"1", 0, 18, 0, 0, 0, Globals::sub_menu_buttons_x, Globals::sub_menu_buttons_y, 10, 0, 0, 0, 1401, -1, 0, 0, 0, IsSubMenuActive},
    {"2", 0, 19, 0, 0, 0, Globals::sub_menu_buttons_x, Globals::sub_menu_buttons_y, 10, 0, 0, 0, 1402, -1, 0, 0, 0, IsSubMenuActive},
    {"3", 0, 20, 0, 0, 0, Globals::sub_menu_buttons_x, Globals::sub_menu_buttons_y, 10, 0, 0, 0, 1403, -1, 0, 0, 0, IsSubMenuActive},
    {"4", 0, 21, 0, 0, 0, Globals::sub_menu_buttons_x, Globals::sub_menu_buttons_y, 10, 0, 0, 0, 1404, -1, 0, 0, 0, IsSubMenuActive},
    {"5", 0, 53, 0, 0, 0, Globals::sub_menu_buttons_x, Globals::sub_menu_buttons_y, 10, 0, 0, 0, 1405, -1, 0, 0, 0, IsSubMenuActive},
    {"6", 0, 22, 0, 0, 0, Globals::sub_menu_buttons_x, Globals::sub_menu_buttons_y, 10, 0, 0, 0, 1406, -1, 0, 0, 0, IsSubMenuActive},
    {"7", 0, 26, 0, 0, 0, Globals::sub_menu_buttons_x, Globals::sub_menu_buttons_y, 10, 0, 0, 0, 1407, -1, 0, 0, 0, IsSubMenuActive},
    {"8", 0, 28, 0, 0, 0, Globals::sub_menu_buttons_x, Globals::sub_menu_buttons_y, 10, 0, 0, 0, 1408, -1, 0, 0, 0, IsSubMenuActive},
    {"9", 0, 25, 0, 0, 0, Globals::sub_menu_buttons_x, Globals::sub_menu_buttons_y, 10, 0, 0, 0, 1409, -1, 0, 0, 0, IsSubMenuActive},
    {"0", 0, 29, 0, 0, 0, Globals::sub_menu_buttons_x, Globals::sub_menu_buttons_y, 10, 0, 0, 0, 1410, -1, 0, 0, 0, IsSubMenuActive},
};

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

    // Cheat speed control (0/1 flags; GlobalOpt narrows them to bytes).
    int g_speedUp;
    int g_slowMotion;

    int g_simEdgesInit;
    int g_simTouchActive;
}

// Touch-window predicates (VirtualKey::condition) and pointer/joystick switches.
int IsInGameSubMenuNotActive(int) {
    if (Globals::subMenuIndex != -1 || Globals::is_menu_visible != 0)
        return false;
    return !Globals::isStarMapVisible;
}

int IsDialogNotVisible2(int) {
    return (Globals::is_dialogue_window_visible | Globals::is_choice_window_visible |
            Globals::is_menu_visible) == 0;
}

int IsDialogVisible(int) {
    return (Globals::is_dialogue_window_visible | Globals::is_choice_window_visible) != 0;
}

int IsStarMapNotVisible(int) {
    return !Globals::isStarMapVisible;
}

int IsDialogNotVisible(int) {
    return (Globals::is_dialogue_window_visible | Globals::is_choice_window_visible) == 0;
}

int IsInPrimaryMenu(int) {
    return Globals::is_menu_visible != 0 && Globals::topMenuIndex == 0 &&
           Globals::menu_touch_window_type == 0 && Globals::is_dialogue_window_visible == 0 &&
           Globals::is_choice_window_visible == 0 && !Globals::isStarMapVisible;
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

int IsInGameSubMenuActive(int) {
    return Globals::subMenuIndex != -1;
}

int HideMouse() {
    return g_input.mouseCaptured;
}

void CaptureMouse(int capture) {
    g_input.mouseCaptured = capture;
}

int SwitchToOtherMouseConifguration(int config) {
    g_mouse.config = config;
    return 0;
}

void UseJoystick(int use) {
    g_input.useJoystick = use;
}

int GetUseJoystick() {
    return g_input.useJoystick;
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

void keyEventPressed(AbyssEngine::Engine *engine, char *name) {
    for (VirtualKey &key: keys) {
        if (std::strcmp(key.name, name) == 0)
            keyPressed(engine, key.keyCode);
    }
}

void keyPressed(AbyssEngine::Engine *engine, int key) {
    if (engine == nullptr)
        return;

    // Cheat sequence: 14, 0x40000000, 1, 0x11, 14, 0xf, 14, 5, 5 unlocks speed control.
    const int progress = g_input.comboProgress;
    if (key == 14) {
        switch (progress) {
            default:
                g_input.comboProgress = 1; // restart the sequence
                break;
            case 4:
                g_input.comboProgress = 5;
                break;
            case 6:
                g_input.comboProgress = 7;
                break;
            case 9:
                break;
        }
    } else if (key == 0x40000000 && progress == 1) {
        g_input.comboProgress = 2;
    } else if (key == 1 && progress == 2) {
        g_input.comboProgress = 3;
    } else if (key == 0x11 && progress == 3) {
        g_input.comboProgress = 4;
    } else if (key == 0xf && progress == 5) {
        g_input.comboProgress = 6;
    } else if (key == 5 && progress == 7) {
        g_input.comboProgress = 8;
    } else if (progress == 8 && (key == 5) != 0) {
        g_input.comboProgress = 9;
    } else if (progress != 9) {
        g_input.comboProgress = 0;
    }

    const unsigned int module = Globals::appManager->GetCurrentApplicationModule();
    int pressed = -1;
    for (int priority = 2; priority >= 0; --priority) {
        for (int i = 0; i < kVirtualKeyCount; ++i) {
            VirtualKey &k = keys[i];
            if (priority != k.priority)
                continue;
            if (k.keyCode != key && k.altCode1 != key && k.altCode2 != key && k.altCode3 != key)
                continue;
            if (k.pressed != 0)
                continue;
            if (module != static_cast<unsigned int>(k.moduleId) && k.moduleId != -1)
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
                engine->appManager->OnTouchBegin(*k.touchX + k.touchOffsetX, k.touchOffsetY + *k.touchY,
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
            g_speedUp = 1;
            g_slowMotion = 0;
        } else if (GetKeyState(const_cast<char *>("Decelerate")) != 0) {
            g_slowMotion = 1;
            g_speedUp = 0;
        } else if (Globals::keys[42] != 0) {
            g_speedUp = 0;
            g_slowMotion = 0;
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
        case 8: engine->wheelDelta = 1.0f;
            break;
        case 9: engine->wheelDelta = -1.0f;
            break;
        case 19: Globals::showWingmanMenu = 1;
            break;
        default: break;
    }
}

void keyEventReleased(AbyssEngine::Engine *engine, char *name) {
    for (VirtualKey &key: keys) {
        if (std::strcmp(key.name, name) == 0)
            keyReleased(engine, key.keyCode);
    }
}

void keyReleased(AbyssEngine::Engine *engine, int key) {
    Globals::is_dialogue_window_visible = 0;
    Globals::is_choice_window_visible = 0;
    Globals::is_menu_visible = 0;
    Globals::is_hacking_visible = 0;
    if (engine == nullptr)
        return;

    Globals::appManager->GetCurrentApplicationModule();
    int released = -1;
    for (int i = 0; i < kVirtualKeyCount; ++i) {
        VirtualKey &k = keys[i];
        if (k.keyCode != key && k.altCode1 != key && k.altCode2 != key && k.altCode3 != key)
            continue;
        if (k.pressed != 1)
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

void SendStoredKeyUpEvents(AbyssEngine::Engine *engine) {
    for (int i = 0; i < g_delayedKeyCount; ++i)
        keyReleased(engine, g_delayedKeys[i]);
    g_delayedKeyCount = 0;
}

void keyReleasedWithDelay(AbyssEngine::Engine * /*engine*/, int key) {
    if (g_delayedKeyCount > kDelayedKeyMax)
        return;
    g_delayedKeys[g_delayedKeyCount] = key;
    ++g_delayedKeyCount;
}

bool ArrowKeyPressed() {
    return keys[0].pressed != 0 || keys[1].pressed != 0 ||
           keys[2].pressed != 0 || keys[3].pressed != 0;
}

bool keyIsPressed() {
    return (keys[0].pressed | keys[1].pressed | keys[2].pressed | keys[3].pressed |
            g_input.mouseCaptured) != 0;
}

void actualizeButtonPositions(AbyssEngine::Engine * /*engine*/) {
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

void MouseInput(int dx, int dy) {
    if (g_input.mouseCaptured == 0)
        return;
    if (g_mouse.config == 0) {
        // Relative mode: accumulate the deltas.
        stackX += dx;
        dy += stackY;
    } else {
        stackX = dx;
    }
    stackY = dy;
}

void KeyboardAnimationTimer(AbyssEngine::Engine * /*engine*/) {
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
        const int arrows = ArrowKeyPressed();
        if ((g_mousePointerLatch != -1 || Globals::isCinematicModeActive != 0) &&
            Globals::appManager->GetCurrentApplicationModule() == 2) {
            // In flight the cursor stays while a cinematic steers or no menu is up.
            const int submenuActive = IsInGameSubMenuNotActive(2);
            bool steering;
            if (!submenuActive) {
                if (Globals::isCinematicModeActive == 0 || !noBlockingOverlay())
                    goto hide;
                steering = true;
            } else {
                if (!noBlockingOverlay())
                    goto hide;
                steering = Globals::isCinematicModeActive != 0;
            }
            const int menuHidden = Globals::is_menu_visible == 0;
            if ((steering | menuHidden) != 1)
                goto hide;
            if (Globals::isStarMapVisible)
                goto hide;
            if (force == 0) {
                if (Globals::showMouseDuringGameOver == 0) {
                    if (!arrows || Globals::appManager->GetCurrentApplicationModule() != 2)
                        goto stillVisible;
                }
            }
        }

hide:
        Globals::mouseCursorActivated = 0;
        if (force == 0)
            g_mousePointerLatch = Globals::is_hacking_visible == 0 && g_mousePointerLatch != -1;
        if (arrows)
            g_mousePointerLatch = 0;
        else if (Globals::showMouseDuringGameOver != 0)
            g_mousePointerLatch = 0;
        return 1;
    }
stillVisible:

    if (Globals::mouseCursorActivated == 0 && (g_mousePointerLatch == 1 || Globals::isCinematicModeActive != 0)) {
        if (Globals::isCinematicModeActive != 0)
            goto show;
        if (Globals::appManager->GetCurrentApplicationModule() != 2)
            goto check;
        if (!IsInGameSubMenuNotActive(2))
            goto check;
        if (__builtin_expect((Globals::is_dialogue_window_visible | Globals::is_choice_window_visible | // lint: arm_builtin
                              Globals::is_menu_visible) != 0, 0))
            goto check;
        // The original reads this flag through a volatile access: the load is not
        // folded into the menu-window test above.
        if (__builtin_expect(*(volatile int *)&Globals::is_hacking_visible != 0, 0)) // lint: arm_builtin
            goto check;
        if (__builtin_expect((Globals::isStarMapVisible | Globals::isCinematicModeActive) != 0, 0)) // lint: arm_builtin
            goto check;
show:
        g_mousePointerLatch = -(g_mousePointerLatch != 1 && Globals::isCinematicModeActive != 0);
        Globals::mouseCursorActivated = 1;
        return -1;
    }
check:
    if (g_mousePointerLatch != 0 && Globals::appManager->GetCurrentApplicationModule() == 5 &&
        Globals::keyBindings[4] != 0)
        g_mousePointerLatch = 0;
    return 0;
}

namespace {
    inline bool keyHeld(int index) { return keys[index].pressed != 0; }

    // Quadratic ease of a [-1, 1] stick deflection, re-centred on 0.5.
    inline float easeAxis(float t) {
        float eased;
        if (t > 0.0f)
            eased = 1.0f - (t - 1.0f) * (t - 1.0f);
        else
            eased = -(1.0f - (t + 1.0f) * (t + 1.0f));
        return eased + 0.5f;
    }
}

// Per-frame keyboard/mouse emulation: maps held virtual keys and the captured mouse onto the
// station/star-map controls and a synthetic touch on the in-flight stick (handle 0xe8b).
void simulateTouch(AbyssEngine::Engine *engine) {
    if (Globals::keys[47] != 0)
        AbyssEngine::Engine::EnablePostEffect = !AbyssEngine::Engine::EnablePostEffect;

    if (g_simEdgesInit == 0) {
        Globals::left_edge = 10;
        Globals::right_edge = static_cast<int>(engine->GetDisplayWidth()) - 10;
        Globals::top_edge = 10;
        Globals::bottom_edge = static_cast<int>(engine->GetDisplayHeight()) - 10;
        g_simEdgesInit = 1;
    }

    if (Globals::resetKeyboard != 0) {
        g_mouse.easedX = 0.5f;
        g_mouse.easedY = 0.5f;
        stackX = 0;
        Globals::resetKeyboard = 0;
        stackY = 0;
    }

    if (keyHeld(8))
        engine->wheelDelta = -1.0f;
    if (keyHeld(9))
        engine->wheelDelta = 1.0f;

    if (keyHeld(11)) {
        Globals::rotateShipInStation = -20.0f;
        if (keyHeld(14))
            Globals::rotateShipInStation = -40.0f;
    } else if (keyHeld(12)) {
        Globals::rotateShipInStation = 20.0f;
        if (keyHeld(13))
            Globals::rotateShipInStation = 40.0f;
    } else if (keyHeld(13)) {
        Globals::rotateShipInStation = 20.0f;
    } else if (keyHeld(14)) {
        Globals::rotateShipInStation = -20.0f;
    }

    const bool menuOpen = (Globals::is_dialogue_window_visible || Globals::is_choice_window_visible) |
        Globals::is_menu_visible;
    if (menuOpen) {
        wheelStack = 0;
    } else {
        // The mouse wheel spins the ship in the hangar and decays.
        const float wheel = static_cast<float>(wheelStack);
        Globals::rotateShipInStation += wheel;
        wheelStack = static_cast<int>(wheel * 0.9f);
        if (Globals::rotateShipInStation > 40.0f)
            Globals::rotateShipInStation = 40.0f;
        else if (Globals::rotateShipInStation < -40.0f)
            Globals::rotateShipInStation = -40.0f;
    }
    if (menuOpen) {
        Globals::rotateShipInStation = 0.0f;
    }

    if ((keys[11].pressed | keys[15].pressed) != 0)
        Globals::translateStarMapInXDirection = 10.0f;
    else if ((keys[12].pressed | keys[16].pressed) != 0)
        Globals::translateStarMapInXDirection = -10.0f;
    else
        Globals::translateStarMapInXDirection = 0.0f;

    if ((keys[13].pressed | keys[17].pressed) != 0)
        Globals::translateStarMapInYDirection = -10.0f;
    else if ((keys[14].pressed | keys[18].pressed) != 0)
        Globals::translateStarMapInYDirection = 10.0f;
    else
        Globals::translateStarMapInYDirection = 0.0f;

    const int captured = g_input.mouseCaptured;
    if (captured != 0) {
        // Push the stick to the side the mouse moved and bleed the accumulated motion off by one.
        bool posX = false;
        bool posY = false;
        if (stackX < 0)
            g_mouse.easedX = 0.0f;
        else if (stackX != 0) {
            g_mouse.easedX = 1.0f;
            posX = true;
        }
        if (stackY < 0)
            g_mouse.easedY = 0.0f;
        else if (stackY != 0) {
            g_mouse.easedY = 1.0f;
            posY = true;
        }
        const int nextX = stackX - posX;
        if (posX || nextX < 0)
            stackX = nextX + (nextX < 0);
        const int nextY = stackY - posY;
        if (posY || nextY < 0)
            stackY = nextY + (nextY < 0);

        if (g_mouse.easedX > 1.0f)
            g_mouse.easedX = 1.0f;
        else if (g_mouse.easedX < 0.0f)
            g_mouse.easedX = 0.0f;
        if (g_mouse.easedY > 1.0f)
            g_mouse.easedY = 1.0f;
        else if (g_mouse.easedY < 0.0f)
            g_mouse.easedY = 0.0f;
    }

    // Keyboard stick: snap back towards the centre when released, else follow the key input.
    if (g_mouse.inputX == 0.5f) {
        if (g_mouse.easedX > 0.5f)
            g_mouse.easedX = static_cast<float>(g_mouse.easedX + -0.05);
        else if (g_mouse.easedX < 0.5f)
            g_mouse.easedX = static_cast<float>(g_mouse.easedX + 0.05);
        if (g_mouse.easedX > 0.4 && g_mouse.easedX < 0.6)
            g_mouse.easedX = 0.5f;
    } else {
        g_mouse.easedX += g_mouse.inputX;
        if (g_mouse.easedX > 1.0f)
            g_mouse.easedX = 1.0f;
        if (g_mouse.easedX < 0.0f)
            g_mouse.easedX = 0.0f;
    }

    if (g_mouse.inputY == 0.5f) {
        if (g_mouse.easedY > 0.5f)
            g_mouse.easedY = static_cast<float>(g_mouse.easedY + -0.05);
        else if (g_mouse.easedY < 0.5f)
            g_mouse.easedY = static_cast<float>(g_mouse.easedY + 0.05);
        if (g_mouse.easedY > 0.4 && g_mouse.easedY < 0.6)
            g_mouse.easedY = 0.5f;
    } else {
        g_mouse.easedY += g_mouse.inputY;
        if (g_mouse.easedY > 1.0f)
            g_mouse.easedY = 1.0f;
        if (g_mouse.easedY < 0.0f)
            g_mouse.easedY = 0.0f;
    }

    if (captured != 0) {
        const float magX = std::fabs(g_mouse.easedX);
        int range = static_cast<int>(magX > 500.0f ? magX : 500.0f);
        const float magY = std::fabs(g_mouse.easedY);
        if (magY > static_cast<float>(range))
            range = static_cast<int>(magY);
        const float nx = std::max(std::min(static_cast<float>(stackX) / static_cast<float>(range), 1.0f), -1.0f);
        const float ny = std::max(std::min(static_cast<float>(stackY) / (static_cast<float>(range) * 0.5f), 1.0f), -1.0f);
        float easedX;
        if (nx > 0.0f)
            easedX = 1.0f - (nx - 1.0f) * (nx - 1.0f);
        else
            easedX = -(1.0f - (nx + 1.0f) * (nx + 1.0f));
        float easedY;
        if (ny > 0.0f)
            easedY = 1.0f - (ny - 1.0f) * (ny - 1.0f);
        else
            easedY = -(1.0f - (ny + 1.0f) * (ny + 1.0f));
        g_mouse.easedX = easedX + 0.5f;
        g_mouse.easedY = easedY + 0.5f;
        if (g_mouse.easedX < 0.0f)
            g_mouse.easedX = 0.0f;
        if (g_mouse.easedY < 0.0f)
            g_mouse.easedY = 0.0f;
        if (g_mouse.easedX > 1.0f)
            g_mouse.easedX = 1.0f;
        if (g_mouse.easedY > 1.0f)
            g_mouse.easedY = 1.0f;
    }

    if (g_input.useJoystick != 0) {
        g_mouse.easedX = g_mouse.gamepadX;
        g_mouse.easedY = g_mouse.gamepadY;
    }

    if (Globals::appManager->GetCurrentApplicationModule() == 2) {
        const float dim = static_cast<float>(Globals::smallButton_dim);
        const float cy = static_cast<float>(Globals::touch_stick_y);
        const float cx = static_cast<float>(Globals::touch_stick_x);
        const float ex = g_mouse.easedX;
        const float ey = g_mouse.easedY;

        // The original keyIsPressed returns an int (0/1); the header declares bool.
        // Calling it through an int-returning pointer keeps the original's
        // normalisation of arbitrary non-zero values without changing the symbol.
        int (*keyPressed)() = reinterpret_cast<int (*)()>(keyIsPressed);
        const int down = keyPressed();
        const int active = g_simTouchActive;
        const int inactive = (active ^ 1) != 0;
        const float touchXf = (cx - dim) + (dim + dim) * ex;
        const float touchYf = (cy - dim) + (dim + dim) * ey;

        if (down != 0 || !inactive) {
            if (down == 0 && active != 0) {
                const int touchX = static_cast<int>(touchXf);
                const int touchY = static_cast<int>(touchYf);
                if (ex == 0.5f && ey == 0.5f) {
                engine->appManager->OnTouchEnd(touchX, touchY, reinterpret_cast<void *>(0xe8b)); // lint: void_ptr (synthetic touch handle for OnTouchEnd ABI)
                    g_simTouchActive = 0;
                } else {
                engine->appManager->OnTouchMove(touchX, touchY, reinterpret_cast<void *>(0xe8b)); // lint: void_ptr (synthetic touch handle for OnTouchMove ABI)
                }
            } else if (down == 1 && active != 0) {
                const int touchX = static_cast<int>(touchXf);
                const int touchY = static_cast<int>(touchYf);
                engine->appManager->OnTouchMove(touchX, touchY, reinterpret_cast<void *>(0xe8b)); // lint: void_ptr (synthetic touch handle for OnTouchMove ABI)
            } else if (down == 1 && active == 0) {
                const int touchX = static_cast<int>(touchXf);
                const int touchY = static_cast<int>(touchYf);
                engine->appManager->OnTouchBegin(touchX, touchY, reinterpret_cast<void *>(0xe8b)); // lint: void_ptr (synthetic touch handle for OnTouchBegin ABI)
                engine->appManager->OnTouchMove(touchX, touchY, reinterpret_cast<void *>(0xe8b)); // lint: void_ptr (synthetic touch handle for OnTouchMove ABI)
                g_simTouchActive = 1;
            }
        }
    }

    if (g_mouse.config == 0) {
        stackX = static_cast<int>(static_cast<float>(stackX) * 0.96f);
        stackY = static_cast<int>(static_cast<float>(stackY) * 0.96f);
    }
}

bool SlowMotion() {
    return g_slowMotion != 0;
}

bool SpeedUp() {
    return g_speedUp != 0;
}

void setValuesForGamepad(float x, float y) {
    g_mouse.gamepadX = x;
    g_mouse.gamepadY = y;
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
