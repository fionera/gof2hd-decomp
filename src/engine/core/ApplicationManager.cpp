#include "engine/core/ApplicationManager.h"
ApplicationManager* gAppManager = nullptr;  // canonical ApplicationManager singleton
#include "engine/audio/AESoundRessource.h"
#include "engine/core/IApplicationModule.h"
#include "engine/file/ConfigReader.h"
#include "engine/render/Engine.h"
#include "game/core/CheatHandler.h"
#include "game/core/PaintCanvasClass.h"   // real PaintCanvas:: methods
#include "externs.h"                        // engine g_* globals / host glue

// PaintCanvas::GetWidth()/GetHeight() return void in the recovered class; the
// underlying int-returning implementation is exposed as pc_GetWidth/pc_GetHeight
// (defined alongside PaintCanvas.cpp).
extern PaintCanvas* gCanvas;   // canonical canvas singleton (defined in PaintCanvas.cpp)
extern "C" int pc_GetWidth(PaintCanvas *self);
extern "C" int pc_GetHeight(PaintCanvas *self);

// Engine/host glue that is not a statically resolvable typed method (PLT veneers /
// per-frame engine hook). Kept as documented externs.
extern "C" void Engine_PreUpdate(void *engine);
extern "C" void Engine_Vibrate(void *engine, unsigned short duration);
extern "C" void Engine_VibrateSupported(void *engine);

using AbyssEngine::AESoundRessource;
using AbyssEngine::AESoundInfo;
using AbyssEngine::CheatHandler;
using AbyssEngine::ConfigReader;
using AbyssEngine::ConfigTokenReadFunction;

ApplicationManager::ApplicationManager(void *engine) {
    this->modules = new Array<void *>();
    this->moduleIds = new Array<unsigned int>();
    this->actionTable = new Array<long long>();

    this->frameTimeMs = 0;
    this->previousFrameTimeMs = 0;
    this->pendingModule = 0;
    this->currentTimeMs = 0;
    this->currentKey = 0;
    this->state = 5;
    this->currentModule = 0;
    this->keyState = 0;
    this->engine = (Engine *)engine;

    this->paintCanvas = new PaintCanvas((Engine *)engine);
    gCanvas = this->paintCanvas;     // publish the global canvas singleton
    gEngine = (Engine *)engine;      // publish the global engine singleton

    this->soundResource = new AESoundRessource();
    this->cheatsEnabled = false;

    this->configReader = new ConfigReader((Engine *)engine);
    this->loadingCallback = 0;
    this->resumeCallback = 0;
    this->soundFxEnabled = true;
    this->vibrateEnabled = true;
    this->orientationTrackingEnabled = false;

    // Key-mapping table: a length-prefixed run of 0x40 fixed (mask, String) entries.
    // [-8] = entry size (0x10), [-4] = entry count (0x40), then `count` 16-byte slots
    // each holding a uint32 mask followed by a default-constructed String.
    char *storage = new char[0x408];
    *(uint32_t *)storage = 0x10;
    *(uint32_t *)(storage + 4) = 0x40;
    char *keys = storage + 8;
    for (int offset = 0; offset != 0x400; offset += 0x10) {
        new ((String *)(keys + offset + 4)) String();
        *(uint32_t *)(keys + offset) = 0;
    }
    this->keyMappingTable = keys;

    this->cheatHandler = new CheatHandler((AbyssEngine::KeyCode *)keys);
    this->lastTouchX = -1;
    this->lastTouchY = -1;
}

ApplicationManager::~ApplicationManager() {
    // Shut down the active module (vtable slot +0x0c), then release/clear every loaded
    // module (slot +0x04). Modules are polymorphic IApplicationModule instances whose
    // full vtable is not exposed; dispatch stays through the recovered slot offsets.
    void *module = this->currentModule;
    if (module != 0) {
        void (**vtable)(void *) = *(void (***)(void *))module;
        vtable[0x0c / 4](module);
    }

    for (unsigned int i = 0; i < this->modules->size(); ++i) {
        void *entry = (*this->modules)[i];
        if (entry != 0) {
            void (**vtable)(void *) = *(void (***)(void *))entry;
            vtable[1](entry);
        }
        (*this->modules)[i] = 0;
    }
    this->modules->clear();
    this->moduleIds->clear();

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
        unsigned int count = *(unsigned int *)(keys - 4);
        for (unsigned int i = count; i != 0; --i) {
            ((String *)(keys - 12 + i * 0x10))->~String();
        }
        delete[] (keys - 8);
    }
    this->keyMappingTable = 0;

    delete this->actionTable;
    delete this->moduleIds;
    delete this->modules;
}

// ---- module registration / lookup ----------------------------------------------------

void ApplicationManager::SetApplicationModule(void *module) {
    void *current = this->currentModule;
    this->pendingModule = module;
    this->state = current != 0;
}

void ApplicationManager::SetCurrentApplicationModule(unsigned int id) {
    // Optional performance-test counter (engine global), kept as recovered.
    if (g_perfPending != 0) {
        uint64_t value = g_perfCounter + 1;
        g_perfCounter = value;
        if ((long long)(value - g_perfLimit) >= 0) {
            *(volatile char *)&g_perfExpired = 1;
            *(volatile char *)&g_perfPending = 0;
        }
    }

    unsigned int count = this->moduleIds->size();
    for (unsigned int index = 0; index < count; ++index) {
        if ((*this->moduleIds)[index] == id) {
            void *module = (*this->modules)[index];
            this->state = this->currentModule != 0;
            this->currentModuleId = id;
            this->pendingModule = module;
            return;
        }
    }
}

void *ApplicationManager::GetApplicationModule(unsigned int id) {
    unsigned int count = this->moduleIds->size();
    for (unsigned int index = 0; index < count; ++index) {
        if ((int)(*this->moduleIds)[index] == (int)id) {
            return (*this->modules)[index];
        }
    }
    return 0;
}

void *ApplicationManager::GetEngine() {
    return this->engine;
}

// ---- frame state machine -------------------------------------------------------------

void ApplicationManager::Resume() {
    if (this->state != 3) {
        return;
    }

    void *module = this->currentModule;
    if (module != 0) {
        void (**vtable)(void *) = *(void (***)(void *))module;
        vtable[0x40 / 4](module);
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

    void *module = this->currentModule;
    if (module != 0) {
        void (**vtable)(void *) = *(void (***)(void *))module;
        vtable[0x3c / 4](module);
        if (this->engine != 0) {
            this->engine->Suspend();
        }
        int oldState = this->state;
        this->state = 3;
        this->savedState = oldState;
    }
}

void ApplicationManager::OnUpdate(long long now) {
    Engine_PreUpdate(this->engine);

    if (this->orientationTrackingEnabled) {
        this->CheckForOrientationChange();
    }
    if (this->soundResource != 0 && this->soundFxEnabled) {
        this->soundResource->checkLooping();
    }

    switch (this->state) {
    case 0: {
        void *next = this->pendingModule;
        void *module = next != 0 ? next : this->currentModule;
        if (next != 0) {
            this->pendingModule = 0;
            this->currentModule = next;
        }
        if (module != 0) {
            int (**vtable)(void *) = *(int (***)(void *))module;
            int loading = vtable[2](module);
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
                this->currentTimeMs += (uint64_t)now - previous;
                this->previousFrameTimeMs = previous;
                this->frameTimeMs = now;
            }
        }
        break;
    }
    case 1: {
        void *module = this->currentModule;
        if (module != 0) {
            void (**vtable)(void *) = *(void (***)(void *))module;
            vtable[3](module);
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
        void *module = this->currentModule;
        if (module != 0) {
            void (**vtable)(void *) = *(void (***)(void *))module;
            vtable[0x30 / 4](module);
            Engine *engine = this->engine;
            engine->field_0x68 = 0;
            engine->field_0x58 = 0;
            this->paintCanvas->field_0x4 = 0;
            vtable[0x34 / 4](module);
            ResumeCallback_t *resume = this->resumeCallback;
            if (resume == 0 || !resume(this->paintCanvas, this->resumeCallbackData)) {
                vtable[0x38 / 4](module);
            }
        }
        break;
    }
    default:
        break;
    }
}

// ---- input: keys ---------------------------------------------------------------------

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
    int *mapping = (int *)this->keyMappingTable;
    while (keyIndex <= 0x3f) {
        if (*mapping == key) {
            int highIndex = (int)keyIndex - 0x20;
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

            char *table = (char *)this->actionTable->data();
            unsigned int offset = 0;
            for (unsigned int i = 0; i < this->actionTable->size(); i += 2) {
                char *entry = table + offset;
                if (*(unsigned int *)(entry + 8) == keyIndex && *(int *)(entry + 0x0c) == 0) {
                    actionLow |= *(uint32_t *)entry;
                    actionHigh |= *(uint32_t *)(entry + 4);
                    this->actionMask = actionLow;
                    this->actionMaskHigh = actionHigh;
                    this->actionState |= actionLow;
                    this->actionStateHigh |= actionHigh;
                }
                offset += 0x10;
            }
            break;
        }
        mapping += 4;
        ++keyIndex;
    }

    void *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        void (**vtable)(void *, void *, unsigned int, unsigned int, unsigned int, unsigned int) =
            *(void (***)(void *, void *, unsigned int, unsigned int, unsigned int, unsigned int))module;
        vtable[0x10 / 4](module, module, keyLow, keyHigh, actionLow, actionHigh);
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
    int *mapping = (int *)this->keyMappingTable;
    while (keyIndex <= 0x3f) {
        if (*mapping == key) {
            int highIndex = (int)keyIndex - 0x20;
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

            char *table = (char *)this->actionTable->data();
            unsigned int offset = 0;
            for (unsigned int i = 0; i < this->actionTable->size(); i += 2) {
                char *entry = table + offset;
                if (*(unsigned int *)(entry + 8) == keyIndex && *(int *)(entry + 0x0c) == 0) {
                    actionLow |= *(uint32_t *)entry;
                    actionHigh |= *(uint32_t *)(entry + 4);
                    this->actionMask = actionLow;
                    this->actionMaskHigh = actionHigh;
                    this->actionState &= ~actionLow;
                    this->actionStateHigh &= ~actionHigh;
                }
                offset += 0x10;
            }
            break;
        }
        mapping += 4;
        ++keyIndex;
    }

    void *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        void (**vtable)(void *, void *, unsigned int, unsigned int, unsigned int, unsigned int) =
            *(void (***)(void *, void *, unsigned int, unsigned int, unsigned int, unsigned int))module;
        vtable[0x14 / 4](module, module, keyLow, keyHigh, actionLow, actionHigh);
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

void ApplicationManager::KeyCodeSetMapping(void *array) {
    unsigned int count = *(unsigned int *)array;
    if (count == 0x40) {
        int offset = 0;
        for (unsigned int index = 0; index < count; ++index) {
            uint32_t *mapping = *(uint32_t **)(*(char **)((char *)array + 4) + index * 4);
            char *dst = this->keyMappingTable + offset;
            *(uint32_t *)dst = *mapping;
            ((String *)(dst + 4))->assign((String *)(mapping + 1));
            offset += 0x10;
            count = *(unsigned int *)array;
        }
    }
}

// ---- input: touch --------------------------------------------------------------------

void ApplicationManager::ConvertTouchCoords(int *x, int *y) {
    PaintCanvas *canvas = this->paintCanvas;
    int orientation = canvas->field_0x30;
    int newY;

    if (orientation != 3) {
        if (orientation != 1) {
            if (orientation != 0) {
                return;
            }
            newY = *x;
            *x = pc_GetWidth(canvas) - *y;
            *y = newY;
            return;
        }
        int oldX = *x;
        *x = *y;
        *y = pc_GetHeight(canvas) - oldX;
        canvas = this->paintCanvas;
    }
    *x = pc_GetWidth(canvas) - *x;
    *y = pc_GetHeight(canvas) - *y;
}

void ApplicationManager::OnTouchBegin(int xArg, int yArg, void *touch) {
    int x = xArg;
    int y = yArg;

    void *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        this->ConvertTouchCoords(&x, &y);
        module = this->currentModule;
        void (**vtable)(void *, int, int, void *) = *(void (***)(void *, int, int, void *))module;
        vtable[0x24 / 4](module, x, y, touch);
        module = this->currentModule;
        void (**vtable2)(void *, int, int) = *(void (***)(void *, int, int))module;
        vtable2[0x18 / 4](module, x, y);
        this->lastTouchX = x;
        this->lastTouchY = y;

        Engine *engine = this->engine;
        int mode = g_touchMode;
        if (mode <= 3) {
            PaintCanvas *canvas = this->paintCanvas;
            if (mode == 0 && x <= 0x31 && y <= 0x31) {
                g_touchMode = 1;
            } else if (mode == 1 && x > pc_GetWidth(canvas) - 0x32 &&
                       y > pc_GetHeight(canvas) - 0x32) {
                g_touchMode = 2;
            } else if (mode == 2 && x <= 0x31 && y > pc_GetHeight(canvas) - 0x32) {
                bool *flag = &engine->field_0x74;
                *flag = !*flag;
            } else if (mode == 3 && y <= 0x31 && x > pc_GetWidth(canvas) - 0x32) {
                g_touchMode = 4;
            }
        } else if (engine->field_0x74) {
            if (y < 100) {
                g_touchToggle ^= 1;
            } else {
                int height = pc_GetHeight(this->paintCanvas);
                int width = pc_GetWidth(this->paintCanvas);
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
    int x = xArg;
    int y = yArg;

    void *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        this->ConvertTouchCoords(&x, &y);
        module = this->currentModule;
        void (**vtable)(void *, int, int, void *) = *(void (***)(void *, int, int, void *))module;
        vtable[0x28 / 4](module, x, y, touch);
        module = this->currentModule;
        void (**vtable2)(void *, int, int) = *(void (***)(void *, int, int))module;
        vtable2[0x1c / 4](module, x, y);
        this->lastTouchX = x;
        this->lastTouchY = y;
    }
}

void ApplicationManager::OnTouchEnd(int xArg, int yArg, void *touch) {
    g_touchDown = 0;
    int x = xArg;
    int y = yArg;

    void *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        this->ConvertTouchCoords(&x, &y);
        module = this->currentModule;
        void (**vtable)(void *, int, int, void *) = *(void (***)(void *, int, int, void *))module;
        vtable[0x2c / 4](module, x, y, touch);
        module = this->currentModule;
        void (**vtable2)(void *, int, int) = *(void (***)(void *, int, int))module;
        vtable2[0x20 / 4](module, x, y);
        this->lastTouchX = x;
        this->lastTouchY = y;
    }
}

void ApplicationManager::OnTouchEndSimple() {
    this->currentKey = 0;
    this->currentKeyHigh = 0;
    this->keyState = 0;
    this->keyStateHigh = 0;
    this->actionMask = 0;
    this->actionMaskHigh = 0;
    this->actionState = 0;
    this->actionStateHigh = 0;
}

// ---- timing --------------------------------------------------------------------------

uint64_t ApplicationManager::GetSystemTimeMillis() {
    return this->currentTimeMs;
}

uint64_t ApplicationManager::GetCurrentTimeMillis() {
    return this->currentTimeMs;
}

uint64_t ApplicationManager::GetElapsedTimeMillis() {
    return this->frameTimeMs - this->previousFrameTimeMs;
}

// ---- orientation tracking ------------------------------------------------------------

static bool update_orientation_timer(ApplicationManager *self, int *timer) {
    int elapsed = self->frameTimeMs - self->previousFrameTimeMs;
    int value = *timer + elapsed;
    *timer = value;
    return value >= 0xfb;
}

void ApplicationManager::CheckForOrientationChange() {
    double tilt = *(double *)((char *)this->engine + 0x4b0);
    PaintCanvas *canvas;
    int *timer;
    AbyssEngine::LandscapeMode target;

    if (tilt < -0.5) {
        canvas = this->paintCanvas;
        if (canvas->field_0x30 == 0) {
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
        if (canvas->field_0x30 == 3) {
            timer = &g_orientationRight;
            target = AbyssEngine::LandscapeMode_2;
            if (update_orientation_timer(this, timer)) {
                canvas->SetGameOrientation(target);
                *timer = 0;
            }
            return;
        }
        if (canvas->field_0x30 == 1) {
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
        if (canvas->field_0x30 == 2) {
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

// ---- config --------------------------------------------------------------------------

void ApplicationManager::ConfigRegisterAction(long long value, long long key) {
    this->actionTable->push_back(value);
    this->actionTable->push_back(key);
}

void *ApplicationManager::ConfigGetKeysForAction(long long action) {
    int low = (int)action;
    int high = (int)(action >> 32);
    unsigned int byteOffset = 0;
    Array<String *> *result = 0;

    for (unsigned int index = 0; index < this->actionTable->size(); index += 2) {
        char *actions = (char *)this->actionTable->data();
        int actionLow = *(int *)(actions + byteOffset);
        int actionHigh = *(int *)(actions + byteOffset + 4);
        if (((actionLow ^ low) | (actionHigh ^ high)) == 0) {
            if (result == 0) {
                result = new Array<String *>();
            }
            actions = (char *)this->actionTable->data();
            char *keys = this->keyMappingTable;
            unsigned int keyIndex = *(unsigned int *)(actions + byteOffset + 8);
            String *string = new String(*(String *)(keys + keyIndex * 0x10 + 4));
            result->push_back(string);
        }
        byteOffset += 0x10;
    }
    return result;
}

// ---- application callbacks -----------------------------------------------------------

void ApplicationManager::SetLoadingCallback(LoadingCallback_t *callback, void *data) {
    this->loadingCallback = callback;
    this->loadingCallbackData = data;
}

void ApplicationManager::SetResumeCallback(ResumeCallback_t *callback, void *data) {
    this->resumeCallback = callback;
    this->resumeCallbackData = data;
}

void ApplicationManager::LoadingCallbackShow(int mode, void *data) {
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

// ---- cheats --------------------------------------------------------------------------

void ApplicationManager::CheatUpdate(unsigned short key) {
    if (this->cheatsEnabled && this->cheatHandler != 0) {
        this->cheatHandler->Update(key);
    }
}

void ApplicationManager::CheatAddCode(void *code, int value) {
    if (this->cheatHandler != 0) {
        this->cheatHandler->AddCheatCode(*(String *)code, value);
    }
}

void ApplicationManager::CheatSetCallback(void *callback, void *data) {
    if (this->cheatHandler != 0) {
        this->cheatHandler->SetCheatFunc((AbyssEngine::CheatFunc)callback, data);
    }
}

void ApplicationManager::CheatEnable(bool enable) {
    this->cheatsEnabled = enable;
}

// ---- audio ---------------------------------------------------------------------------

void ApplicationManager::SoundPlay(int soundId) {
    if (this->soundResource != 0 && this->soundFxEnabled) {
        this->soundResource->play(soundId, 1.0f);
    }
}

void ApplicationManager::SoundPlay_vol(int soundId, float volume) {
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

void ApplicationManager::SoundStop() {
    if (this->soundResource != 0) {
        this->soundResource->stopAll();
    }
}

void ApplicationManager::SoundStopSounds() {
    if (this->soundResource != 0) {
        this->soundResource->stopAll();
    }
}

void ApplicationManager::SoundPause() {
    if (this->soundResource != 0) {
        this->soundResource->pauseAll();
    }
}

void ApplicationManager::SoundPauseSounds() {
    if (this->soundResource != 0) {
        this->soundResource->pauseAll();
    }
}

void ApplicationManager::SoundResume(int soundId) {
    if ((this->soundResource != 0 && this->soundFxEnabled) || this->musicEnabled) {
        this->soundResource->resume(soundId);
    }
}

void ApplicationManager::SoundResumeSelf() {
    if (this->soundResource != 0) {
        this->soundResource->resumeAll();
    }
}

void ApplicationManager::SoundResumeSounds() {
    if ((this->soundResource != 0 && this->soundFxEnabled) || this->musicEnabled) {
        this->soundResource->resumeAll();
    }
}

void ApplicationManager::SoundRelease() {
    if (this->soundResource != 0) {
        this->soundResource->freeAllRessources();
    }
}

int ApplicationManager::SoundIsPlaying() {
    if (this->soundResource == 0) {
        return 0;
    }
    return this->soundResource->isPlaying(0);
}

void ApplicationManager::SoundSetVolume(int volume) {
    if (this->soundResource != 0) {
        this->soundResource->setVolume(volume);
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

// ---- vibration -----------------------------------------------------------------------

void ApplicationManager::Vibrate(unsigned short duration) {
    if (this->vibrateEnabled) {
        Engine_Vibrate(this->engine, duration);
    }
}

void ApplicationManager::VibrateEnable(bool enable) {
    this->vibrateEnabled = enable;
}

void ApplicationManager::VibrateSupported() {
    Engine_VibrateSupported(this->engine);
}

// ---- recovered free-function API (operate on the public manager state) ---------------

// Returns the build version string (out is constructed in place by the caller's slot).
void ApplicationManager_GetApplicationVersionString(String *out) {
    new (out) String();
    out->s = u"2.0.16";
}

// Parse a named config file through the manager's ConfigReader.
void ApplicationManager_ConfigReadFile(ApplicationManager *self, String *name) {
    ConfigReader *reader = self->configReader;
    if (reader != 0) {
        reader->ParseFile(*name);
    }
}

// Register a section handler with the manager's ConfigReader.
void ApplicationManager_ConfigRegisterTokenReadFunction(ApplicationManager *self, String *name,
                                                         ConfigTokenReadFunction read, void *context) {
    ConfigReader *reader = self->configReader;
    if (reader != 0) {
        reader->RegisterTokenReadFunction(*name, read, context);
    }
}

// Arm the per-frame performance counter (engine-global bookkeeping).
void ApplicationManager_EnablePerformanceTest(int count) {
    g_perfElapsed = 0;
    g_perfActionCount = (long long)count;
    g_perfLimitValue = 0;
    g_perfTotal = 0;
    g_perfFrame = 0;
    g_perfEnabled = 1;
    g_perfPendingFlag = 0;
}

// Install the sound-info table and initialise each loaded sound slot.
void ApplicationManager_SoundSet(ApplicationManager *self, AESoundInfo *info, int count) {
    if (info != 0 && self->soundResource != 0) {
        AESoundRessource *sound = self->soundResource;
        sound->SetSound(info, count);
        for (int i = 0; i < count; ++i) {
            sound->init(i);
        }
    }
}

// Register an application module under `id`, binding it back to this manager.
void ApplicationManager_RegisterApplicationModule(ApplicationManager *self, unsigned int id, void *module) {
    if (module != 0) {
        ((IApplicationModule *)module)->SetApplicationManager(self);
        self->modules->push_back(module);
        self->moduleIds->push_back(id);
    }
}
