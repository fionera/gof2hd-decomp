#ifndef GOF2_APPLICATIONMANAGER_H
#define GOF2_APPLICATIONMANAGER_H
#include "AEString.h"
#include "engine/render/Engine.h"
#include "engine/core/IApplicationModule.h"
#include "engine/core/KeyCode.h"
#include "engine/file/ConfigReader.h"
#include <new>
#include "engine/audio/AESoundRessource.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/CheatHandler.h"

namespace AbyssEngine {
    class AESoundRessource;
    class CheatHandler;
    class ConfigReader;
    class IApplicationModule;
    class KeyCode;
    struct AESoundInfo;
}

namespace AbyssEngine {
    class PaintCanvas;
    class Engine;
}

using ::AbyssEngine::PaintCanvas;
using ::AbyssEngine::Engine;

typedef void LoadingCallback_t(PaintCanvas *canvas, int loading, void *data); // lint: void_ptr (callback typedef, opaque user data; original exports with Pv mangling)

// lint: void_ptr (callback typedef, opaque user data; original exports with Pv mangling)

typedef bool ResumeCallback_t(PaintCanvas *canvas, void *data); // lint: void_ptr (callback typedef, opaque user data; original exports with Pv mangling)

// lint: void_ptr (callback typedef, opaque user data; original exports with Pv mangling)

typedef void QuitCallback_t();

namespace AbyssEngine {
    class ApplicationManager {
    public:
        PaintCanvas *paintCanvas;
        // Ghidra ground truth: a 4-byte field sits at 0x4 between paintCanvas and currentKey@0x8;
        // our decomp lacked it, shifting currentKey..savedState 4 bytes low.
        uint32_t field_0x4;
        uint32_t currentKey;
        uint32_t currentKeyHigh;
        char *keyMappingTable;
        bool orientationTrackingEnabled;
        IApplicationModule *currentModule;
        QuitCallback_t *quitCallback;
        LoadingCallback_t *loadingCallback;
        void *loadingCallbackData; // lint: void_ptr (opaque user data forwarded to LoadingCallback_t)
        ResumeCallback_t *resumeCallback;
        void *resumeCallbackData; // lint: void_ptr (opaque user data forwarded to ResumeCallback_t)
        CheatHandler *cheatHandler;
        bool cheatsEnabled;
        ConfigReader *configReader;
        int state;
        int savedState;
        // Ghidra ground truth: these are INLINE Array members (count+data_+capacity_ = 12 bytes each at
        // 0x44/0x50), not heap pointers. Our decomp modeled them as Array<T>* (4 bytes), losing 16 bytes
        // and shifting state/savedState/currentModuleId/... below.
        Array<IApplicationModule *> modules;
        Array<unsigned int> moduleIds;
        unsigned int currentModuleId;
        IApplicationModule *pendingModule;
        void *applicationData; // lint: void_ptr (opaque app data, GetApplicationData/SetApplicationData ABI)
        uint64_t currentTimeMs;
        uint64_t frameTimeMs;
        uint64_t previousFrameTimeMs;
        uint32_t keyState;
        uint32_t keyStateHigh;
        Array<long long> actionTable; // inline Array (see modules/moduleIds above)
        // Ghidra: a 4-byte field sits between actionTable (ends 0x94) and actionMask@0x98; ours lacked it.
        uint32_t field_0x94;
        uint32_t actionMask;
        uint32_t actionMaskHigh;
        uint32_t actionState;
        uint32_t actionStateHigh;
        Engine *engine;
        AESoundRessource *soundResource;
        bool soundFxEnabled;
        bool musicEnabled;
        bool vibrateEnabled;
        int lastTouchX;
        int lastTouchY;

        explicit ApplicationManager(AbyssEngine::Engine *engine);

        ~ApplicationManager();

        bool CheckCrack(const char *path);

        void CheatAddCode(const String &code, int value);

        void CheatEnable(bool enable);

        void CheatSetCallback(void (*callback)(int, void *), void *data); // lint: void_ptr (exported ABI signature, opaque cheat callback user data)

        // lint: void_ptr (exported ABI signature, opaque cheat callback user data)

        void CheatUpdate(unsigned short key);

        void CheckForOrientationChange();

        void *ConfigGetKeysForAction(long long action); // lint: void_ptr (exported ABI signature, returns Pv)

        void ConfigReadFile(String name);

        void ConfigRegisterAction(long long value, long long key);

        void ConfigRegisterTokenReadFunction(String name,
                                             ConfigTokenReadFunction read,
                                             void *context); // lint: void_ptr (exported ABI signature, opaque token-read context)

        // lint: void_ptr (exported ABI signature, opaque token-read context)

        void ConvertTouchCoords(int &x, int &y);

        void EnablePerformanceTest(int count);

        uint64_t GetActionState();

        void *GetApplicationData(); // lint: void_ptr (exported ABI signature, returns Pv)

        void *GetApplicationModule(unsigned int id); // lint: void_ptr (exported ABI signature, returns Pv)

        String GetApplicationVersionString();

        void *GetCurrentApplicationModule() const; // lint: void_ptr (exported ABI signature, returns Pv)

        uint64_t GetCurrentTimeMillis();

        uint64_t GetElapsedTimeMillis();

        void *GetEngine(); // lint: void_ptr (exported ABI signature, returns Pv)

        uint64_t GetKeyState();

        uint64_t GetSystemTimeMillis();

        void KeyCodeSetMapping(Array<KeyCode *> *array);

        void LoadingCallbackShow(int mode, void *data); // lint: void_ptr (exported ABI signature, opaque loading-callback user data)

        // lint: void_ptr (exported ABI signature, opaque loading-callback user data)

        void OnKeyPress(int key);

        void OnKeyRelease(int key);

        void OnTouchBegin(int xArg, int yArg, void *touch); // lint: void_ptr (exported ABI signature, opaque platform touch handle)

        // lint: void_ptr (exported ABI signature, opaque platform touch handle)

        void OnTouchEnd(int xArg, int yArg, void *touch); // lint: void_ptr (exported ABI signature, opaque platform touch handle)

        // lint: void_ptr (exported ABI signature, opaque platform touch handle)

        void OnTouchEnd();

        void OnTouchMove(int xArg, int yArg, void *touch); // lint: void_ptr (exported ABI signature, opaque platform touch handle)

        // lint: void_ptr (exported ABI signature, opaque platform touch handle)

        void OnUpdate(long long now);

        void Quit();

        void RegisterApplicationModule(unsigned int id, IApplicationModule *module);

        void ResetKeyState();

        void Resume(bool arg);

        void SetApplicationData(void *data); // lint: void_ptr (exported ABI signature, opaque app data)

        void SetApplicationModule(IApplicationModule *module);

        void SetCurrentApplicationModule(unsigned int id);

        void SetExitCallback(QuitCallback_t *callback);

        void SetLoadingCallback(LoadingCallback_t *callback, void *data); // lint: void_ptr (exported ABI signature, opaque loading-callback user data)

        // lint: void_ptr (exported ABI signature, opaque loading-callback user data)

        void SetResumeCallback(ResumeCallback_t *callback, void *data); // lint: void_ptr (exported ABI signature, opaque loading-callback user data)

        // lint: void_ptr (exported ABI signature, opaque resume-callback user data)

        void SoundEnable(bool enable);

        void SoundFxEnable(bool enable);

        int SoundIsPlaying(int soundId);

        void SoundMusicEnable(bool enable);

        void SoundPause(int soundId);

        void SoundPauseSounds();

        void SoundPlay(int soundId);

        void SoundPlay(int soundId, float volume);

        void SoundPlayLoop(int soundId);

        void SoundPlayMusic(int soundId);

        void SoundPlayMusicLoop(int soundId);

        void SoundRelease(int soundId);

        void SoundResume();

        void SoundResume(int soundId);

        void SoundResumeSounds();

        void SoundSet(const AESoundInfo *info, int count);

        void SoundSetFXVolume(int volume);

        void SoundSetMusicVolume(int volume);

        void SoundSetVolume(int soundId, int volume);

        void SoundStop(int soundId);

        void SoundStopSounds();

        void Suspend();

        void Vibrate(unsigned short duration);

        void VibrateEnable(bool enable);

        void VibrateSupported();
    };
}

using ::AbyssEngine::ApplicationManager;

Engine *GetEngine();

#endif
