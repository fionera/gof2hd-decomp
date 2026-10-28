#include "engine/core/ApplicationManager.h"
#include "game/core/Globals.h"
#include "engine/audio/AESoundRessource.h"
#include "engine/core/IApplicationModule.h"
#include "engine/file/ConfigReader.h"
#include "engine/render/Engine.h"
#include "game/core/CheatHandler.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/AELoadedTexture.h"
#include "engine/render/ShaderBaseStruct.h"
#include "engine/core/KeyCode.h"
#include "engine/core/AbyssEngine.h"
#include <cstddef>

#if __SIZEOF_POINTER__ == 4
static_assert(sizeof(AbyssEngine::KeyCode) == 0x10, "KeyCode entry size");
static_assert(offsetof(AbyssEngine::KeyCode, name) == 0x04, "KeyCode name offset");
#endif


namespace AbyssEngine {
    int TextureCreateFromFileIntern(Engine *engine, const char *path, void (*cb)(Image *, void *), // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    void *user, unsigned int *outIds, float aniso, // lint: void_ptr (exported TextureCreateFromFileIntern signature)
                                    AELoadedTexture *outTex, bool flag);
}

namespace AbyssEngine {
    float debugTouch[4];
    long long performanceFrameCounter;
    long long performanceOverallModulSwitches;
    long long performanceModulSwitches;
    long long triDrawn;
    long long performanceTime;
    unsigned char performanceTest;
    unsigned char performanceTestEnded;
    int orientationChangeTimer;
    int fpsTimer;
    int fpsCounter;
    int currentFps;
    int SwapCounter;
    int loadTexture;
    int loadMesh;
    bool PostEffectFlag;
    int debugModusIndex;
}


static bool update_orientation_timer(ApplicationManager *self) {
    AbyssEngine::orientationChangeTimer += (int) (self->frameTimeMs - self->previousFrameTimeMs);
    return AbyssEngine::orientationChangeTimer > 250;
}


Engine *GetEngine() {
    return gEngine;
}

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
void ApplicationManager::SoundSet(const AESoundInfo *info, int count) {
    if (info != 0 && this->soundResource != 0) {
        this->soundResource->SetSound(info, count);
        for (int i = 0; i < count; ++i) {
            this->soundResource->init(i);
        }
    }
}
void ApplicationManager::SoundRelease(int soundId) {
    if (this->soundResource != 0) {
        this->soundResource->release(soundId);
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
void ApplicationManager::SoundPauseSounds() {
    if (this->soundResource != 0) {
        this->soundResource->pause();
    }
}
void ApplicationManager::SoundPause(int soundId) {
    if (this->soundResource != 0) {
        this->soundResource->pause(soundId);
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
bool ApplicationManager::SoundIsPlaying(int soundId) {
    if (this->soundResource == 0) {
        return false;
    }
    return this->soundResource->isPlaying(soundId);
}
void ApplicationManager::VibrateEnable(bool enable) {
    this->vibrateEnabled = enable;
}
bool ApplicationManager::VibrateSupported() {
    return this->engine->HasVibration();
}
void ApplicationManager::Vibrate(unsigned short duration) {
    if (this->vibrateEnabled) {
        this->engine->Vibrate(duration);
    }
}
void ApplicationManager::RegisterApplicationModule(unsigned int id, IApplicationModule *module) {
    if (module != 0) {
        module->SetApplicationManager(this);
        ArrayAdd(module, this->modules);
        ArrayAdd(id, this->moduleIds);
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
void ApplicationManager::SetApplicationModule(IApplicationModule *module) {
    IApplicationModule *current = this->currentModule;
    this->pendingModule = module;
    this->state = current != 0;
}
unsigned int ApplicationManager::GetCurrentApplicationModule() const {
    return this->currentModuleId;
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
void ApplicationManager::Quit() {
    QuitCallback_t *callback = this->quitCallback;
    if (callback != 0) {
        callback();
    }
}
String ApplicationManager::GetApplicationVersionString() {
    // The 2.0.16 binary returns an empty string here (literal pool -> ""), not a version number.
    return String("");
}
void ApplicationManager::SetApplicationData(void *data) { // lint: void_ptr (exported SetApplicationData param, opaque app data, baked ABI)
    // lint: void_ptr (exported SetApplicationData param, opaque app data, baked ABI)
    this->applicationData = data;
}
void *ApplicationManager::GetApplicationData() { // lint: void_ptr (exported GetApplicationData return type, baked ABI)
    // lint: void_ptr (exported GetApplicationData return type, baked ABI)
    return this->applicationData;
}
void *ApplicationManager::GetEngine() { // lint: void_ptr (exported GetEngine return type, header-declared ABI)
    // lint: void_ptr (exported GetEngine return type, header-declared ABI)
    return this->engine;
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
        this->keyState = 0;
        this->actionState = 0;
        this->state = 4;
    }
}
void ApplicationManager::SetExitCallback(QuitCallback_t *callback) {
    this->quitCallback = callback;
}
void ApplicationManager::SetLoadingCallback(LoadingCallback_t *callback, void *data) { // lint: void_ptr (opaque user data forwarded to LoadingCallback_t, baked ABI)
    // lint: void_ptr (opaque user data forwarded to LoadingCallback_t, baked ABI)
    this->loadingCallback = callback;
    this->loadingCallbackData = data;
}
void ApplicationManager::LoadingCallbackShow(int mode, void *data) { // lint: void_ptr (opaque user data forwarded to LoadingCallback_t, baked ABI)
    // lint: void_ptr (opaque user data forwarded to LoadingCallback_t, baked ABI)
    LoadingCallback_t *callback = this->loadingCallback;
    if (callback != 0) {
        callback(this->paintCanvas, mode, data);
    }
}
void ApplicationManager::SetResumeCallback(ResumeCallback_t *callback, void *data) { // lint: void_ptr (opaque user data forwarded to ResumeCallback_t, baked ABI)
    // lint: void_ptr (opaque user data forwarded to ResumeCallback_t, baked ABI)
    this->resumeCallback = callback;
    this->resumeCallbackData = data;
}
void ApplicationManager::CheatSetCallback(void (*callback)(int, void *), void *data) { // lint: void_ptr (exported CheatSetCallback signature, opaque cheat callback + data, baked ABI)
    // lint: void_ptr (exported CheatSetCallback signature, opaque cheat callback + data, baked ABI)
    if (this->cheatHandler != 0) {
        this->cheatHandler->SetCheatFunc(callback, data);
    }
}
void ApplicationManager::CheatAddCode(const String &code, int value) {
    if (this->cheatHandler != 0) {
        this->cheatHandler->AddCheatCode(code, value);
    }
}
void ApplicationManager::CheatEnable(bool enable) {
    this->cheatsEnabled = enable;
}
void ApplicationManager::CheatUpdate(unsigned short key) {
    if (this->cheatsEnabled && this->cheatHandler != 0) {
        this->cheatHandler->Update(key);
    }
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
uint64_t ApplicationManager::GetElapsedTimeMillis() {
    return this->frameTimeMs - this->previousFrameTimeMs;
}
void ApplicationManager::OnUpdate(long long now) {
    this->engine->PreUpdate();

    if (Engine::CheckForOrientationChange) {
        this->CheckForOrientationChange();
    }
    if (this->soundResource != 0 && this->soundFxEnabled) {
        this->soundResource->checkLooping();
    }

    switch (this->state) {
        case 0:
            if (AbyssEngine::SwapCounter > 0) {
                AbyssEngine::SwapCounter--;
                this->engine->ClearBuffer(0);
                this->engine->SwapBuffer();
                return;
            } else {
                IApplicationModule *next = this->pendingModule;
                if (next != 0) {
                    this->currentModule = next;
                    this->pendingModule = 0;
                    if (this->loadingCallback != 0 && next->ShowLoadingScreen()) {
                        this->loadingCallback(this->paintCanvas, -1, this->loadingCallbackData);
                    }
                }
                if (this->currentModule != 0) {
                    int loading = this->currentModule->OnInitialize();
                    if (this->loadingCallback != 0 && this->currentModule->ShowLoadingScreen()) {
                        this->loadingCallback(this->paintCanvas, loading, this->loadingCallbackData);
                    }
                    if (loading == 0) {
                        this->previousFrameTimeMs = now - 1;
                        this->state = 5;
                        this->actionState = 0;
                        this->currentTimeMs = 0;
                        this->frameTimeMs = now;
                        this->keyState = 0;
                        loadTexture = 0;
                        loadMesh = 0;
                    } else {
                        uint64_t elapsed = (uint64_t) now - this->frameTimeMs;
                        uint64_t previous = this->frameTimeMs;
                        this->currentTimeMs += elapsed;
                        this->frameTimeMs = now;
                        this->previousFrameTimeMs = previous;
                    }
                }
            }
            break;
        case 1:
            if (this->currentModule != 0) {
                this->currentModule->OnRelease();
                this->engine->ResetLightParam();
                AbyssEngine::SwapCounter = Engine::SwapCounter;
                this->currentModule = 0;
                this->state = 0;
            }
            break;
        case 4:
            this->keyState = 0;
            this->previousFrameTimeMs = now - 1;
            this->frameTimeMs = now;
            this->currentTimeMs += 1;
            this->state = this->savedState;
            this->actionState = 0;
            break;
        case 5: {
            if (performanceTest) {
                performanceFrameCounter++;
                performanceTime += this->frameTimeMs - this->previousFrameTimeMs;
            }
            if (this->currentModule == 0) {
                break;
            }
            fpsTimer += (int) (this->frameTimeMs - this->previousFrameTimeMs);
            fpsCounter++;
            if (fpsTimer > 1000) {
                currentFps = fpsCounter;
                fpsCounter = 0;
                fpsTimer -= 1000;
            }
            if (Engine::enableShader) {
                if (this->engine->IsPostEffectActivated()) {
                    PostEffectFlag = true;
                    this->paintCanvas->StartDraw2FBO();
                }
                if (Engine::enableShader) {
                    this->paintCanvas->CheckNUseRefractFBO(true);
                }
            }
            this->currentModule->OnUpdate();
            Engine *engine = this->engine;
            engine->triangleCountA = 0;
            engine->triangleCountB = 0;
            engine->field_0x58 = 0;
            engine->drawCallCountB = 0;
            engine->drawCallCountA = 0;
            this->paintCanvas->culledCount = 0;
            this->currentModule->OnRender3D();
            if (PostEffectFlag && Engine::enableShader && this->engine->IsPostEffectActivated()) {
                PostEffectFlag = false;
                this->paintCanvas->StopDraw2FBO();
            }
            if (this->resumeCallback == 0 || !this->resumeCallback(this->paintCanvas, this->resumeCallbackData)) {
                this->currentModule->OnRender2D();
            }

            if (this->engine->field_0x74) {
                this->engine->field_0xfe = 0;
                this->engine->statsBucketFlag = 0;
                this->paintCanvas->SetColor(0, 255, 0, 255);
                this->paintCanvas->DrawString(this->engine->field_0x78, String("fps: ") + String(currentFps), 5, 5,
                                              false);
                this->paintCanvas->DrawString(this->engine->field_0x78,
                                              String("t3D: ") + String((int) this->engine->field_0x58), 5,
                                              this->paintCanvas->GetTextHeight(this->engine->field_0x78) + 10, false);
                this->paintCanvas->DrawString(this->engine->field_0x78,
                                              String("t2D: ") + String((int) (this->engine->field_0x58 >> 32)), 5,
                                              this->paintCanvas->GetTextHeight(this->engine->field_0x78) * 2 + 15,
                                              false);
                this->paintCanvas->DrawString(this->engine->field_0x78,
                                              String("d3D: ") + String(this->engine->drawCallCountA), 5,
                                              this->paintCanvas->GetTextHeight(this->engine->field_0x78) * 3 + 20,
                                              false);
                this->paintCanvas->DrawString(this->engine->field_0x78,
                                              String("d2D: ") + String(this->engine->drawCallCountB), 5,
                                              this->paintCanvas->GetTextHeight(this->engine->field_0x78) * 4 + 25,
                                              false);
                this->paintCanvas->DrawString(this->engine->field_0x78,
                                              String("vfC: ") + String(this->paintCanvas->culledCount), 5,
                                              this->paintCanvas->GetTextHeight(this->engine->field_0x78) * 5 + 30,
                                              false);
                this->paintCanvas->DrawString(this->engine->field_0x78,
                                              String("T: ") + String(this->lastTouchX) + String("  ") +
                                                      String(this->lastTouchY),
                                              5, this->paintCanvas->GetTextHeight(this->engine->field_0x78) * 6 + 35,
                                              false);
                this->paintCanvas->DrawString(this->engine->field_0x78,
                                              String("vbo3D: ") + String(this->engine->triangleCountA), 5,
                                              this->paintCanvas->GetTextHeight(this->engine->field_0x78) * 7 + 35,
                                              false);
                this->paintCanvas->DrawString(this->engine->field_0x78,
                                              String("vRam: ") + String(this->engine->textureByteCounter / 1048576) +
                                                      String(" + ") + String(Engine::vboSize / 1024),
                                              5, this->paintCanvas->GetTextHeight(this->engine->field_0x78) * 8 + 35,
                                              false);
                this->paintCanvas->DrawString(this->engine->field_0x78,
                                              String("lodDist: ") + String(Engine::LodDistShader), 5,
                                              this->paintCanvas->GetTextHeight(this->engine->field_0x78) * 9 + 35,
                                              false);
                this->paintCanvas->DrawString(
                        this->engine->field_0x78, Engine::vendor,
                        (this->paintCanvas->GetWidth() -
                         this->paintCanvas->GetTextWidth(this->engine->field_0x78, Engine::vendor)) /
                                2,
                        5, false);
                this->paintCanvas->DrawString(
                        this->engine->field_0x78, Engine::renderer,
                        (this->paintCanvas->GetWidth() -
                         this->paintCanvas->GetTextWidth(this->engine->field_0x78, Engine::renderer)) /
                                2,
                        this->paintCanvas->GetTextHeight(this->engine->field_0x78) + 10, false);
                if (Engine::enableShader) {
                    this->paintCanvas->DrawString(
                            this->engine->field_0x78, String("ES2"),
                            this->paintCanvas->GetWidth() -
                                    this->paintCanvas->GetTextWidth(this->engine->field_0x78, String("ES2 ")),
                            5, false);
                    int y = this->paintCanvas->GetTextHeight(this->engine->field_0x78) + 10;
                    for (uint32_t i = 0; i < this->engine->triangleCounts.size(); ++i) {
                        if (this->engine->triangleCounts.data()[i] != 0) {
                            this->paintCanvas->DrawString(
                                    this->engine->field_0x78,
                                    String(this->engine->triangleCounts.data()[i]) + String(" ") +
                                            this->engine->shaders.data()[i]->GetShaderName(),
                                    this->paintCanvas->GetWidth() - 300, y, false);
                            y += this->paintCanvas->GetTextHeight(this->engine->field_0x78) + 5;
                        }
                    }
                }
                this->engine->statsBucketFlag = 1;
                this->engine->field_0xfe = 1;
            }

            if (performanceTestEnded) {
                this->paintCanvas->SetColor(0, 255, 0, 255);
                long long frames = performanceFrameCounter;
                long long tris = triDrawn;
                long long time = performanceTime;
                int seconds = (int) (time / 1000);
                long long trisPerSecond = tris / seconds;
                String text = String("PerfTest ended: ") + String((int) (frames * 1000 / time)) + String(" / ") +
                              String(seconds) + String(" / ") + String(trisPerSecond);
                this->paintCanvas->DrawString(this->engine->field_0x78, text,
                                              this->paintCanvas->GetWidth() / 2 -
                                                      this->paintCanvas->GetTextWidth(this->engine->field_0x78, text) / 2,
                                              5, false);
            }
            if (performanceTest) {
                triDrawn += (int) this->engine->field_0x58 + this->engine->triangleCountA;
                triDrawn += (int) (this->engine->field_0x58 >> 32) + this->engine->triangleCountB;
                this->paintCanvas->SetColor(0, 255, 0, 255);
                int seconds = (int) (performanceTime / 1000);
                String text = String("PerfTest (") + String(performanceModulSwitches) + String("/") +
                              String(performanceOverallModulSwitches) + String(") : ") +
                              String(seconds) + String("   ") + String(triDrawn);
                this->paintCanvas->DrawString(this->engine->field_0x78, text, this->paintCanvas->GetWidth() / 2, 5,
                                              false);
                text = String(this->paintCanvas->GetWidth()) + String(" / ") + String(this->paintCanvas->GetHeight());
                this->paintCanvas->DrawString(this->engine->field_0x78, text, this->paintCanvas->GetWidth() / 2,
                                              this->paintCanvas->GetTextHeight(this->engine->field_0x78) + 10, false);
            }
            uint64_t previous = this->frameTimeMs;
            this->previousFrameTimeMs = previous;
            this->frameTimeMs = now;
            this->currentTimeMs += (uint64_t) now - previous;
            break;
        }
        case 6: {
            if (this->currentTimeMs == 0 && this->paintCanvas->WarmUpTexture()) {
                this->state = 5;
                this->previousFrameTimeMs = now - 1;
                this->currentTimeMs = 0;
                this->actionState = 0;
                this->frameTimeMs = now;
                this->keyState = 0;
            }
            uint64_t previous = this->frameTimeMs;
            this->frameTimeMs = now;
            this->previousFrameTimeMs = previous;
            long long elapsed = this->currentTimeMs + ((uint64_t) now - previous);
            if (elapsed > 500) {
                elapsed = 0;
            }
            this->currentTimeMs = elapsed;
            break;
        }
        case 7: {
            if ((uint32_t) loadTexture < this->paintCanvas->cubeTextures.size() ||
                (uint32_t) loadMesh < this->paintCanvas->meshes.size()) {
                if (loadTexture == 0 && this->loadingCallback != 0 && this->currentModule->ShowLoadingScreen()) {
                    this->loadingCallback(this->paintCanvas, 0, this->loadingCallbackData);
                }
                if ((uint32_t) loadTexture < this->paintCanvas->cubeTextures.size()) {
                    AELoadedTexture *texture = this->paintCanvas->cubeTextures.data()[loadTexture];
                    if (texture != 0 && !texture->valid) {
                        char *path = texture->name.GetAEChar();
                        unsigned int id;
                        TextureCreateFromFileIntern(this->engine, path, 0, 0, &id, texture->scale, texture, true);
                        delete[] path;
                    }
                    loadTexture++;
                }
                for (int i = 0; i < 10; ++i) {
                    if ((uint32_t) loadMesh < this->paintCanvas->meshes.size()) {
                        this->paintCanvas->MeshConvertToVBO(loadMesh);
                        loadMesh++;
                    }
                }
            } else {
                this->previousFrameTimeMs = now - 1;
                this->frameTimeMs = now;
                this->currentTimeMs = 0;
                this->keyState = 0;
                this->actionState = 0;
                this->state = 5;
            }
            uint64_t previous = this->frameTimeMs;
            this->previousFrameTimeMs = previous;
            this->frameTimeMs = now;
            long long elapsed = this->currentTimeMs + ((uint64_t) now - previous);
            if (elapsed > 500) {
                elapsed = 0;
            }
            this->currentTimeMs = elapsed;
            break;
        }
        default:
            break;
    }
    this->engine->SwapBuffer();
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
            case 2:
                if (x > this->paintCanvas->GetWidth() - 50 && y > this->paintCanvas->GetHeight() - 50) {
                    AbyssEngine::debugModusIndex++;
                    return;
                }
                break;
            case 3:
                if (x <= 49 && y > this->paintCanvas->GetHeight() - 50) {
                    this->engine->field_0x74 = !this->engine->field_0x74;
                    return;
                }
                break;
            case 1: {
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
                    if (x < half) {
                        Engine::LodDistShader = 0.0f;
                    } else {
                        Engine::LodDistShader = 10000000.0f;
                    }
                } else if (x < half) {
                    Engine::LodDistShader += -100.0f;
                } else {
                    Engine::LodDistShader += 100.0f;
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
    this->keyState = 0;
    this->currentKey = 0;
    this->actionMask = 0;
    this->actionState = 0;
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
uint64_t ApplicationManager::GetKeyState() {
    return this->keyState;
}
uint64_t ApplicationManager::GetActionState() {
    return this->actionState;
}
void ApplicationManager::ResetKeyState() {
    this->keyState = 0;
}
uint64_t ApplicationManager::GetCurrentTimeMillis() {
    return this->currentTimeMs;
}
uint64_t ApplicationManager::GetSystemTimeMillis() {
    return this->currentTimeMs;
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
bool ApplicationManager::CheckCrack(const char *path) {
    (void) path;
    return false;
}