#include "gof2/ApplicationManager.h"
#include "gof2/AESoundRessource.h"
#include "gof2/externs.h"
#include "gof2/IApplicationModule.h"
#include "gof2/String.h"
#include "gof2/Engine.h"
#include "gof2/ConfigReader.h"
#include "gof2/PaintCanvasClass.h"   // real PaintCanvas:: methods

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
extern "C" void ArrayCtor_modules(void *array);
extern "C" void ArrayCtor_uint(void *array);
extern "C" void ArrayCtor_long_long(void *array);
extern "C" void AESoundRessource_ctor(void *sound);
extern "C" void ConfigReader_ctor(void *reader, void *engine);
extern "C" void CheatHandler_ctor(void *cheats, void *keys);
extern "C" void AESoundRessource_SetSound(void *sound, void *info, int count);
extern "C" void ArrayAdd_IApplicationModule(void *module, void *array);
extern "C" void ext_001ab5b8(unsigned int id, void *array);
extern "C" void ext_001ab318(void *engine);
extern "C" void ArrayAdd_long_long(long long value, void *array);
extern "C" void ext_001ab5f8(long long key, void *array);
extern "C" void Array_StringPtr_ctor(void *array);
extern "C" void ArrayAdd_StringPtr(String *string, void *array);
extern "C" void ArrayRelease_modules(void *array);
extern "C" void ArrayRelease_uint(void *array);
extern "C" void ArrayDtor_long_long(void *array);
extern "C" void ArrayDtor_uint(void *array);
extern "C" void ArrayDtor_modules(void *array);
extern "C" void AESoundRessource_dtor(void *sound);
extern "C" void CheatHandler_dtor(void *cheats);
extern "C" void ConfigReader_dtor(void *reader);

// ---- SetLoadingCallback_82506.cpp ----
typedef void LoadingCallback_t(PaintCanvas *, int, void *);

void ApplicationManager::SetLoadingCallback(LoadingCallback_t *callback, void *data) {
    ApplicationManager *self = this;
    self->loadingCallback = (void *)callback;
    self->loadingCallbackData = data;
}

// ---- SoundSetVolume_82270.cpp ----
void ApplicationManager::SoundSetVolume(int volume) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0) {
        ext_001ab578(sound, volume);
    }
}

// ---- SoundStopSounds_821fc.cpp ----
void ApplicationManager::SoundStopSounds() {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0) {
        ext_001ab528(sound);
    }
}

// ---- SoundIsPlaying_8229a.cpp ----
int ApplicationManager::SoundIsPlaying() {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound == 0) {
        return 0;
    }
    return ext_001ab5a8(sound);
}

// ---- GetSystemTimeMillis_83e9e.cpp ----
uint64_t ApplicationManager::GetSystemTimeMillis() {
    ApplicationManager *self = this;
    return self->currentTimeMs;
}

// ---- LoadingCallbackShow_8250c.cpp ----
typedef void LoadingShowCallback(PaintCanvas *, int, void *);

void ApplicationManager::LoadingCallbackShow(int mode, void *data) {
    ApplicationManager *self = this;
    LoadingShowCallback *callback = (LoadingShowCallback *)self->loadingCallback;
    if (callback != 0) {
        callback(*(PaintCanvas **)self, mode, data);
    }
}

// ---- SoundPlay_821b2.cpp ----
void ApplicationManager::SoundPlay(int soundId) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0 && self->soundFxEnabled) {
        ext_001ab4f8(sound, soundId);
    }
}

// ---- SoundPlay_821c6.cpp ----
void ApplicationManager::SoundPlay_vol(int soundId, float volume) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound == 0) {
        return;
    }
    if (self->soundFxEnabled == 0) {
        return;
    }
    ext_001ab488(sound, soundId, volume);
}

// ---- CheatUpdate_8253a.cpp ----
void ApplicationManager::CheatUpdate(unsigned short key) {
    ApplicationManager *self = this;
    if (self->cheatsEnabled) {
        void *cheats = self->cheatHandler;
        if (cheats != 0) {
            ext_001ab5e8(cheats, key);
        }
    }
}

// ---- CheatAddCode_82528.cpp ----
void ApplicationManager::CheatAddCode(void *code, int value) {
    ApplicationManager *self = this;
    void *cheats = self->cheatHandler;
    if (cheats != 0) {
        ext_001ab5d8(cheats);
    }
}

// ---- SoundPause_82218.cpp ----
void ApplicationManager::SoundPause() {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0) {
        ext_001ab548(sound);
    }
}

// ---- GetActionState_83e8a.cpp ----
uint64_t ApplicationManager::GetActionState() {
    ApplicationManager *self = this;
    return self->actionState;
}

// ---- Resume_824d0.cpp ----
typedef void ModuleCallback(void *);


void ApplicationManager::Resume() {
    ApplicationManager *self = this;
    if (self->field_0x3c != 3) {
        return;
    }

    void *module = self->currentModule;
    if (module != 0) {
        ModuleCallback **vtable = *(ModuleCallback ***)module;
        vtable[0x40 / 4](module);
        void *engine = self->engine;
        if (engine != 0) {
            ((Engine *)(engine))->Resume();
        }
        self->actionState = 0;
        self->actionStateHigh = 0;
        self->keyState = 0;
        self->keyStateHigh = 0;
        self->field_0x3c = 4;
    }
}

// ---- SoundStop_821ee.cpp ----
void ApplicationManager::SoundStop() {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0) {
        ext_001ab518(sound);
    }
}

// ---- SoundPlayLoop_821da.cpp ----
void ApplicationManager::SoundPlayLoop(int soundId) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0 && self->soundFxEnabled) {
        ext_001ab508(sound, soundId);
    }
}

// ---- SoundPlayMusicLoop_8219e.cpp ----
void ApplicationManager::SoundPlayMusicLoop(int soundId) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0 && self->musicEnabled) {
        ext_001ab4e8(sound, soundId);
    }
}

// ---- ResetKeyState_83e90.cpp ----
void ApplicationManager::ResetKeyState() {
    ApplicationManager *self = this;
    self->keyState = 0;
}

// ---- GetElapsedTimeMillis_827dc.cpp ----
uint64_t ApplicationManager::GetElapsedTimeMillis() {
    ApplicationManager *self = this;
    return self->frameTimeMs - self->previousFrameTimeMs;
}

// ---- SoundMusicEnable_8226a.cpp ----
void ApplicationManager::SoundMusicEnable(bool enable) {
    ApplicationManager *self = this;
    self->musicEnabled = enable;
}

// ---- CheatEnable_82534.cpp ----
void ApplicationManager::CheatEnable(bool enable) {
    ApplicationManager *self = this;
    self->cheatsEnabled = enable;
}

// ---- GetCurrentTimeMillis_83e98.cpp ----
uint64_t ApplicationManager::GetCurrentTimeMillis() {
    ApplicationManager *self = this;
    return self->currentTimeMs;
}

// ---- SoundPlayMusic_8218a.cpp ----
void ApplicationManager::SoundPlayMusic(int soundId) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0 && self->musicEnabled) {
        ext_001ab4d8(sound, soundId);
    }
}

// ---- OnTouchEnd_83c90.cpp ----
void ApplicationManager::OnTouchEndSimple() {
    ApplicationManager *self = this;
    self->currentKey = 0;
    self->currentKeyHigh = 0;
    self->keyState = 0;
    self->keyStateHigh = 0;
    // Original wrote a 16-byte vector zero across +0x98..+0xa7 (action mask + action state).
    self->actionMask = 0;
    self->actionMaskHigh = 0;
    self->actionState = 0;
    self->actionStateHigh = 0;
}

// ---- SoundEnable_8225a.cpp ----
void ApplicationManager::SoundEnable(bool enable) {
    ApplicationManager *self = this;
    self->soundFxEnabled = enable;
    self->musicEnabled = enable;
}

// ---- SoundSetMusicVolume_8228c.cpp ----
void ApplicationManager::SoundSetMusicVolume(int volume) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0) {
        ext_001ab598(sound);
    }
}

// ---- SetResumeCallback_82516.cpp ----
typedef bool ResumeCallback_t(PaintCanvas *, void *);

void ApplicationManager::SetResumeCallback(ResumeCallback_t *callback, void *data) {
    ApplicationManager *self = this;
    self->resumeCallback = (void *)callback;
    self->resumeCallbackData = data;
}

// ---- GetApplicationVersionString_82480.cpp ----
extern "C" __attribute__((disable_tail_calls)) void ApplicationManager_GetApplicationVersionString(String *out)
{
    new (out) String();
    out->s = u"2.0.16";
}

// ---- VibrateEnable_822b6.cpp ----
void ApplicationManager::VibrateEnable(bool enable) {
    ApplicationManager *self = this;
    self->vibrateEnabled = enable;
}

// ---- SoundResume_82226.cpp ----
void ApplicationManager::SoundResume(int soundId) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if ((sound != 0 && self->soundFxEnabled) || self->musicEnabled) {
        ext_001ab558(sound, soundId);
    }
}

// ---- SoundRelease_8217c.cpp ----
void ApplicationManager::SoundRelease() {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0) {
        ext_001ab4c8(sound);
    }
}

// ---- Quit_82476.cpp ----
typedef void QuitCallback_t();

void ApplicationManager::Quit() {
    ApplicationManager *self = this;
    QuitCallback_t *callback = (QuitCallback_t *)self->quitCallback;
    if (callback != 0) {
        callback();
    }
}

// ---- GetEngine_8249c.cpp ----
void * ApplicationManager::GetEngine() {
    ApplicationManager *self = this;
    return self->engine;
}

// ---- Suspend_824a2.cpp ----
typedef void ModuleCallback(void *);


void ApplicationManager::Suspend() {
    ApplicationManager *self = this;
    unsigned int state = self->field_0x3c;
    if (state - 3 < 2) {
        return;
    }

    void *module = self->currentModule;
    if (module != 0) {
        ModuleCallback **vtable = *(ModuleCallback ***)module;
        vtable[0x3c / 4](module);
        void *engine = self->engine;
        if (engine != 0) {
            ((Engine *)(engine))->Suspend();
        }
        int oldState = self->field_0x3c;
        self->field_0x3c = 3;
        self->savedState = oldState;
    }
}

// ---- SoundFxEnable_82264.cpp ----
void ApplicationManager::SoundFxEnable(bool enable) {
    ApplicationManager *self = this;
    self->soundFxEnabled = enable;
}

// ---- SoundResume_822a8.cpp ----
void ApplicationManager::SoundResumeSelf() {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0) {
        ext_001ab568(sound);
    }
}

// ---- Vibrate_822c4.cpp ----
void ApplicationManager::Vibrate(unsigned short duration) {
    ApplicationManager *self = this;
    if (self->vibrateEnabled) {
        ext_001ab328(self->engine, duration);
    }
}

// ---- SoundSetFXVolume_8227e.cpp ----
void ApplicationManager::SoundSetFXVolume(int volume) {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0) {
        ext_001ab588(sound);
    }
}

// ---- CheatSetCallback_8251c.cpp ----
void ApplicationManager::CheatSetCallback(void *callback, void *data) {
    ApplicationManager *self = this;
    void *cheats = self->cheatHandler;
    if (cheats != 0) {
        ext_001ab5c8(cheats);
    }
}

// ---- SoundResumeSounds_82240.cpp ----
void ApplicationManager::SoundResumeSounds() {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if ((sound != 0 && self->soundFxEnabled) || self->musicEnabled) {
        ext_001ab568(sound);
    }
}

// ---- SoundPauseSounds_8220a.cpp ----
void ApplicationManager::SoundPauseSounds() {
    ApplicationManager *self = this;
    void *sound = self->soundResource;
    if (sound != 0) {
        ext_001ab538(sound);
    }
}

// ---- GetKeyState_83e84.cpp ----
uint64_t ApplicationManager::GetKeyState() {
    ApplicationManager *self = this;
    return self->keyState;
}

// ---- SetApplicationModule_82440.cpp ----
void ApplicationManager::SetApplicationModule(void *module) {
    ApplicationManager *self = this;
    void *current = self->currentModule;
    self->pendingModule = module;
    self->field_0x3c = current != 0;
}

// ---- ConfigReadFile_8254c.cpp ----
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

// ---- OnTouchEnd_83c18.cpp ----
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

// ---- EnablePerformanceTest_82334.cpp ----
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

// ---- OnUpdate_827f0.cpp ----
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

// ---- ApplicationManager_8193c.cpp ----
ApplicationManager * ApplicationManager::ctor(void *engine) {
    ApplicationManager *self = this;

    ArrayCtor_modules((char *)self + 0x44);
    ArrayCtor_uint((char *)self + 0x50);
    ArrayCtor_long_long((char *)self + 0x88);

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

    return self;
}

// ---- OnTouchMove_83bb0.cpp ----
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

// ---- OnKeyPress_83ca6.cpp ----
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

            unsigned int offset = 0;
            for (unsigned int i = 0; i < self->actionTableCount; i += 2) {
                char *entry = self->actionTableData + offset;
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

// ---- SoundSet_82148.cpp ----
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

// ---- RegisterApplicationModule_822d4.cpp ----
__attribute__((minsize)) extern "C" void ApplicationManager_RegisterApplicationModule(ApplicationManager *self, unsigned int id, void *module)
{
    if (module != 0) {
        ((IApplicationModule *)(module))->SetApplicationManager(self);
        ArrayAdd_IApplicationModule(module, (char *)self + 0x44);
        ext_001ab5b8(id, (char *)self + 0x50);
    }
}

// ---- SetCurrentApplicationModule_823b0.cpp ----
void ApplicationManager::SetCurrentApplicationModule(unsigned int id) {
    ApplicationManager *self = this;
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

    unsigned int count = self->moduleIdCount;
    unsigned int index = 0;
    while (index < count) {
        if (*(unsigned int *)(self->moduleIdData + index * 4) == id) {
            void *module = *(void **)(self->modulesData + index * 4);
            void *current = self->currentModule;
            self->field_0x3c = current != 0;
            self->currentModuleId = id;
            self->pendingModule = module;
            return;
        }
        ++index;
    }
}

// ---- ConvertTouchCoords_839de.cpp ----
void ApplicationManager::ConvertTouchCoords(int *x, int *y) {
    ApplicationManager *self = this;
    ApplicationManager *manager = self;
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

// ---- VibrateSupported_822bc.cpp ----
void ApplicationManager::VibrateSupported() {
    ApplicationManager *self = this;
    ext_001ab318(self->engine);
}

// ---- OnTouchBegin_83a38.cpp ----
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

// ---- ConfigRegisterAction_82600.cpp ----
void ApplicationManager::ConfigRegisterAction(long long value, long long key) {
    ApplicationManager *self = this;
    void *array = (char *)self + 0x88;
    ArrayAdd_long_long(value, array);
    ext_001ab5f8(key, array);
}

// ---- GetApplicationModule_82452.cpp ----
void * ApplicationManager::GetApplicationModule(unsigned int id) {
    ApplicationManager *self = this;
    unsigned int count = self->moduleIdCount;
    unsigned int index = 0;
    goto check;

advance:
    ++index;
check:
    if (index >= count) {
        return 0;
    }
    if (*(int *)(self->moduleIdData + index * 4) != (int)id) {
        goto advance;
    }
    return *(void **)(self->modulesData + index * 4);
}

// ---- ConfigGetKeysForAction_82654.cpp ----
void * ApplicationManager::ConfigGetKeysForAction(long long action) {
    ApplicationManager *self = this;
    int low = (int)action;
    int high = (int)(action >> 32);
    unsigned int byteOffset = 0;
    void *result = 0;
    unsigned int index = 0;
    goto check;

loop:
    {
        char *actions = self->actionTableData;
        int actionLow = *(int *)(actions + byteOffset);
        int actionHigh = *(int *)(actions + byteOffset + 4);
        int mismatch = (actionLow ^ low) | (actionHigh ^ high);
        if (mismatch == 0) {
            if (result == 0) {
                result = operator new(0xc);
                Array_StringPtr_ctor(result);
            }
            String *string = (String *)operator new(0xc);
            actions = self->actionTableData;
            char *keys = self->keyMappingTable;
            unsigned int keyIndex = *(unsigned int *)(actions + byteOffset + 8);
            new (string) String(*(String *)(keys + keyIndex * 0x10 + 4));
            ArrayAdd_StringPtr(string, result);
        }
        byteOffset += 0x10;
        index += 2;
    }
check:
    if (index < self->actionTableCount) {
        goto loop;
    }
    return result;
}

// ---- OnKeyRelease_83d98.cpp ----
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

            unsigned int offset = 0;
            for (unsigned int i = 0; i < self->actionTableCount; i += 2) {
                char *entry = self->actionTableData + offset;
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

// ---- KeyCodeSetMapping_83ea4.cpp ----
void ApplicationManager::KeyCodeSetMapping(void *array) {
    ApplicationManager *self = this;
    unsigned int count = *(unsigned int *)array;
    if (count == 0x40) {
        unsigned int index = 0;
        int offset = 0;
        while (index < count) {
            uint32_t *mapping = *(uint32_t **)(*(char **)((char *)array + 4) + index * 4);
            char *dst = self->keyMappingTable + offset;
            *(uint32_t *)dst = *mapping;
            ((String *)((String *)(dst + 4)))->assign((String *)(mapping + 1));
            offset += 0x10;
            count = *(unsigned int *)array;
            ++index;
        }
    }
}

// ---- CheckForOrientationChange_826e8.cpp ----
static bool update_orientation_timer(ApplicationManager *self, int *timer)
{
    int elapsed = self->frameTimeMs - self->previousFrameTimeMs;
    int value = *timer + elapsed;
    *timer = value;
    return value >= 0xfb;
}

void ApplicationManager::CheckForOrientationChange() {
    ApplicationManager *self = this;
    void *engine = self->engine;
    double tilt = *(double *)((char *)engine + 0x4b0);
    void *canvas;
    int orientation;
    int *timer;
    int target;

    if (tilt < -0.5) {
        canvas = *(void **)self;
        orientation = *(int *)((char *)canvas + 0x30);
        if (orientation == 0) {
            timer = &g_orientationLeft;
            target = 1;
            if (update_orientation_timer(self, timer)) {
                goto setOrientation;
            }
            return;
        }
    }

    if (tilt > 0.5) {
        canvas = *(void **)self;
        orientation = *(int *)((char *)canvas + 0x30);
        if (orientation == 3) {
            timer = &g_orientationRight;
            target = 2;
            if (update_orientation_timer(self, timer)) {
                goto setOrientation;
            }
            return;
        }
        if (orientation == 1) {
            timer = &g_orientationFlat;
            if (update_orientation_timer(self, timer)) {
                ((PaintCanvas *)canvas)->SetGameOrientation(0);
                *timer = 0;
            }
            return;
        }
    }

    if (tilt < -0.5) {
        canvas = *(void **)self;
        if (*(int *)((char *)canvas + 0x30) == 2) {
            timer = &g_orientationUpsideDown;
            target = 3;
            if (update_orientation_timer(self, timer)) {
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

// ---- _ApplicationManager_82038.cpp ----
typedef void ModuleCallback(void *);

__attribute__((minsize)) ApplicationManager::~ApplicationManager()
{
    ApplicationManager *self = this;
    void *module = self->currentModule;
    if (module != 0) {
        ModuleCallback **vtable = *(ModuleCallback ***)module;
        vtable[0x0c / 4](module);
    }

    void *modules = (char *)self + 0x44;
    unsigned int offset = 0;
    for (unsigned int i = 0; i < *(unsigned int *)modules; ++i) {
        void **slot = (void **)(self->modulesData + offset);
        void *entry = *slot;
        if (entry != 0) {
            ModuleCallback **vtable = *(ModuleCallback ***)entry;
            vtable[1](entry);
            slot = (void **)(self->modulesData + offset);
        }
        *slot = 0;
        offset += 4;
    }
    ArrayRelease_modules(modules);
    ArrayRelease_uint((char *)self + 0x50);

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

    ArrayDtor_long_long((char *)self + 0x88);
    ArrayDtor_uint((char *)self + 0x50);
    ArrayDtor_modules(modules);
}

// ---- ConfigRegisterTokenReadFunction_825a4.cpp ----
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
