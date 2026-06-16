#ifndef GOF2_APPLICATIONMANAGER_H
#define GOF2_APPLICATIONMANAGER_H
#include "common.h"   // Array, String, fixed-width integer types
#include <new>             // placement new (key-mapping / config-string construction)

// Cross-class types referenced only through pointers.
class PaintCanvas;   // global-scope render canvas (game/core/PaintCanvasClass.h)
class Engine;        // global-scope renderer/device root (engine/render/Engine.h)
namespace AbyssEngine { class ConfigReader; }   // engine/file/ConfigReader.h

using String = AbyssEngine::String;

// Application-level callbacks, recovered from the SetLoadingCallback / SetResumeCallback
// signatures. Invoked through the PaintCanvas the manager owns at offset 0.
typedef void LoadingCallback_t(PaintCanvas *canvas, int loading, void *data);
typedef bool ResumeCallback_t(PaintCanvas *canvas, void *data);

// Top-level driver for the running game: owns the PaintCanvas, the active application
// module, input/key state, the audio resource and the config reader, and steps the
// per-frame state machine in OnUpdate.
class ApplicationManager {
public:
    PaintCanvas *paintCanvas;             // owned render canvas (object starts here)
    uint32_t     currentKey;
    uint32_t     currentKeyHigh;
    char        *keyMappingTable;         // key-code -> (mask, String) mapping table
    bool         orientationTrackingEnabled;
    void        *currentModule;           // active IApplicationModule (polymorphic)
    void        *quitCallback;            // QuitCallback*
    void        *loadingCallback;         // LoadingCallback_t*
    void        *loadingCallbackData;
    void        *resumeCallback;          // ResumeCallback_t*
    void        *resumeCallbackData;
    void        *cheatHandler;            // AbyssEngine::CheatHandler*
    bool         cheatsEnabled;
    void        *configReader;            // AbyssEngine::ConfigReader*
    int          field_0x3c;              // frame state-machine state
    int          savedState;             // state saved across suspend/resume
    Array<void*>        *modules;         // loaded application modules (IApplicationModule*)
    Array<unsigned int> *moduleIds;       // module-id table (parallel to `modules`)
    unsigned int currentModuleId;
    void        *pendingModule;           // module to switch to next frame
    uint64_t     currentTimeMs;
    uint64_t     frameTimeMs;
    uint64_t     previousFrameTimeMs;
    uint32_t     keyState;
    uint32_t     keyStateHigh;
    Array<long long>    *actionTable;     // action table: pairs of (action, key) longs
    uint32_t     actionMask;
    uint32_t     actionMaskHigh;
    uint32_t     actionState;
    uint32_t     actionStateHigh;
    void        *engine;                  // Engine*
    void        *soundResource;           // AbyssEngine::AESoundRessource*
    bool         soundFxEnabled;
    bool         musicEnabled;
    bool         vibrateEnabled;
    int          lastTouchX;
    int          lastTouchY;

    explicit ApplicationManager(void *engine);
    ~ApplicationManager();

    void CheatAddCode(void *code, int value);
    void CheatEnable(bool enable);
    void CheatSetCallback(void *callback, void *data);
    void CheatUpdate(unsigned short key);
    void CheckForOrientationChange();
    void *ConfigGetKeysForAction(long long action);
    void ConfigRegisterAction(long long value, long long key);
    void ConvertTouchCoords(int *x, int *y);
    uint64_t GetActionState();
    void *GetApplicationModule(unsigned int id);
    uint64_t GetCurrentTimeMillis();
    uint64_t GetElapsedTimeMillis();
    void *GetEngine();
    uint64_t GetKeyState();
    uint64_t GetSystemTimeMillis();
    void KeyCodeSetMapping(void *array);
    void LoadingCallbackShow(int mode, void *data);
    void OnKeyPress(int key);
    void OnKeyRelease(int key);
    void OnTouchBegin(int xArg, int yArg, void *touch);
    void OnTouchEnd(int xArg, int yArg, void *touch);
    void OnTouchEndSimple();
    void OnTouchMove(int xArg, int yArg, void *touch);
    void OnUpdate(long long now);
    void Quit();
    void ResetKeyState();
    void Resume();
    void SetApplicationModule(void *module);
    void SetCurrentApplicationModule(unsigned int id);
    void SetLoadingCallback(LoadingCallback_t *callback, void *data);
    void SetResumeCallback(ResumeCallback_t *callback, void *data);
    void SoundEnable(bool enable);
    void SoundFxEnable(bool enable);
    int SoundIsPlaying();
    void SoundMusicEnable(bool enable);
    void SoundPause();
    void SoundPauseSounds();
    void SoundPlay(int soundId);
    void SoundPlayLoop(int soundId);
    void SoundPlayMusic(int soundId);
    void SoundPlayMusicLoop(int soundId);
    void SoundPlay_vol(int soundId, float volume);
    void SoundRelease();
    void SoundResume(int soundId);
    void SoundResumeSelf();
    void SoundResumeSounds();
    void SoundSetFXVolume(int volume);
    void SoundSetMusicVolume(int volume);
    void SoundSetVolume(int volume);
    void SoundStop();
    void SoundStopSounds();
    void Suspend();
    void Vibrate(unsigned short duration);
    void VibrateEnable(bool enable);
    void VibrateSupported();
};

extern ApplicationManager* gAppManager;  // canonical ApplicationManager singleton (binary .bss 0x2281d8)

#endif
