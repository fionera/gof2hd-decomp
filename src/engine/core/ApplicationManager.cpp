#include "engine/core/ApplicationManager.h"
#include "game/core/Globals.h"
#include "engine/audio/AESoundRessource.h"
#include "engine/core/IApplicationModule.h"
#include "engine/file/ConfigReader.h"
#include "engine/render/Engine.h"
#include "game/core/CheatHandler.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/KeyCode.h"
#include "engine/core/AbyssEngine.h"
#include <cstddef>

#if __SIZEOF_POINTER__ == 4
static_assert(sizeof(AbyssEngine::KeyCode) == 0x10, "KeyCode entry size");
static_assert(offsetof(AbyssEngine::KeyCode, name) == 0x04, "KeyCode name offset");
#endif

ApplicationManager::ApplicationManager(Engine *engine) {
    AbyssEngine::debugTouch[0] = 0.0f;
    AbyssEngine::debugTouch[1] = 0.0f;
    AbyssEngine::debugTouch[2] = 0.0f;
    AbyssEngine::debugTouch[3] = 0.0f;
    this->quitCallback = 0;
    this->state = 5;
    this->applicationData = 0;
    this->pendingModule = 0;
    this->frameTimeMs = 0;
    this->currentKey = 0;
    this->previousFrameTimeMs = 0;
    this->currentTimeMs = 0;
    this->currentModule = 0;
    this->keyState = 0;
    this->engine = engine;

    this->paintCanvas = new PaintCanvas(engine);
    this->soundResource = new AESoundRessource();
    this->cheatsEnabled = false;
    this->configReader = new ConfigReader(engine);
    this->loadingCallback = 0;
    this->resumeCallback = 0;
    this->soundFxEnabled = true;
    this->musicEnabled = true;
    this->vibrateEnabled = true;
    this->orientationTrackingEnabled = false;

    AbyssEngine::KeyCode defaultKeys[50] = {
            {0, String("0", false)},
            {1, String("1", false)},
            {2, String("2", false)},
            {3, String("3", false)},
            {4, String("4", false)},
            {5, String("5", false)},
            {6, String("6", false)},
            {7, String("7", false)},
            {8, String("8", false)},
            {9, String("9", false)},
            {10, String("#", false)},
            {11, String("*", false)},
            {12, String("D-pad left", false)},
            {13, String("D-pad right", false)},
            {14, String("D-pad up", false)},
            {15, String("D-pad down", false)},
            {0, String("D-pad fire", false)},
            {0, String("Softbutton left", false)},
            {0, String("Softbutton right", false)},
            {0, String("CLR", false)},
            {97, String("A", false)},
            {98, String("B", false)},
            {99, String("C", false)},
            {100, String("D", false)},
            {101, String("E", false)},
            {102, String("F", false)},
            {103, String("G", false)},
            {104, String("H", false)},
            {105, String("I", false)},
            {106, String("J", false)},
            {107, String("K", false)},
            {108, String("L", false)},
            {109, String("M", false)},
            {110, String("N", false)},
            {111, String("O", false)},
            {112, String("P", false)},
            {113, String("Q", false)},
            {114, String("R", false)},
            {115, String("S", false)},
            {116, String("T", false)},
            {117, String("U", false)},
            {118, String("V", false)},
            {119, String("W", false)},
            {120, String("X", false)},
            {121, String("Y", false)},
            {122, String("Z", false)},
            {32, String("Space", false)},
            {10, String("Enter", false)},
            {16, String("Menu", false)},
            {17, String("CAPLK", false)},
    };
    this->keyMappingTable = new AbyssEngine::KeyCode[0x40];
    for (unsigned int i = 0; i < 50; ++i) {
        this->keyMappingTable[i] = defaultKeys[i];
    }

    this->cheatHandler = new CheatHandler(this->keyMappingTable);
    this->lastTouchX = -1;
    this->lastTouchY = -1;
}

ApplicationManager::~ApplicationManager() {
    IApplicationModule *module = this->currentModule;
    if (module != 0) {
        module->OnRelease();
    }

    for (unsigned int i = 0; i < this->modules.size(); ++i) {
        IApplicationModule *entry = this->modules[i];
        if (entry != 0) {
            delete entry;
        }
        this->modules[i] = 0;
    }
    ArrayRelease(this->modules);
    ArrayRelease(this->moduleIds);

    delete this->paintCanvas;
    this->paintCanvas = 0;

    delete this->soundResource;
    this->soundResource = 0;

    delete this->cheatHandler;
    this->cheatHandler = 0;

    delete this->configReader;
    this->configReader = 0;

    delete[] this->keyMappingTable;
    this->keyMappingTable = 0;

}

void ApplicationManager::SetApplicationModule(IApplicationModule *module) {
    IApplicationModule *current = this->currentModule;
    this->pendingModule = module;
    this->state = current != 0;
}

void ApplicationManager::SetCurrentApplicationModule(unsigned int id) {
    if (AbyssEngine::performanceTest) {
        AbyssEngine::performanceModulSwitches++;
        if (AbyssEngine::performanceModulSwitches >= AbyssEngine::performanceOverallModulSwitches) {
            AbyssEngine::performanceTest = 0;
            AbyssEngine::performanceTestEnded = 1;
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

void *ApplicationManager::GetApplicationModule(unsigned int id) { // lint: void_ptr (exported GetApplicationModule return type, baked ABI)
    // lint: void_ptr (exported GetApplicationModule return type, baked ABI)
    unsigned int count = this->moduleIds.size();
    for (unsigned int index = 0; index < count; ++index) {
        if ((int) this->moduleIds[index] == (int) id) {
            return this->modules[index];
        }
    }
    return 0;
}

void *ApplicationManager::GetEngine() { // lint: void_ptr (exported GetEngine return type, header-declared ABI)
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
        module->OnResume();
        if (this->engine != 0) {
            this->engine->Resume();
        }
        this->actionState = 0;
        this->keyState = 0;
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
        module->OnSuspend();
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
                int loading = module->OnInitialize();
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
                module->OnRelease();
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
                module->OnUpdate();
                Engine *engine = this->engine;
                engine->triangleCountA = 0;
                engine->field_0x58 = 0;
                this->paintCanvas->culledCount = 0;
                module->OnRender3D();
                ResumeCallback_t *resume = this->resumeCallback;
                if (resume == 0 || !resume(this->paintCanvas, this->resumeCallbackData)) {
                    module->OnRender2D();
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

    long long keys = 0;
    long long actions = 0;
    for (unsigned int i = 0; i < 0x40; ++i) {
        if (this->keyMappingTable[i].code == key) {
            keys = 1LL << i;
            this->keyState |= keys;
            for (unsigned int j = 0; j < this->actionTable.size(); j += 2) {
                if (this->actionTable[j + 1] == i) {
                    actions |= this->actionTable[j];
                    this->actionMask = actions;
                    this->actionState |= actions;
                }
            }
            break;
        }
    }

    IApplicationModule *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        module->OnKeyPress(keys, actions);
    }
}

void ApplicationManager::OnKeyRelease(int key) {
    this->actionMask = 0;

    long long keys = 0;
    long long actions = 0;
    for (unsigned int i = 0; i < 0x40; ++i) {
        if (this->keyMappingTable[i].code == key) {
            keys = 1LL << i;
            this->keyState &= ~keys;
            for (unsigned int j = 0; j < this->actionTable.size(); j += 2) {
                if (this->actionTable[j + 1] == i) {
                    actions |= this->actionTable[j];
                    this->actionMask = actions;
                    this->actionState &= ~actions;
                }
            }
            break;
        }
    }

    IApplicationModule *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        module->OnKeyRelease(keys, actions);
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
    if (array->size() == 0x40) {
        for (unsigned int i = 0; i < array->size(); ++i) {
            AbyssEngine::KeyCode *mapping = (*array)[i];
            this->keyMappingTable[i].code = mapping->code;
            this->keyMappingTable[i].name = mapping->name;
        }
    }
}

void ApplicationManager::ConvertTouchCoords(int &x, int &y) {
    int old;
    switch (this->paintCanvas->gameOrientation) {
        case 0:
            old = x;
            x = this->paintCanvas->GetWidth() - y;
            y = old;
            break;
        case 1:
            old = x;
            x = y;
            y = this->paintCanvas->GetHeight() - old;
            // fall through: then mirror both axes
        case 3:
            x = this->paintCanvas->GetWidth() - x;
            y = this->paintCanvas->GetHeight() - y;
            break;
    }
}

void ApplicationManager::OnTouchBegin(int x, int y, void *touch) { // lint: void_ptr (exported OnTouchBegin touch handle param, baked ABI)
    // lint: void_ptr (exported OnTouchBegin touch handle param, baked ABI)
    IApplicationModule *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        this->ConvertTouchCoords(x, y);
        module = this->currentModule;
        module->OnTouchBegin(x, y, touch);
        module = this->currentModule;
        module->OnTouchBegin(x, y);
        this->lastTouchX = x;
        this->lastTouchY = y;

        switch (AbyssEngine::debugModusIndex) {
            case 0:
                if (x <= 49 && y <= 49) {
                    AbyssEngine::debugModusIndex = 1;
                    return;
                }
                break;
            case 1:
                if (x > this->paintCanvas->GetWidth() - 50 && y > this->paintCanvas->GetHeight() - 50) {
                    AbyssEngine::debugModusIndex++;
                    return;
                }
                break;
            case 2:
                if (x <= 49 && y > this->paintCanvas->GetHeight() - 50) {
                    this->engine->field_0x74 = !this->engine->field_0x74;
                    return;
                }
                break;
            case 3: {
                int width = this->paintCanvas->GetWidth();
                if (y <= 49 && x > width - 50) {
                    AbyssEngine::debugModusIndex++;
                    return;
                }
                break;
            }
        }

        if (this->engine->field_0x74) {
            if (y < 100) {
                Engine::UseAdvancedShader ^= 1;
            } else {
                int height = this->paintCanvas->GetHeight();
                int half = this->paintCanvas->GetWidth() / 2;
                if (y > height - 100) {
                    Engine::LodDistShader = x >= half ? 10000000.0f : 0.0f;
                } else {
                    Engine::LodDistShader += x >= half ? 100.0f : -100.0f;
                }
            }
        }
    }
}

void ApplicationManager::OnTouchMove(int x, int y, void *touch) { // lint: void_ptr (exported OnTouchMove touch handle param, baked ABI)
    // lint: void_ptr (exported OnTouchMove touch handle param, baked ABI)
    IApplicationModule *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        this->ConvertTouchCoords(x, y);
        module = this->currentModule;
        module->OnTouchMove(x, y, touch);
        module = this->currentModule;
        module->OnTouchMove(x, y);
        this->lastTouchX = x;
        this->lastTouchY = y;
    }
}

void ApplicationManager::OnTouchEnd(int x, int y, void *touch) { // lint: void_ptr (exported OnTouchEnd touch handle param, baked ABI)
    // lint: void_ptr (exported OnTouchEnd touch handle param, baked ABI)
    AbyssEngine::debugModusIndex = 0;

    IApplicationModule *module = this->currentModule;
    if (module != 0 && this->state == 5) {
        this->ConvertTouchCoords(x, y);
        module = this->currentModule;
        module->OnTouchEnd(x, y, touch);
        module = this->currentModule;
        module->OnTouchEnd(x, y);
        this->lastTouchX = x;
        this->lastTouchY = y;
    }
}

void ApplicationManager::OnTouchEnd() {
    this->currentKey = 0;
    this->keyState = 0;
    this->actionMask = 0;
    this->actionState = 0;
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

static bool update_orientation_timer(ApplicationManager *self) {
    AbyssEngine::orientationChangeTimer += (int) (self->frameTimeMs - self->previousFrameTimeMs);
    return AbyssEngine::orientationChangeTimer > 250;
}

void ApplicationManager::CheckForOrientationChange() {
    double tilt = this->engine->accelRaw[0];

    if (tilt < -0.5 && this->paintCanvas->gameOrientation == 0) {
        if (update_orientation_timer(this)) {
            this->paintCanvas->SetGameOrientation(AbyssEngine::LandscapeMode_1);
            AbyssEngine::orientationChangeTimer = 0;
        }
    } else if (tilt > 0.5 && this->paintCanvas->gameOrientation == 1) {
        if (update_orientation_timer(this)) {
            this->paintCanvas->SetGameOrientation(AbyssEngine::LandscapeMode_dummy);
            AbyssEngine::orientationChangeTimer = 0;
        }
    } else if (tilt > 0.5 && this->paintCanvas->gameOrientation == 3) {
        if (update_orientation_timer(this)) {
            this->paintCanvas->SetGameOrientation(AbyssEngine::LandscapeMode_2);
            AbyssEngine::orientationChangeTimer = 0;
        }
    } else if (tilt < -0.5 && this->paintCanvas->gameOrientation == 2) {
        if (update_orientation_timer(this)) {
            this->paintCanvas->SetGameOrientation(AbyssEngine::LandscapeMode_3);
            AbyssEngine::orientationChangeTimer = 0;
        }
    } else {
        AbyssEngine::orientationChangeTimer = 0;
    }
}

void ApplicationManager::ConfigRegisterAction(long long value, long long key) {
    ArrayAdd(value, this->actionTable);
    ArrayAdd(key, this->actionTable);
}

void *ApplicationManager::ConfigGetKeysForAction(long long action) { // lint: void_ptr (exported ConfigGetKeysForAction return type, header-declared ABI)
    // lint: void_ptr (exported ConfigGetKeysForAction return type, header-declared ABI)
    Array<String *> *result = 0;
    for (unsigned int i = 0; i < this->actionTable.size(); i += 2) {
        if (this->actionTable[i] == action) {
            if (result == 0) {
                result = new Array<String *>();
            }
            ArrayAdd(new String(this->keyMappingTable[this->actionTable[i + 1]].name, false), *result);
        }
    }
    return result;
}

void ApplicationManager::SetLoadingCallback(LoadingCallback_t *callback, void *data) { // lint: void_ptr (opaque user data forwarded to LoadingCallback_t, baked ABI)
    // lint: void_ptr (opaque user data forwarded to LoadingCallback_t, baked ABI)
    this->loadingCallback = callback;
    this->loadingCallbackData = data;
}

void ApplicationManager::SetResumeCallback(ResumeCallback_t *callback, void *data) { // lint: void_ptr (opaque user data forwarded to ResumeCallback_t, baked ABI)
    // lint: void_ptr (opaque user data forwarded to ResumeCallback_t, baked ABI)
    this->resumeCallback = callback;
    this->resumeCallbackData = data;
}

void ApplicationManager::LoadingCallbackShow(int mode, void *data) { // lint: void_ptr (opaque user data forwarded to LoadingCallback_t, baked ABI)
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

void ApplicationManager::CheatSetCallback(void (*callback)(int, void *), void *data) { // lint: void_ptr (exported CheatSetCallback signature, opaque cheat callback + data, baked ABI)
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
    if (this->soundResource != 0) {
        this->soundResource->resume();
    }
}

void ApplicationManager::SoundResume(int soundId) {
    if ((this->soundResource != 0 && this->soundFxEnabled) || this->musicEnabled) {
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

bool ApplicationManager::SoundIsPlaying(int soundId) {
    if (this->soundResource == 0) {
        return false;
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
    this->musicEnabled = enable;
    this->soundFxEnabled = enable;
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

void *ApplicationManager::GetApplicationData() { // lint: void_ptr (exported GetApplicationData return type, baked ABI)
    // lint: void_ptr (exported GetApplicationData return type, baked ABI)
    return this->applicationData;
}

void ApplicationManager::SetApplicationData(void *data) { // lint: void_ptr (exported SetApplicationData param, opaque app data, baked ABI)
    // lint: void_ptr (exported SetApplicationData param, opaque app data, baked ABI)
    this->applicationData = data;
}

void ApplicationManager::SetExitCallback(QuitCallback_t *callback) {
    this->quitCallback = callback;
}

unsigned int ApplicationManager::GetCurrentApplicationModule() const {
    return this->currentModuleId;
}

String ApplicationManager::GetApplicationVersionString() {
    return String("2.0.16");
}

void ApplicationManager::ConfigReadFile(String name) {
    ConfigReader *reader = this->configReader;
    if (reader != 0) {
        reader->ParseFile(String(name, false));
    }
}

void ApplicationManager::ConfigRegisterTokenReadFunction(String name, ConfigTokenReadFunction read,
                                                         void *context) { // lint: void_ptr (exported ConfigRegisterTokenReadFunction opaque context param, baked ABI)
    // lint: void_ptr (exported ConfigRegisterTokenReadFunction opaque context param, baked ABI)
    ConfigReader *reader = this->configReader;
    if (reader != 0) {
        reader->RegisterTokenReadFunction(String(name, false), read, context);
    }
}

void ApplicationManager::EnablePerformanceTest(int count) {
    AbyssEngine::performanceOverallModulSwitches = count;
    AbyssEngine::performanceModulSwitches = 0;
    AbyssEngine::performanceFrameCounter = 0;
    AbyssEngine::performanceTime = 0;
    AbyssEngine::triDrawn = 0;
    AbyssEngine::performanceTest = 1;
    AbyssEngine::performanceTestEnded = 0;
}

void ApplicationManager::SoundSet(const AESoundInfo *info, int count) {
    if (info != 0 && this->soundResource != 0) {
        this->soundResource->SetSound(info, count);
        for (int i = 0; i < count; ++i) {
            this->soundResource->init(i);
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
