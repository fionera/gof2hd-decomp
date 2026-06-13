#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/engine/audio/AESoundRessource.h"
#include "gof2/externs.h"
#include "gof2/engine/core/IApplicationModule.h"
#include "gof2/game/core/String.h"
#include "gof2/engine/render/Engine.h"
#include "gof2/engine/file/ConfigReader.h"
#include "gof2/game/core/PaintCanvasClass.h"   // real PaintCanvas:: methods

// PaintCanvas::GetWidth()/GetHeight() return void in the recovered class; the
// underlying int-returning implementation is exposed as pc_GetWidth/pc_GetHeight
// (defined alongside PaintCanvas.cpp). The ctor/dtor remain free functions.
extern "C" int pc_GetWidth(PaintCanvas *self);
extern "C" int pc_GetHeight(PaintCanvas *self);
extern AbyssEngine::PaintCanvas *PaintCanvasCtor(AbyssEngine::PaintCanvas *, AbyssEngine::Engine *);
extern AbyssEngine::PaintCanvas *PaintCanvasDtor(AbyssEngine::PaintCanvas *);

extern "C" void ext_001ab578(void *sound, int volume);
extern "C" void ext_001ab528(void *sound);
extern "C" int ext_001ab5a8(void *sound);
extern "C" void ext_001ab4f8(void *sound, int soundId);
extern "C" void ext_001ab488(void *sound, int soundId, float volume);
extern "C" void ext_001ab5e8(void *cheats, unsigned short key);
extern "C" void ext_001ab5d8(void *cheats);
extern "C" void ext_001ab548(void *sound);
extern "C" void ext_001ab518(void *sound);
extern "C" void ext_001ab508(void *sound, int soundId);
extern "C" void ext_001ab4e8(void *sound, int soundId);
extern "C" void ext_001ab4d8(void *sound, int soundId);
extern "C" void ext_001ab598(void *sound);
extern "C" void ext_001ab558(void *sound, int soundId);
extern "C" void ext_001ab4c8(void *sound);
extern "C" void ext_001ab568(void *sound);
extern "C" void ext_001ab328(void *engine, unsigned short duration);
extern "C" void ext_001ab588(void *sound);
extern "C" void ext_001ab5c8(void *cheats);
extern "C" void ext_001ab538(void *sound);
extern "C" void Engine_PreUpdate(void *engine);
extern "C" void ext_001ab610(void);
extern "C" void AESoundRessource_ctor(void *sound);
extern "C" void ConfigReader_ctor(void *reader, void *engine);
extern "C" void CheatHandler_ctor(void *cheats, void *keys);
extern "C" void AESoundRessource_SetSound(void *sound, void *info, int count);
extern "C" void ext_001ab318(void *engine);
extern "C" void AESoundRessource_dtor(void *sound);
extern "C" void CheatHandler_dtor(void *cheats);
extern "C" void ConfigReader_dtor(void *reader);

typedef void LoadingCallback_t(PaintCanvas *, int, void *);

void ApplicationManager::SetLoadingCallback(LoadingCallback_t *callback, void *data) {
    this->loadingCallback = (void *)callback;
    this->loadingCallbackData = data;
}

void ApplicationManager::SoundSetVolume(int volume) {
    void *sound = this->soundResource;
    if (sound != 0) {
        ext_001ab578(sound, volume);
    }
}

void ApplicationManager::SoundStopSounds() {
    void *sound = this->soundResource;
    if (sound != 0) {
        ext_001ab528(sound);
    }
}

int ApplicationManager::SoundIsPlaying() {
    void *sound = this->soundResource;
    if (sound == 0) {
        return 0;
    }
    return ext_001ab5a8(sound);
}

uint64_t ApplicationManager::GetSystemTimeMillis() {
    return this->currentTimeMs;
}

typedef void LoadingShowCallback(PaintCanvas *, int, void *);

void ApplicationManager::LoadingCallbackShow(int mode, void *data) {
    LoadingShowCallback *callback = (LoadingShowCallback *)this->loadingCallback;
    if (callback != 0) {
        callback(*(PaintCanvas **)this, mode, data);
    }
}

void ApplicationManager::SoundPlay(int soundId) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0 && self->soundFxEnabled) {
        ext_001ab4f8(sound, soundId);
    }
}

void ApplicationManager::SoundPlay_vol(int soundId, float volume) {
    void *sound = this->soundResource;
    if (sound == 0) {
        return;
    }
    if (this->soundFxEnabled == 0) {
        return;
    }
    ext_001ab488(sound, soundId, volume);
}

void ApplicationManager::CheatUpdate(unsigned short key) {
    if (this->cheatsEnabled) {
        void *cheats = this->cheatHandler;
        if (cheats != 0) {
            ext_001ab5e8(cheats, key);
        }
    }
}

void ApplicationManager::CheatAddCode(void *code, int value) {
    void *cheats = this->cheatHandler;
    if (cheats != 0) {
        ext_001ab5d8(cheats);
    }
}

void ApplicationManager::SoundPause() {
    void *sound = this->soundResource;
    if (sound != 0) {
        ext_001ab548(sound);
    }
}

uint64_t ApplicationManager::GetActionState() {
    return this->actionState;
}

typedef void ModuleCallback(void *);

void ApplicationManager::Resume() {
    if (this->field_0x3c != 3) {
        return;
    }

    void *module = this->currentModule;
    if (module != 0) {
        ModuleCallback **vtable = *(ModuleCallback ***)module;
        vtable[0x40 / 4](module);
        void *engine = this->engine;
        if (engine != 0) {
            ((Engine *)(engine))->Resume();
        }
        this->actionState = 0;
        this->actionStateHigh = 0;
        this->keyState = 0;
        this->keyStateHigh = 0;
        this->field_0x3c = 4;
    }
}

void ApplicationManager::SoundStop() {
    void *sound = this->soundResource;
    if (sound != 0) {
        ext_001ab518(sound);
    }
}

void ApplicationManager::SoundPlayLoop(int soundId) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0 && self->soundFxEnabled) {
        ext_001ab508(sound, soundId);
    }
}

void ApplicationManager::SoundPlayMusicLoop(int soundId) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0 && self->musicEnabled) {
        ext_001ab4e8(sound, soundId);
    }
}

void ApplicationManager::ResetKeyState() {
    this->keyState = 0;
}

uint64_t ApplicationManager::GetElapsedTimeMillis() {
    return this->frameTimeMs - this->previousFrameTimeMs;
}

void ApplicationManager::SoundMusicEnable(bool enable) {
    this->musicEnabled = enable;
}

void ApplicationManager::CheatEnable(bool enable) {
    this->cheatsEnabled = enable;
}

uint64_t ApplicationManager::GetCurrentTimeMillis() {
    return this->currentTimeMs;
}

void ApplicationManager::SoundPlayMusic(int soundId) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0 && self->musicEnabled) {
        ext_001ab4d8(sound, soundId);
    }
}

void ApplicationManager::OnTouchEndSimple() {
    this->currentKey = 0;
    this->currentKeyHigh = 0;
    this->keyState = 0;
    this->keyStateHigh = 0;
    // Original wrote a 16-byte vector zero across +0x98..+0xa7 (action mask + action state).
    this->actionMask = 0;
    this->actionMaskHigh = 0;
    this->actionState = 0;
    this->actionStateHigh = 0;
}

void ApplicationManager::SoundEnable(bool enable) {
    this->soundFxEnabled = enable;
    this->musicEnabled = enable;
}

void ApplicationManager::SoundSetMusicVolume(int volume) {
    void *sound = this->soundResource;
    if (sound != 0) {
        ext_001ab598(sound);
    }
}

typedef bool ResumeCallback_t(PaintCanvas *, void *);

void ApplicationManager::SetResumeCallback(ResumeCallback_t *callback, void *data) {
    this->resumeCallback = (void *)callback;
    this->resumeCallbackData = data;
}

extern "C" __attribute__((disable_tail_calls)) void ApplicationManager_GetApplicationVersionString(String *out)
{
    new (out) String();
    out->s = u"2.0.16";
}

void ApplicationManager::VibrateEnable(bool enable) {
    this->vibrateEnabled = enable;
}

void ApplicationManager::SoundResume(int soundId) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if ((sound != 0 && self->soundFxEnabled) || self->musicEnabled) {
        ext_001ab558(sound, soundId);
    }
}

void ApplicationManager::SoundRelease() {
    void *sound = this->soundResource;
    if (sound != 0) {
        ext_001ab4c8(sound);
    }
}

typedef void QuitCallback_t();

void ApplicationManager::Quit() {
    QuitCallback_t *callback = (QuitCallback_t *)this->quitCallback;
    if (callback != 0) {
        callback();
    }
}

void * ApplicationManager::GetEngine() {
    return this->engine;
}

typedef void ModuleCallback(void *);

void ApplicationManager::Suspend() {
    unsigned int state = this->field_0x3c;
    if (state - 3 < 2) {
        return;
    }

    void *module = this->currentModule;
    if (module != 0) {
        ModuleCallback **vtable = *(ModuleCallback ***)module;
        vtable[0x3c / 4](module);
        void *engine = this->engine;
        if (engine != 0) {
            ((Engine *)(engine))->Suspend();
        }
        int oldState = this->field_0x3c;
        this->field_0x3c = 3;
        this->savedState = oldState;
    }
}

void ApplicationManager::SoundFxEnable(bool enable) {
    this->soundFxEnabled = enable;
}

void ApplicationManager::SoundResumeSelf() {
    void *sound = this->soundResource;
    if (sound != 0) {
        ext_001ab568(sound);
    }
}

void ApplicationManager::Vibrate(unsigned short duration) {
    if (this->vibrateEnabled) {
        ext_001ab328(this->engine, duration);
    }
}

void ApplicationManager::SoundSetFXVolume(int volume) {
    void *sound = this->soundResource;
    if (sound != 0) {
        ext_001ab588(sound);
    }
}

void ApplicationManager::CheatSetCallback(void *callback, void *data) {
    void *cheats = this->cheatHandler;
    if (cheats != 0) {
        ext_001ab5c8(cheats);
    }
}

void ApplicationManager::SoundResumeSounds() {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if ((sound != 0 && self->soundFxEnabled) || self->musicEnabled) {
        ext_001ab568(sound);
    }
}

void ApplicationManager::SoundPauseSounds() {
    void *sound = this->soundResource;
    if (sound != 0) {
        ext_001ab538(sound);
    }
}

uint64_t ApplicationManager::GetKeyState() {
    return this->keyState;
}

void ApplicationManager::SetApplicationModule(void *module) {
    void *current = this->currentModule;
    this->pendingModule = module;
    this->field_0x3c = current != 0;
}

extern "C" __attribute__((disable_tail_calls)) void ApplicationManager_ConfigReadFile(ApplicationManager *self, String *name)
{
    unsigned char storage[sizeof(String)] __attribute__((aligned(4)));
    ConfigReader *reader = (ConfigReader *)self->configReader;
    if (reader != 0) {
        String *copy = (String *)storage;
        new (copy) String(*name);
        ((::AbyssEngine::ConfigReader *)reader)->ParseFile(*copy);
        ((String *)storage)->~String();
    }
    return;
}

typedef void TouchEndCallback(void *, int, int, void *);
typedef void TouchReleaseCallback(void *, int, int);

void ApplicationManager::OnTouchEnd(int xArg, int yArg, void *touch) {
    ApplicationManager *self = this;
    g_touchDown = 0;
    int x = xArg;
    int y = yArg;

    void *module = self->currentModule;
    if (module != 0 && self->field_0x3c == 5) {
        ((ApplicationManager *)(self))->ConvertTouchCoords(&x, &y);
        module = self->currentModule;
        void **vtable = *(void ***)module;
        ((TouchEndCallback *)vtable[0x2c / 4])(module, x, y, touch);
        module = self->currentModule;
        vtable = *(void ***)module;
        ((TouchReleaseCallback *)vtable[0x20 / 4])(module, x, y);
        self->lastTouchX = x;
        self->lastTouchY = y;
    }

    return;
}

void ApplicationManager_EnablePerformanceTest(int count)
{
    g_perfElapsed = 0;
    g_perfActionCount = (long long)count;
    g_perfLimitValue = 0;
    g_perfTotal = 0;
    g_perfFrame = 0;
    g_perfEnabled = 1;
    g_perfPendingFlag = 0;
}

typedef int ModuleIntCallback(void *);
typedef void ModuleVoidCallback(void *);
typedef bool LoadingCallbackU(PaintCanvas *, int, void *);
typedef bool ResumeCallbackU(PaintCanvas *, void *);

void ApplicationManager::OnUpdate(long long now) {
    ApplicationManager *self = this;
    void *engine = self->engine;
    Engine_PreUpdate(engine);

    if (self->orientationTrackingEnabled) {
        ((ApplicationManager *)(self))->CheckForOrientationChange();
    }
    void *sound = self->soundResource;
    if (sound != 0 && self->soundFxEnabled) {
        ((AbyssEngine::AESoundRessource *)(sound))->checkLooping();
    }

    switch (self->field_0x3c) {
    case 0: {
        void *next = self->pendingModule;
        void *module = next != 0 ? next : self->currentModule;
        if (next != 0) {
            self->pendingModule = 0;
            self->currentModule = next;
        }
        if (module != 0) {
            void **vtable = *(void ***)module;
            int loading = ((ModuleIntCallback *)vtable[2])(module);
            LoadingCallbackU *callback = (LoadingCallbackU *)self->loadingCallback;
            if (callback != 0) {
                callback(*(PaintCanvas **)self, loading, self->loadingCallbackData);
            }
            if (loading == 0) {
                self->actionState = 0;
                self->field_0x3c = 5;
                self->currentTimeMs = 0;
                self->frameTimeMs = now;
                self->previousFrameTimeMs = now - 1;
                self->keyState = 0;
            } else {
                uint64_t previous = self->frameTimeMs;
                self->currentTimeMs += (uint64_t)now - previous;
                self->previousFrameTimeMs = previous;
                self->frameTimeMs = now;
            }
        }
        break;
    }
    case 1: {
        void *module = self->currentModule;
        if (module != 0) {
            void **vtable = *(void ***)module;
            ((ModuleVoidCallback *)vtable[3])(module);
            ((Engine *)(engine))->ResetLightParam();
            self->field_0x3c = 0;
            self->currentModule = 0;
        }
        break;
    }
    case 4:
        self->actionState = 0;
        self->field_0x3c = self->savedState;
        self->currentTimeMs += 1;
        self->frameTimeMs = now;
        self->previousFrameTimeMs = now - 1;
        self->keyState = 0;
        break;
    case 5: {
        void *module = self->currentModule;
        if (module != 0) {
            void **vtable = *(void ***)module;
            ((ModuleVoidCallback *)vtable[0x30 / 4])(module);
            *(uint64_t *)((char *)engine + 0x68) = 0;
            *(uint64_t *)((char *)engine + 0x58) = 0;
            *(int *)((char *)*(void **)self + 4) = 0;
            ((ModuleVoidCallback *)vtable[0x34 / 4])(module);
            ResumeCallbackU *resume = (ResumeCallbackU *)self->resumeCallback;
            if (resume == 0 || !resume(*(PaintCanvas **)self, self->resumeCallbackData)) {
                ((ModuleVoidCallback *)vtable[0x38 / 4])(module);
            }
        }
        break;
    }
    default:
        break;
    }

    return;
    ext_001ab610();
}

ApplicationManager::ApplicationManager(void *engine) {
    ApplicationManager *self = this;

    self->modules = new Array<void *>();
    self->moduleIds = new Array<unsigned int>();
    self->actionTable = new Array<long long>();

    self->frameTimeMs = 0;
    self->previousFrameTimeMs = 0;
    self->pendingModule = 0;
    self->currentTimeMs = 0;
    self->currentKey = 0;
    self->field_0x3c = 5;
    self->currentModule = 0;
    self->keyState = 0;
    self->engine = engine;

    void *canvas = ::operator new(0x20c);
    PaintCanvasCtor((AbyssEngine::PaintCanvas *)canvas, (AbyssEngine::Engine *)engine);
    *(void **)self = canvas;

    void *sound = ::operator new(0x14);
    AESoundRessource_ctor(sound);
    self->cheatsEnabled = false;
    self->soundResource = sound;

    void *reader = ::operator new(0x14);
    ConfigReader_ctor(reader, engine);
    self->loadingCallback = 0;
    self->configReader = reader;
    self->resumeCallback = 0;
    self->soundFxEnabled = 0x101;
    self->vibrateEnabled = true;
    self->orientationTrackingEnabled = false;

    char *storage = (char *)::operator new(0x408);
    *(uint32_t *)storage = 0x10;
    *(uint32_t *)(storage + 4) = 0x40;
    char *keys = storage + 8;
    for (int offset = 0; offset != 0x400; offset += 0x10) {
        new ((String *)(keys + offset + 4)) String();
        *(uint32_t *)(keys + offset) = 0;
    }
    self->keyMappingTable = keys;

    void *cheats = ::operator new(0x10);
    CheatHandler_ctor(cheats, keys);
    self->cheatHandler = cheats;
    self->lastTouchX = -1;
    self->lastTouchY = -1;
}

typedef void TouchMoveCallback(void *, int, int, void *);
typedef void TouchDragCallback(void *, int, int);

void ApplicationManager::OnTouchMove(int xArg, int yArg, void *touch) {
    ApplicationManager *self = this;
    int x = xArg;
    int y = yArg;

    void *module = self->currentModule;
    if (module != 0 && self->field_0x3c == 5) {
        ((ApplicationManager *)(self))->ConvertTouchCoords(&x, &y);
        module = self->currentModule;
        void **vtable = *(void ***)module;
        ((TouchMoveCallback *)vtable[0x28 / 4])(module, x, y, touch);
        module = self->currentModule;
        vtable = *(void ***)module;
        ((TouchDragCallback *)vtable[0x1c / 4])(module, x, y);
        self->lastTouchX = x;
        self->lastTouchY = y;
    }

    return;
}

typedef void KeyCallback(void *, void *, unsigned int, unsigned int, unsigned int, unsigned int);

void ApplicationManager::OnKeyPress(int key) {
    ApplicationManager *self = this;
    self->actionMask = 0;
    self->currentKey = key;
    self->currentKeyHigh = key >> 31;
    self->actionMaskHigh = 0;

    unsigned int keyLow = 0;
    unsigned int keyHigh = 0;
    unsigned int actionLow = 0;
    unsigned int actionHigh = 0;
    unsigned int keyIndex = 0;
    int *mapping = (int *)self->keyMappingTable;
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
            self->keyState =
                self->keyState | (((uint64_t)keyHigh << 32) | keyLow);

            char *table = (char *)self->actionTable->data();
            unsigned int offset = 0;
            for (unsigned int i = 0; i < self->actionTable->size(); i += 2) {
                char *entry = table + offset;
                if (*(unsigned int *)(entry + 8) == keyIndex && *(int *)(entry + 0x0c) == 0) {
                    actionLow |= *(uint32_t *)entry;
                    actionHigh |= *(uint32_t *)(entry + 4);
                    self->actionMask = actionLow;
                    self->actionMaskHigh = actionHigh;
                    self->actionState |= actionLow;
                    self->actionStateHigh |= actionHigh;
                }
                offset += 0x10;
            }
            break;
        }
        mapping += 4;
        ++keyIndex;
    }

    void *module = self->currentModule;
    if (module != 0 && self->field_0x3c == 5) {
        void **vtable = *(void ***)module;
        ((KeyCallback *)vtable[0x10 / 4])(module, module, keyLow, keyHigh, actionLow, actionHigh);
    }
}

__attribute__((minsize)) extern "C" void ApplicationManager_SoundSet(ApplicationManager *self, void *info, int count)
{
    if (info != 0) {
        void *sound = self->soundResource;
        if (sound != 0) {
            AESoundRessource_SetSound(sound, info, count);
            for (int i = 0; i < count; ++i) {
                ((AbyssEngine::AESoundRessource *)(self->soundResource))->init(i);
            }
        }
    }
}

__attribute__((minsize)) extern "C" void ApplicationManager_RegisterApplicationModule(ApplicationManager *self, unsigned int id, void *module)
{
    if (module != 0) {
        ((IApplicationModule *)(module))->SetApplicationManager(self);
        self->modules->push_back(module);
        self->moduleIds->push_back(id);
    }
}

void ApplicationManager::SetCurrentApplicationModule(unsigned int id) {
    char *pending = &g_perfPending;
    if (*pending != 0) {
        uint64_t *counter = &g_perfCounter;
        uint64_t limit = g_perfLimit;
        uint64_t value = *counter + 1;
        *counter = value;
        if ((long long)(value - limit) >= 0) {
            *(volatile char *)&g_perfExpired = 1;
            *(volatile char *)pending = 0;
        }
    }

    unsigned int count = this->moduleIds->size();
    unsigned int index = 0;
    while (index < count) {
        if ((*this->moduleIds)[index] == id) {
            void *module = (*this->modules)[index];
            void *current = this->currentModule;
            this->field_0x3c = current != 0;
            this->currentModuleId = id;
            this->pendingModule = module;
            return;
        }
        ++index;
    }
}

void ApplicationManager::ConvertTouchCoords(int *x, int *y) {
    ApplicationManager *manager = this;
    int *xPtr = x;
    int *yPtr = y;
    void *canvas = *(void **)manager;
    int orientation = *(int *)((char *)canvas + 0x30);
    int newY;

    if (orientation != 3) {
        if (orientation != 1) {
            if (orientation != 0) {
                return;
            }
            newY = *xPtr;
            *xPtr = pc_GetWidth((PaintCanvas *)canvas) - *yPtr;
            goto storeY;
        }
        int oldX = *xPtr;
        *xPtr = *yPtr;
        *yPtr = pc_GetHeight((PaintCanvas *)canvas) - oldX;
        canvas = *(void **)manager;
    }
    *xPtr = pc_GetWidth((PaintCanvas *)canvas) - *xPtr;
    newY = pc_GetHeight((PaintCanvas *)canvas) - *yPtr;

storeY:
    *yPtr = newY;
}

void ApplicationManager::VibrateSupported() {
    ext_001ab318(this->engine);
}

typedef void TouchBeginCallback(void *, int, int, void *);
typedef void TouchPressCallback(void *, int, int);

void ApplicationManager::OnTouchBegin(int xArg, int yArg, void *touch) {
    ApplicationManager *self = this;
    int x = xArg;
    int y = yArg;

    void *module = self->currentModule;
    if (module != 0 && self->field_0x3c == 5) {
        ((ApplicationManager *)(self))->ConvertTouchCoords(&x, &y);
        module = self->currentModule;
        void **vtable = *(void ***)module;
        ((TouchBeginCallback *)vtable[0x24 / 4])(module, x, y, touch);
        module = self->currentModule;
        vtable = *(void ***)module;
        ((TouchPressCallback *)vtable[0x18 / 4])(module, x, y);
        self->lastTouchX = x;
        self->lastTouchY = y;

        void *engine = self->engine;
        int mode = g_touchMode;
        if (mode <= 3) {
            void *canvas = *(void **)self;
            if (mode == 0 && x <= 0x31 && y <= 0x31) {
                g_touchMode = 1;
            } else if (mode == 1 && x > pc_GetWidth((PaintCanvas *)canvas) - 0x32 &&
                       y > pc_GetHeight((PaintCanvas *)canvas) - 0x32) {
                g_touchMode = 2;
            } else if (mode == 2 && x <= 0x31 && y > pc_GetHeight((PaintCanvas *)canvas) - 0x32) {
                uint8_t *flag = (uint8_t *)((char *)engine + 0x74);
                *flag = !*flag;
            } else if (mode == 3 && y <= 0x31 && x > pc_GetWidth((PaintCanvas *)canvas) - 0x32) {
                g_touchMode = 4;
            }
        } else if (*(uint8_t *)((char *)engine + 0x74)) {
            if (y < 100) {
                g_touchToggle ^= 1;
            } else {
                int height = pc_GetHeight((PaintCanvas *)*(void **)self);
                int width = pc_GetWidth((PaintCanvas *)*(void **)self);
                int half = width / 2;
                if (height - 100 < y) {
                    g_touchValue = x < half ? 0 : 1;
                } else {
                    g_touchFloat += x < half ? -1.0f : 1.0f;
                }
            }
        }
    }

    return;
}

void ApplicationManager::ConfigRegisterAction(long long value, long long key) {
    this->actionTable->push_back(value);
    this->actionTable->push_back(key);
}

void * ApplicationManager::GetApplicationModule(unsigned int id) {
    unsigned int count = this->moduleIds->size();
    unsigned int index = 0;
    goto check;

advance:
    ++index;
check:
    if (index >= count) {
        return 0;
    }
    if ((int)(*this->moduleIds)[index] != (int)id) {
        goto advance;
    }
    return (*this->modules)[index];
}

void * ApplicationManager::ConfigGetKeysForAction(long long action) {
    int low = (int)action;
    int high = (int)(action >> 32);
    unsigned int byteOffset = 0;
    Array<String *> *result = 0;
    unsigned int index = 0;
    goto check;

loop:
    {
        char *actions = (char *)this->actionTable->data();
        int actionLow = *(int *)(actions + byteOffset);
        int actionHigh = *(int *)(actions + byteOffset + 4);
        int mismatch = (actionLow ^ low) | (actionHigh ^ high);
        if (mismatch == 0) {
            if (result == 0) {
                result = new Array<String *>();
            }
            String *string = (String *)operator new(0xc);
            actions = (char *)this->actionTable->data();
            char *keys = this->keyMappingTable;
            unsigned int keyIndex = *(unsigned int *)(actions + byteOffset + 8);
            new (string) String(*(String *)(keys + keyIndex * 0x10 + 4));
            result->push_back(string);
        }
        byteOffset += 0x10;
        index += 2;
    }
check:
    if (index < this->actionTable->size()) {
        goto loop;
    }
    return result;
}

typedef void KeyCallback(void *, void *, unsigned int, unsigned int, unsigned int, unsigned int);

void ApplicationManager::OnKeyRelease(int key) {
    ApplicationManager *self = this;
    self->actionMask = 0;
    self->actionMaskHigh = 0;

    unsigned int keyLow = 0;
    unsigned int keyHigh = 0;
    unsigned int actionLow = 0;
    unsigned int actionHigh = 0;
    unsigned int keyIndex = 0;
    int *mapping = (int *)self->keyMappingTable;
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
            self->keyState &= ~keyLow;
            self->keyStateHigh &= ~keyHigh;

            char *table = (char *)self->actionTable->data();
            unsigned int offset = 0;
            for (unsigned int i = 0; i < self->actionTable->size(); i += 2) {
                char *entry = table + offset;
                if (*(unsigned int *)(entry + 8) == keyIndex && *(int *)(entry + 0x0c) == 0) {
                    actionLow |= *(uint32_t *)entry;
                    actionHigh |= *(uint32_t *)(entry + 4);
                    self->actionMask = actionLow;
                    self->actionMaskHigh = actionHigh;
                    self->actionState &= ~actionLow;
                    self->actionStateHigh &= ~actionHigh;
                }
                offset += 0x10;
            }
            break;
        }
        mapping += 4;
        ++keyIndex;
    }

    void *module = self->currentModule;
    if (module != 0 && self->field_0x3c == 5) {
        void **vtable = *(void ***)module;
        ((KeyCallback *)vtable[0x14 / 4])(module, module, keyLow, keyHigh, actionLow, actionHigh);
    }
}

void ApplicationManager::KeyCodeSetMapping(void *array) {
    unsigned int count = *(unsigned int *)array;
    if (count == 0x40) {
        unsigned int index = 0;
        int offset = 0;
        while (index < count) {
            uint32_t *mapping = *(uint32_t **)(*(char **)((char *)array + 4) + index * 4);
            char *dst = this->keyMappingTable + offset;
            *(uint32_t *)dst = *mapping;
            ((String *)((String *)(dst + 4)))->assign((String *)(mapping + 1));
            offset += 0x10;
            count = *(unsigned int *)array;
            ++index;
        }
    }
}

static bool update_orientation_timer(ApplicationManager *self, int *timer)
{
    int elapsed = self->frameTimeMs - self->previousFrameTimeMs;
    int value = *timer + elapsed;
    *timer = value;
    return value >= 0xfb;
}

void ApplicationManager::CheckForOrientationChange() {
    void *engine = this->engine;
    double tilt = *(double *)((char *)engine + 0x4b0);
    void *canvas;
    int orientation;
    int *timer;
    int target;

    if (tilt < -0.5) {
        canvas = *(void **)this;
        orientation = *(int *)((char *)canvas + 0x30);
        if (orientation == 0) {
            timer = &g_orientationLeft;
            target = 1;
            if (update_orientation_timer(this, timer)) {
                goto setOrientation;
            }
            return;
        }
    }

    if (tilt > 0.5) {
        canvas = *(void **)this;
        orientation = *(int *)((char *)canvas + 0x30);
        if (orientation == 3) {
            timer = &g_orientationRight;
            target = 2;
            if (update_orientation_timer(this, timer)) {
                goto setOrientation;
            }
            return;
        }
        if (orientation == 1) {
            timer = &g_orientationFlat;
            if (update_orientation_timer(this, timer)) {
                ((PaintCanvas *)canvas)->SetGameOrientation(0);
                *timer = 0;
            }
            return;
        }
    }

    if (tilt < -0.5) {
        canvas = *(void **)this;
        if (*(int *)((char *)canvas + 0x30) == 2) {
            timer = &g_orientationUpsideDown;
            target = 3;
            if (update_orientation_timer(this, timer)) {
                goto setOrientation;
            }
            return;
        }
    }

    g_orientationInactive = 0;
    return;

setOrientation:
    ((PaintCanvas *)canvas)->SetGameOrientation(target);
    *timer = 0;
}

typedef void ModuleCallback(void *);

__attribute__((minsize)) ApplicationManager::~ApplicationManager()
{
    ApplicationManager *self = this;
    void *module = self->currentModule;
    if (module != 0) {
        ModuleCallback **vtable = *(ModuleCallback ***)module;
        vtable[0x0c / 4](module);
    }

    for (unsigned int i = 0; i < self->modules->size(); ++i) {
        void *entry = (*self->modules)[i];
        if (entry != 0) {
            ModuleCallback **vtable = *(ModuleCallback ***)entry;
            vtable[1](entry);
        }
        (*self->modules)[i] = 0;
    }
    self->modules->clear();
    self->moduleIds->clear();

    void *canvas = *(void **)self;
    if (canvas != 0) {
        PaintCanvasDtor((AbyssEngine::PaintCanvas *)canvas);
        ::operator delete(canvas);
    }
    *(void **)self = 0;

    void *sound = self->soundResource;
    if (sound != 0) {
        AESoundRessource_dtor(sound);
        ::operator delete(sound);
    }
    self->soundResource = 0;

    void *cheats = self->cheatHandler;
    if (cheats != 0) {
        CheatHandler_dtor(cheats);
        ::operator delete(cheats);
    }
    self->cheatHandler = 0;

    void *reader = self->configReader;
    if (reader != 0) {
        ConfigReader_dtor(reader);
        ::operator delete(reader);
    }
    self->configReader = 0;

    char *keys = self->keyMappingTable;
    if (keys != 0) {
        unsigned int count = *(unsigned int *)(keys - 4);
        for (unsigned int i = count; i != 0; --i) {
            ((String *)(keys - 12 + i * 0x10))->~String();
        }
        ::operator delete[](keys - 8);
    }
    self->keyMappingTable = 0;

    delete self->actionTable;
    delete self->moduleIds;
    delete self->modules;
}

typedef void ConfigTokenReadFunction(ConfigReader *, void *);

extern "C" __attribute__((disable_tail_calls)) void ApplicationManager_ConfigRegisterTokenReadFunction(
    ApplicationManager *self, String *name, ConfigTokenReadFunction *read, void *context)
{
    unsigned char storage[sizeof(String)] __attribute__((aligned(4)));
    ConfigReader *reader = (ConfigReader *)self->configReader;
    if (reader != 0) {
        String *copy = (String *)storage;
        new (copy) String(*name);
        ((::AbyssEngine::ConfigReader *)reader)->RegisterTokenReadFunction(
            *copy, (::AbyssEngine::ConfigTokenReadFunction)read, context);
        ((String *)storage)->~String();
    }
    return;
}
