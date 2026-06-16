#ifndef GOF2_APPLICATIONMANAGER_H
#define GOF2_APPLICATIONMANAGER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include <new>             // placement new (key-mapping / config-string construction)

// Cross-class types referenced only through pointers.
class PaintCanvas;   // global-scope render canvas (game/core/PaintCanvasClass.h)
class Engine;        // global-scope renderer/device root (engine/render/Engine.h)
namespace AbyssEngine {
    class ConfigReader;       // engine/file/ConfigReader.h
    class CheatHandler;       // game/core/CheatHandler.h
    class AESoundRessource;   // engine/audio/AESoundRessource.h
}

using String = AbyssEngine::String;

// Application-level callbacks, recovered from the SetLoadingCallback / SetResumeCallback
// signatures. Invoked through the PaintCanvas the manager owns at offset 0.
typedef void LoadingCallback_t(PaintCanvas *canvas, int loading, void *data);
typedef bool ResumeCallback_t(PaintCanvas *canvas, void *data);
typedef void QuitCallback_t();

// Top-level driver for the running game: owns the PaintCanvas, the active application
// module, input/key state, the audio resource and the config reader, and steps the
// per-frame state machine in OnUpdate.
class ApplicationManager {
public:
    PaintCanvas *paintCanvas;             // +0x00 owned render canvas (object starts here)
    uint32_t     currentKey;              // +0x08
    uint32_t     currentKeyHigh;          // +0x0c
    char        *keyMappingTable;         // +0x10 key-code -> (mask, String) mapping table
    bool         orientationTrackingEnabled; // +0x14
    void        *currentModule;           // +0x18 active IApplicationModule (raw-vtable dispatch)
    QuitCallback_t    *quitCallback;      // +0x1c
    LoadingCallback_t *loadingCallback;   // +0x20
    void        *loadingCallbackData;     // +0x24
    ResumeCallback_t  *resumeCallback;    // +0x28
    void        *resumeCallbackData;      // +0x2c
    AbyssEngine::CheatHandler *cheatHandler;   // +0x30
    bool         cheatsEnabled;           // +0x34
    AbyssEngine::ConfigReader *configReader;   // +0x38
    int          state;                   // +0x3c frame state-machine state
    int          savedState;              // +0x40 state saved across suspend/resume
    Array<void*>        *modules;         // +0x48 loaded application modules (IApplicationModule*)
    Array<unsigned int> *moduleIds;       // +0x50 module-id table (parallel to `modules`)
    unsigned int currentModuleId;         // +0x5c
    void        *pendingModule;           // +0x60 module to switch to next frame (raw-vtable dispatch)
    uint64_t     currentTimeMs;           // +0x68
    uint64_t     frameTimeMs;             // +0x70
    uint64_t     previousFrameTimeMs;     // +0x78
    uint32_t     keyState;                // +0x80
    uint32_t     keyStateHigh;            // +0x84
    Array<long long>    *actionTable;     // +0x8c action table: pairs of (action, key) longs
    uint32_t     actionMask;              // +0x98
    uint32_t     actionMaskHigh;          // +0x9c
    uint32_t     actionState;             // +0xa0
    uint32_t     actionStateHigh;         // +0xa4
    Engine      *engine;                  // +0xa8
    AbyssEngine::AESoundRessource *soundResource; // +0xac
    bool         soundFxEnabled;          // +0xb0
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
