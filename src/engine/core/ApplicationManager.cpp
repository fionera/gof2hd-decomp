#include "engine/core/ApplicationManager.h"
#include "game/core/Globals.h"
#include "engine/audio/AESoundRessource.h"
#include "engine/core/IApplicationModule.h"
#include "engine/file/ConfigReader.h"
#include "engine/render/Engine.h"
#include "game/core/CheatHandler.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/KeyCode.h"
#include <cstddef>

namespace {
    struct ModuleVTable {
        void (*slot0)();

        void (*destroy)(IApplicationModule *self);

        int (*showLoading)(IApplicationModule *self);

        void (*release)(IApplicationModule *self);

        void (*onKeyPress)(IApplicationModule *self, IApplicationModule *self2,
                           unsigned int, unsigned int, unsigned int, unsigned int);

        void (*onKeyRelease)(IApplicationModule *self, IApplicationModule *self2,
                             unsigned int, unsigned int, unsigned int, unsigned int);

        void (*onTouchBegin2)(IApplicationModule *self, int x, int y);

        void (*onTouchMove2)(IApplicationModule *self, int x, int y);

        void (*onTouchEnd2)(IApplicationModule *self, int x, int y);

        void (*onTouchBegin3)(IApplicationModule *self, int x, int y, void *data);

        // lint: void_ptr (touch handle is void* in IApplicationModule interface)
        void (*onTouchMove3)(IApplicationModule *self, int x, int y, void *data);

        // lint: void_ptr (touch handle is void* in IApplicationModule interface)
        void (*onTouchEnd3)(IApplicationModule *self, int x, int y, void *data);

        // lint: void_ptr (touch handle is void* in IApplicationModule interface)
        void (*onRender3D)(IApplicationModule *self);

        void (*onRender2D)(IApplicationModule *self);

        void (*onUpdate)(IApplicationModule *self);

        void (*onSuspend)(IApplicationModule *self);

        void (*onResume)(IApplicationModule *self);
    };

    inline ModuleVTable *module_vtable(IApplicationModule *module) {
        return *reinterpret_cast<ModuleVTable **>(module);
    }

    struct KeyMappingHeader {
        unsigned int capacity;
        unsigned int count;
    };

    inline AbyssEngine::KeyCode *key_entries(char *keyMappingTable) {
        return reinterpret_cast<AbyssEngine::KeyCode *>(keyMappingTable);
    }

    inline KeyMappingHeader *key_header(char *keyMappingTable) {
        return reinterpret_cast<KeyMappingHeader *>(keyMappingTable -
                                                    sizeof(KeyMappingHeader));
    }

    struct ActionEntry {
        unsigned int actionLow;
        unsigned int actionHigh;
        unsigned int keyIndex;
        int flags;
    };

    inline ActionEntry *action_entries(Array<long long> *table) {
        return reinterpret_cast<ActionEntry *>(table->data());
    }
}

#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(ModuleVTable, destroy) == 0x04, "vtable destroy slot");
static_assert(offsetof(ModuleVTable, showLoading) == 0x08, "vtable showLoading slot");
static_assert(offsetof(ModuleVTable, release) == 0x0c, "vtable release slot");
static_assert(offsetof(ModuleVTable, onKeyPress) == 0x10, "vtable onKeyPress slot");
static_assert(offsetof(ModuleVTable, onKeyRelease) == 0x14, "vtable onKeyRelease slot");
static_assert(offsetof(ModuleVTable, onTouchBegin2) == 0x18, "vtable onTouchBegin2 slot");
static_assert(offsetof(ModuleVTable, onTouchEnd2) == 0x20, "vtable onTouchEnd2 slot");
static_assert(offsetof(ModuleVTable, onTouchBegin3) == 0x24, "vtable onTouchBegin3 slot");
static_assert(offsetof(ModuleVTable, onRender3D) == 0x30, "vtable onRender3D slot");
static_assert(offsetof(ModuleVTable, onUpdate) == 0x38, "vtable onUpdate slot");
static_assert(offsetof(ModuleVTable, onSuspend) == 0x3c, "vtable onSuspend slot");
static_assert(offsetof(ModuleVTable, onResume) == 0x40, "vtable onResume slot");
static_assert(sizeof(AbyssEngine::KeyCode) == 0x10, "KeyCode entry size");
static_assert(offsetof(AbyssEngine::KeyCode, name) == 0x04, "KeyCode name offset");
static_assert(sizeof(KeyMappingHeader) == 0x08, "key mapping header size");
static_assert(sizeof(ActionEntry) == 0x10, "action entry size");
#endif

namespace {
    int g_perfPending;
    uint64_t g_perfCounter;
    uint64_t g_perfLimit;
    int g_perfExpired;

    int g_touchMode;
    int g_touchToggle;
    int g_touchValue;
    float g_touchFloat;
    int g_touchDown;

    int g_orientationLeft;
    int g_orientationRight;
    int g_orientationFlat;
    int g_orientationUpsideDown;
    int g_orientationInactive;

    int64_t g_perfElapsed;
    int64_t g_perfActionCount;
    int64_t g_perfLimitValue;
    int64_t g_perfTotal;
    int64_t g_perfFrame;
    int g_perfEnabled;
    int g_perfPendingFlag;
}

ApplicationManager::ApplicationManager(Engine *engine) {

    this->frameTimeMs = 0;
    this->previousFrameTimeMs = 0;
    this->pendingModule = 0;
    this->currentTimeMs = 0;
    this->currentKey = 0;
    this->state = 5;
    this->currentModule = 0;
    this->keyState = 0;
    this->engine = engine;

    this->paintCanvas = new PaintCanvas(engine);
    Globals::Canvas = this->paintCanvas;
    gEngine = engine;

    this->soundResource = new AESoundRessource();
    this->cheatsEnabled = false;

    this->configReader = new ConfigReader(engine);
    this->loadingCallback = 0;
    this->resumeCallback = 0;
    this->soundFxEnabled = true;
    this->vibrateEnabled = true;
    this->orientationTrackingEnabled = false;

    char *storage = new char[sizeof(KeyMappingHeader) + 0x40 * sizeof(AbyssEngine::KeyCode)];
    KeyMappingHeader *header = reinterpret_cast<KeyMappingHeader *>(storage);
    header->capacity = 0x10;
    header->count = 0x40;
    AbyssEngine::KeyCode *entries =
            reinterpret_cast<AbyssEngine::KeyCode *>(storage + sizeof(KeyMappingHeader));
    for (int i = 0; i != 0x40; ++i) {
        new(&entries[i].name) String();
        entries[i].code = 0;
    }
    char *keys = reinterpret_cast<char *>(entries);
    this->keyMappingTable = keys;

    this->cheatHandler = new CheatHandler((AbyssEngine::KeyCode *) keys);
    this->lastTouchX = -1;
    this->lastTouchY = -1;
}

ApplicationManager::~ApplicationManager() {
    IApplicationModule *module = this->currentModule;
    if (module != 0) {
        module_vtable(module)->release(module);
    }

    for (unsigned int i = 0; i < this->modules.size(); ++i) {
        IApplicationModule *entry = this->modules[i];
        if (entry != 0) {
            module_vtable(entry)->destroy(entry);
        }
        this->modules[i] = 0;
    }
    ArrayRelease(this->modules);
    ArrayRemoveAll(this->moduleIds);

    delete this->paintCanvas;
    this->paintCanvas = 0;

    delete this->soundResource;
    this->soundResource = 0;

    delete this->cheatHandler;
    this->cheatHandler = 0;

    delete this->configReader;
    this->configReader = 0;

    char *keys = this->keyMappingTable;
    if (keys != 0) {
        AbyssEngine::KeyCode *entries = key_entries(keys);
        unsigned int count = key_header(keys)->count;
        for (unsigned int i = count; i != 0; --i) {
            entries[i - 1].name.~String();
        }
        delete[] reinterpret_cast<char *>(key_header(keys));
    }
    this->keyMappingTable = 0;

}

void ApplicationManager::SetApplicationModule(IApplicationModule *module) {
    IApplicationModule *current = this->currentModule;
    this->pendingModule = module;
    this->state = current != 0;
}

void ApplicationManager::SetCurrentApplicationModule(unsigned int id) {
    if (g_perfPending != 0) {
        uint64_t value = g_perfCounter + 1;
        g_perfCounter = value;
        if ((long long) (value - g_perfLimit) >= 0) {
            *(volatile char *) &g_perfExpired = 1;
            *(volatile char *) &g_perfPending = 0;
        }
    }

    unsigned int count = this->moduleIds.size();
    for (unsigned int index = 0; index < count; ++index) {
        if (this->moduleIds[index] == id) {
            IApplicationModule *module = this->modules[index];
            this->state = this->currentModule != 0;
            this->currentModuleId = id;
            this->pendingModule = module;
            return;
        }
    }
}

void *ApplicationManager::GetApplicationModule(unsigned int id) {
    // lint: void_ptr (exported GetApplicationModule return type, baked ABI)
    unsigned int count = this->moduleIds.size();
    for (unsigned int index = 0; index < count; ++index) {
        if ((int) this->moduleIds[index] == (int) id) {
            return this->modules[index];
        }
    }
    return 0;
}

void *ApplicationManager::GetEngine() {
    // lint: void_ptr (exported GetEngine return type, header-declared ABI)
    return this->engine;
}

void ApplicationManager::Resume(bool arg) {
    (void) arg;
    if (this->state != 3) {
        return;
    }

    IApplicationModule *module = this->currentModule;
    if (module != 0) {
        module_vtable(module)->onResume(module);
        if (this->engine != 0) {
            this->engine->Resume();
        }
        this->actionState = 0;
        this->actionStateHigh = 0;
        this->keyState = 0;
        this->keyStateHigh = 0;
        this->state = 4;
    }
}

void ApplicationManager::Suspend() {
    unsigned int currentState = this->state;
    if (currentState - 3 < 2) {
        return;
    }

    IApplicationModule *module = this->currentModule;
    if (module != 0) {
        module_vtable(module)->onSuspend(module);
        if (this->engine != 0) {
            this->engine->Suspend();
        }
        int oldState = this->state;
        this->state = 3;
        this->savedState = oldState;
    }
}

void ApplicationManager::OnUpdate(long long now) {
    this->engine->PreUpdate();

    if (this->orientationTrackingEnabled) {
        this->CheckForOrientationChange();
    }
    if (this->soundResource != 0 && this->soundFxEnabled) {
        this->soundResource->checkLooping();
    }

    switch (this->state) {
        case 0: {
            IApplicationModule *next = this->pendingModule;
            IApplicationModule *module = next != 0 ? next : this->currentModule;
            if (next != 0) {
                this->pendingModule = 0;
                this->currentModule = next;
            }
            if (module != 0) {
                int loading = module_vtable(module)->showLoading(module);
                LoadingCallback_t *callback = this->loadingCallback;
                if (callback != 0) {
                    callback(this->paintCanvas, loading, this->loadingCallbackData);
                }
                if (loading == 0) {
                    this->actionState = 0;
                    this->state = 5;
                    this->currentTimeMs = 0;
                    this->frameTimeMs = now;
                    this->previousFrameTimeMs = now - 1;
                    this->keyState = 0;
                } else {
                    uint64_t previous = this->frameTimeMs;
                    this->currentTimeMs += (uint64_t) now - previous;
                    this->previousFrameTimeMs = previous;
                    this->frameTimeMs = now;
                }
            }
            break;
        }
        case 1: {
            IApplicationModule *module = this->currentModule;
            if (module != 0) {
                module_vtable(module)->release(module);
                this->engine->ResetLightParam();
                this->state = 0;
                this->currentModule = 0;
            }
            break;
        }
        case 4:
            this->actionState = 0;
            this->state = this->savedState;
            this->currentTimeMs += 1;
            this->frameTimeMs = now;
            this->previousFrameTimeMs = now - 1;
            this->keyState = 0;
            break;
        case 5: {
            IApplicationModule *module = this->currentModule;
            if (module != 0) {
                ModuleVTable *vtable = module_vtable(module);
                vtable->onRender3D(module);
                Engine *engine = this->engine;
                engine->triangleCountA = 0;
                engine->field_0x58 = 0;
                this->paintCanvas->culledCount = 0;
                vtable->onRender2D(module);
                ResumeCallback_t *resume = this->resumeCallback;
                if (resume == 0 || !resume(this->paintCanvas, this->resumeCallbackData)) {
                    vtable->onUpdate(module);
                }
            }
            break;
        }
        default:
            break;
    }
}

void ApplicationManager::OnKeyPress(int key) {
    this->actionMask = 0;
    this->currentKey = key;
    this->currentKeyHigh = key >> 31;
    this->actionMaskHigh = 0;

    unsigned int keyLow = 0;
    unsigned int keyHigh = 0;
    unsigned int actionLow = 0;
    unsigned int actionHigh = 0;
    unsigned int keyIndex = 0;
    AbyssEngine::KeyCode *mapping = key_entries(this->keyMappingTable);
    while (keyIndex <= 0x3f) {
        if (mapping->code == key) {
            int highIndex = (int) keyIndex - 0x20;
            keyLow = 1u << keyIndex;
            if (highIndex >= 0) {
                keyLow = 0;
            }
            keyHigh = 1u >> (0x20 - keyIndex);
            if (highIndex >= 0) {
                keyHigh = 1u << highIndex;
            }
            this->keyState |= keyLow;
            this->keyStateHigh |= keyHigh;

            ActionEntry *entries = action_entries(&this->actionTable);
            for (unsigned int i = 0; i < this->actionTable.size(); i += 2) {
                ActionEntry *entry = &entries[i / 2];
                if (entry->keyIndex == keyIndex && entry->flags == 0) {
                    actionLow |= entry->actionLow;
                    actionHigh |= entry->actionHigh;
                    this->actionMask = actionLow;
                    this->actionMaskHigh = actionHigh;
                    this->actionState |= actionLow;
                    this->actionStateHigh |= actionHigh;
                }
            }
            break;
        }
        ++mapping;
        ++keyIndex;
    }

    IApplicationModule *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        module_vtable(module)->onKeyPress(module, module, keyLow, keyHigh, actionLow, actionHigh);
    }
}

void ApplicationManager::OnKeyRelease(int key) {
    this->actionMask = 0;
    this->actionMaskHigh = 0;

    unsigned int keyLow = 0;
    unsigned int keyHigh = 0;
    unsigned int actionLow = 0;
    unsigned int actionHigh = 0;
    unsigned int keyIndex = 0;
    AbyssEngine::KeyCode *mapping = key_entries(this->keyMappingTable);
    while (keyIndex <= 0x3f) {
        if (mapping->code == key) {
            int highIndex = (int) keyIndex - 0x20;
            keyLow = 1u << keyIndex;
            if (highIndex >= 0) {
                keyLow = 0;
            }
            keyHigh = 1u >> (0x20 - keyIndex);
            if (highIndex >= 0) {
                keyHigh = 1u << highIndex;
            }
            this->keyState &= ~keyLow;
            this->keyStateHigh &= ~keyHigh;

            ActionEntry *entries = action_entries(&this->actionTable);
            for (unsigned int i = 0; i < this->actionTable.size(); i += 2) {
                ActionEntry *entry = &entries[i / 2];
                if (entry->keyIndex == keyIndex && entry->flags == 0) {
                    actionLow |= entry->actionLow;
                    actionHigh |= entry->actionHigh;
                    this->actionMask = actionLow;
                    this->actionMaskHigh = actionHigh;
                    this->actionState &= ~actionLow;
                    this->actionStateHigh &= ~actionHigh;
                }
            }
            break;
        }
        ++mapping;
        ++keyIndex;
    }

    IApplicationModule *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        module_vtable(module)->onKeyRelease(module, module, keyLow, keyHigh, actionLow, actionHigh);
    }
}

void ApplicationManager::ResetKeyState() {
    this->keyState = 0;
}

uint64_t ApplicationManager::GetKeyState() {
    return this->keyState;
}

uint64_t ApplicationManager::GetActionState() {
    return this->actionState;
}

void ApplicationManager::KeyCodeSetMapping(Array<AbyssEngine::KeyCode *> *array) {
    unsigned int count = array->size();
    if (count == 0x40) {
        AbyssEngine::KeyCode *entries = key_entries(this->keyMappingTable);
        for (unsigned int index = 0; index < count; ++index) {
            AbyssEngine::KeyCode *mapping = (*array)[index];
            entries[index].code = mapping->code;
            entries[index].name = mapping->name;
            count = array->size();
        }
    }
}

void ApplicationManager::ConvertTouchCoords(int &x, int &y) {
    PaintCanvas *canvas = this->paintCanvas;
    int orientation = canvas->gameOrientation;
    int newY;

    if (orientation != 3) {
        if (orientation != 1) {
            if (orientation != 0) {
                return;
            }
            newY = x;
            x = canvas->GetWidth() - y;
            y = newY;
            return;
        }
        int oldX = x;
        x = y;
        y = canvas->GetHeight() - oldX;
        canvas = this->paintCanvas;
    }
    x = canvas->GetWidth() - x;
    y = canvas->GetHeight() - y;
}

void ApplicationManager::OnTouchBegin(int xArg, int yArg, void *touch) {
    // lint: void_ptr (exported OnTouchBegin touch handle param, baked ABI)
    int x = xArg;
    int y = yArg;

    IApplicationModule *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        this->ConvertTouchCoords(x, y);
        module = this->currentModule;
        module_vtable(module)->onTouchBegin3(module, x, y, touch);
        module = this->currentModule;
        module_vtable(module)->onTouchBegin2(module, x, y);
        this->lastTouchX = x;
        this->lastTouchY = y;

        Engine *engine = this->engine;
        int mode = g_touchMode;
        if (mode <= 3) {
            PaintCanvas *canvas = this->paintCanvas;
            if (mode == 0 && x <= 0x31 && y <= 0x31) {
                g_touchMode = 1;
            } else if (mode == 1 && x > canvas->GetWidth() - 0x32 &&
                       y > canvas->GetHeight() - 0x32) {
                g_touchMode = 2;
            } else if (mode == 2 && x <= 0x31 && y > canvas->GetHeight() - 0x32) {
                bool *flag = &engine->field_0x74;
                *flag = !*flag;
            } else if (mode == 3 && y <= 0x31 && x > canvas->GetWidth() - 0x32) {
                g_touchMode = 4;
            }
        } else if (engine->field_0x74) {
            if (y < 100) {
                g_touchToggle ^= 1;
            } else {
                int height = this->paintCanvas->GetHeight();
                int width = this->paintCanvas->GetWidth();
                int half = width / 2;
                if (height - 100 < y) {
                    g_touchValue = x < half ? 0 : 1;
                } else {
                    g_touchFloat += x < half ? -1.0f : 1.0f;
                }
            }
        }
    }
}

void ApplicationManager::OnTouchMove(int xArg, int yArg, void *touch) {
    // lint: void_ptr (exported OnTouchMove touch handle param, baked ABI)
    int x = xArg;
    int y = yArg;

    IApplicationModule *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        this->ConvertTouchCoords(x, y);
        module = this->currentModule;
        module_vtable(module)->onTouchMove3(module, x, y, touch);
        module = this->currentModule;
        module_vtable(module)->onTouchMove2(module, x, y);
        this->lastTouchX = x;
        this->lastTouchY = y;
    }
}

void ApplicationManager::OnTouchEnd(int xArg, int yArg, void *touch) {
    // lint: void_ptr (exported OnTouchEnd touch handle param, baked ABI)
    g_touchDown = 0;
    int x = xArg;
    int y = yArg;

    IApplicationModule *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        this->ConvertTouchCoords(x, y);
        module = this->currentModule;
        module_vtable(module)->onTouchEnd3(module, x, y, touch);
        module = this->currentModule;
        module_vtable(module)->onTouchEnd2(module, x, y);
        this->lastTouchX = x;
        this->lastTouchY = y;
    }
}

void ApplicationManager::OnTouchEnd() {
    this->currentKey = 0;
    this->currentKeyHigh = 0;
    this->keyState = 0;
    this->keyStateHigh = 0;
    this->actionMask = 0;
    this->actionMaskHigh = 0;
    this->actionState = 0;
    this->actionStateHigh = 0;
}

uint64_t ApplicationManager::GetSystemTimeMillis() {
    return this->currentTimeMs;
}

uint64_t ApplicationManager::GetCurrentTimeMillis() {
    return this->currentTimeMs;
}

uint64_t ApplicationManager::GetElapsedTimeMillis() {
    return this->frameTimeMs - this->previousFrameTimeMs;
}

static bool update_orientation_timer(ApplicationManager *self, int *timer) {
    int elapsed = self->frameTimeMs - self->previousFrameTimeMs;
    int value = *timer + elapsed;
    *timer = value;
    return value >= 0xfb;
}

void ApplicationManager::CheckForOrientationChange() {
    double tilt = this->engine->accelRaw[0];
    PaintCanvas *canvas;
    int *timer;
    AbyssEngine::LandscapeMode target;

    if (tilt < -0.5) {
        canvas = this->paintCanvas;
        if (canvas->gameOrientation == 0) {
            timer = &g_orientationLeft;
            target = AbyssEngine::LandscapeMode_1;
            if (update_orientation_timer(this, timer)) {
                canvas->SetGameOrientation(target);
                *timer = 0;
            }
            return;
        }
    }

    if (tilt > 0.5) {
        canvas = this->paintCanvas;
        if (canvas->gameOrientation == 3) {
            timer = &g_orientationRight;
            target = AbyssEngine::LandscapeMode_2;
            if (update_orientation_timer(this, timer)) {
                canvas->SetGameOrientation(target);
                *timer = 0;
            }
            return;
        }
        if (canvas->gameOrientation == 1) {
            timer = &g_orientationFlat;
            if (update_orientation_timer(this, timer)) {
                canvas->SetGameOrientation(AbyssEngine::LandscapeMode_dummy);
                *timer = 0;
            }
            return;
        }
    }

    if (tilt < -0.5) {
        canvas = this->paintCanvas;
        if (canvas->gameOrientation == 2) {
            timer = &g_orientationUpsideDown;
            target = AbyssEngine::LandscapeMode_3;
            if (update_orientation_timer(this, timer)) {
                canvas->SetGameOrientation(target);
                *timer = 0;
            }
            return;
        }
    }

    g_orientationInactive = 0;
}

void ApplicationManager::ConfigRegisterAction(long long value, long long key) {
    ArrayAdd(value, this->actionTable);
    ArrayAdd(key, this->actionTable);
}

void *ApplicationManager::ConfigGetKeysForAction(long long action) {
    // lint: void_ptr (exported ConfigGetKeysForAction return type, header-declared ABI)
    int low = (int) action;
    int high = (int) (action >> 32);
    Array<String *> *result = 0;

    for (unsigned int index = 0; index < this->actionTable.size(); index += 2) {
        ActionEntry *entry = &action_entries(&this->actionTable)[index / 2];
        int actionLow = (int) entry->actionLow;
        int actionHigh = (int) entry->actionHigh;
        if (((actionLow ^ low) | (actionHigh ^ high)) == 0) {
            if (result == 0) {
                result = new Array<String *>();
            }
            unsigned int keyIndex = action_entries(&this->actionTable)[index / 2].keyIndex;
            AbyssEngine::KeyCode *entries = key_entries(this->keyMappingTable);
            String *string = new String(entries[keyIndex].name);
            ArrayAdd(string, *result);
        }
    }
    return result;
}

void ApplicationManager::SetLoadingCallback(LoadingCallback_t *callback, void *data) {
    // lint: void_ptr (opaque user data forwarded to LoadingCallback_t, baked ABI)
    this->loadingCallback = callback;
    this->loadingCallbackData = data;
}

void ApplicationManager::SetResumeCallback(ResumeCallback_t *callback, void *data) {
    // lint: void_ptr (opaque user data forwarded to ResumeCallback_t, baked ABI)
    this->resumeCallback = callback;
    this->resumeCallbackData = data;
}

void ApplicationManager::LoadingCallbackShow(int mode, void *data) {
    // lint: void_ptr (opaque user data forwarded to LoadingCallback_t, baked ABI)
    LoadingCallback_t *callback = this->loadingCallback;
    if (callback != 0) {
        callback(this->paintCanvas, mode, data);
    }
}

void ApplicationManager::Quit() {
    QuitCallback_t *callback = this->quitCallback;
    if (callback != 0) {
        callback();
    }
}

void ApplicationManager::CheatUpdate(unsigned short key) {
    if (this->cheatsEnabled && this->cheatHandler != 0) {
        this->cheatHandler->Update(key);
    }
}

void ApplicationManager::CheatAddCode(const String &code, int value) {
    if (this->cheatHandler != 0) {
        this->cheatHandler->AddCheatCode(code, value);
    }
}

void ApplicationManager::CheatSetCallback(void (*callback)(int, void *), void *data) {
    // lint: void_ptr (exported CheatSetCallback signature, opaque cheat callback + data, baked ABI)
    if (this->cheatHandler != 0) {
        this->cheatHandler->SetCheatFunc(callback, data);
    }
}

void ApplicationManager::CheatEnable(bool enable) {
    this->cheatsEnabled = enable;
}

void ApplicationManager::SoundPlay(int soundId) {
    if (this->soundResource != 0 && this->soundFxEnabled) {
        this->soundResource->play(soundId);
    }
}

void ApplicationManager::SoundPlay(int soundId, float volume) {
    if (this->soundResource != 0 && this->soundFxEnabled) {
        this->soundResource->play(soundId, volume);
    }
}

void ApplicationManager::SoundPlayLoop(int soundId) {
    if (this->soundResource != 0 && this->soundFxEnabled) {
        this->soundResource->playLoop(soundId);
    }
}

void ApplicationManager::SoundPlayMusic(int soundId) {
    if (this->soundResource != 0 && this->musicEnabled) {
        this->soundResource->playMusic(soundId);
    }
}

void ApplicationManager::SoundPlayMusicLoop(int soundId) {
    if (this->soundResource != 0 && this->musicEnabled) {
        this->soundResource->playMusicLoop(soundId);
    }
}

void ApplicationManager::SoundStop(int soundId) {
    if (this->soundResource != 0) {
        this->soundResource->stop(soundId);
    }
}

void ApplicationManager::SoundStopSounds() {
    if (this->soundResource != 0) {
        this->soundResource->stop();
    }
}

void ApplicationManager::SoundPause(int soundId) {
    if (this->soundResource != 0) {
        this->soundResource->pause(soundId);
    }
}

void ApplicationManager::SoundPauseSounds() {
    if (this->soundResource != 0) {
        this->soundResource->pause();
    }
}

void ApplicationManager::SoundResume() {
    if ((this->soundResource != 0 && this->soundFxEnabled) || this->musicEnabled) {
        this->soundResource->resume();
    }
}

void ApplicationManager::SoundResume(int soundId) {
    if (this->soundResource != 0 && (this->soundFxEnabled || this->musicEnabled)) {
        this->soundResource->resume(soundId);
    }
}

void ApplicationManager::SoundResumeSounds() {
    if ((this->soundResource != 0 && this->soundFxEnabled) || this->musicEnabled) {
        this->soundResource->resume();
    }
}

void ApplicationManager::SoundRelease(int soundId) {
    if (this->soundResource != 0) {
        this->soundResource->release(soundId);
    }
}

int ApplicationManager::SoundIsPlaying(int soundId) {
    if (this->soundResource == 0) {
        return 0;
    }
    return this->soundResource->isPlaying(soundId);
}

void ApplicationManager::SoundSetVolume(int soundId, int volume) {
    if (this->soundResource != 0) {
        this->soundResource->setVolume(soundId, volume);
    }
}

void ApplicationManager::SoundSetFXVolume(int volume) {
    if (this->soundResource != 0) {
        this->soundResource->setSoundVolume(volume);
    }
}

void ApplicationManager::SoundSetMusicVolume(int volume) {
    if (this->soundResource != 0) {
        this->soundResource->setMusicVolume(volume);
    }
}

void ApplicationManager::SoundEnable(bool enable) {
    this->soundFxEnabled = enable;
    this->musicEnabled = enable;
}

void ApplicationManager::SoundFxEnable(bool enable) {
    this->soundFxEnabled = enable;
}

void ApplicationManager::SoundMusicEnable(bool enable) {
    this->musicEnabled = enable;
}

void ApplicationManager::Vibrate(unsigned short duration) {
    if (this->vibrateEnabled) {
        this->engine->Vibrate(duration);
    }
}

void ApplicationManager::VibrateEnable(bool enable) {
    this->vibrateEnabled = enable;
}

void ApplicationManager::VibrateSupported() {
    this->engine->VibrateSupported();
}

bool ApplicationManager::CheckCrack(const char *path) {
    (void) path;
    return false;
}

void *ApplicationManager::GetApplicationData() {
    // lint: void_ptr (exported GetApplicationData return type, baked ABI)
    return this->applicationData;
}

void ApplicationManager::SetApplicationData(void *data) {
    // lint: void_ptr (exported SetApplicationData param, opaque app data, baked ABI)
    this->applicationData = data;
}

void ApplicationManager::SetExitCallback(QuitCallback_t *callback) {
    this->quitCallback = callback;
}

void *ApplicationManager::GetCurrentApplicationModule() const {
    // lint: void_ptr (exported GetCurrentApplicationModule return type, baked ABI)
    return reinterpret_cast<void *>(this->currentModuleId);
    // lint: void_ptr (cast produces the void* return value mandated by the waived signature)
}

String ApplicationManager::GetApplicationVersionString() {
    return String("2.0.16");
}

void ApplicationManager::ConfigReadFile(String name) {
    ConfigReader *reader = this->configReader;
    if (reader != 0) {
        reader->ParseFile(name);
    }
}

void ApplicationManager::ConfigRegisterTokenReadFunction(String name, ConfigTokenReadFunction read,
                                                         void *context) {
    // lint: void_ptr (exported ConfigRegisterTokenReadFunction opaque context param, baked ABI)
    ConfigReader *reader = this->configReader;
    if (reader != 0) {
        reader->RegisterTokenReadFunction(name, read, context);
    }
}

void ApplicationManager::EnablePerformanceTest(int count) {
    g_perfElapsed = 0;
    g_perfActionCount = (long long) count;
    g_perfLimitValue = 0;
    g_perfTotal = 0;
    g_perfFrame = 0;
    g_perfEnabled = 1;
    g_perfPendingFlag = 0;
}

void ApplicationManager::SoundSet(const AESoundInfo *info, int count) {
    if (info != 0 && this->soundResource != 0) {
        AESoundRessource *sound = this->soundResource;
        sound->SetSound(info, count);
        for (int i = 0; i < count; ++i) {
            sound->init(i);
        }
    }
}

void ApplicationManager::RegisterApplicationModule(unsigned int id, IApplicationModule *module) {
    if (module != 0) {
        module->SetApplicationManager(this);
        ArrayAdd(module, this->modules);
        ArrayAdd(id, this->moduleIds);
    }
}

Engine *GetEngine() {
    return gEngine;
}
