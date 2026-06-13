#ifndef GOF2_APPLICATIONMANAGER_H
#define GOF2_APPLICATIONMANAGER_H
#include "gof2/common.h"
// Galaxy on Fire 2 - ApplicationManager.
// Top-level class (matches the global forward decls in fwd.h / Engine.h). Real named
// struct (field_0xNN member names kept for cross-class refs). The recovered bodies still
// do some raw byte arithmetic over inline sub-arrays (modules at +0x44, the uint id table
// at +0x50, the action table at +0x88) and treat offset 0 as the PaintCanvas pointer;
// those raw accesses compile against char*/void* casts.
#include <new>   // placement operator new

struct ConfigReader;
struct PaintCanvas;
using String = AbyssEngine::String;

// Callback function types (used only via pointer below; PaintCanvas fwd-declared above).
// Signatures recovered from mangled symbols (unified_symbols.tsv) and src/ApplicationManager.cpp:
//   SetLoadingCallback(void (*)(PaintCanvas*, int, void*), void*)
//   SetResumeCallback (bool (*)(PaintCanvas*, void*),       void*)
typedef void LoadingCallback_t(PaintCanvas *, int, void *);
typedef bool ResumeCallback_t(PaintCanvas *, void *);

class ApplicationManager {
public:
    void        *paintCanvas;             // +0x0  PaintCanvas* (object starts with the canvas ptr)
    uint32_t     currentKey;             // +0x8
    uint32_t     currentKeyHigh;             // +0xc
    char        *keyMappingTable;            // +0x10 key mapping table
    bool         orientationTrackingEnabled;            // +0x14 orientation-tracking flag
    void        *currentModule;            // +0x18 current application module
    void        *quitCallback;            // +0x1c QuitCallback*
    void        *loadingCallback;            // +0x20 LoadingCallback*
    void        *loadingCallbackData;            // +0x24 loading-callback data
    void        *resumeCallback;            // +0x28 ResumeCallback*
    void        *resumeCallbackData;            // +0x2c resume-callback data
    void        *cheatHandler;            // +0x30 CheatHandler*
    bool         cheatsEnabled;            // +0x34 cheats-enabled flag
    void        *configReader;            // +0x38 ConfigReader*
    int          field_0x3c;            // +0x3c state machine state
    int          savedState;            // +0x40 saved state
    Array<void*>        *modules;            // +0x44 loaded application modules (IApplicationModule*)
    Array<unsigned int> *moduleIds;            // +0x50 module-id table (parallel to `modules`)
    unsigned int currentModuleId;            // +0x5c current module id
    void        *pendingModule;            // +0x60 pending application module
    uint64_t     currentTimeMs;            // +0x68 current time (ms)
    uint64_t     frameTimeMs;            // +0x70 frame time (ms)
    uint64_t     previousFrameTimeMs;            // +0x78 previous frame time (ms)
    uint32_t     keyState;            // +0x80 key state (low word)
    uint32_t     keyStateHigh;            // +0x84 key state (high word)
    Array<long long>    *actionTable;            // +0x88 action table (pairs of long long: value, key)
    uint32_t     actionMask;            // +0x98 action mask (low word)
    uint32_t     actionMaskHigh;            // +0x9c action mask (high word)
    uint32_t     actionState;            // +0xa0 action state (low word)
    uint32_t     actionStateHigh;            // +0xa4 action state (high word)
    void        *engine;            // +0xa8 Engine*
    void        *soundResource;            // +0xac AESoundRessource*
    bool         soundFxEnabled;            // +0xb0 sound-fx enabled
    bool         musicEnabled;            // +0xb1 music enabled
    bool         vibrateEnabled;            // +0xb2 vibrate enabled
    int          lastTouchX;            // +0xb4 last touch x
    int          lastTouchY;            // +0xb8 last touch y

    ~ApplicationManager();

    // ---- methods (converted from free functions) ----
    void CheatAddCode(void *code, int value);
    void CheatEnable(bool enable);
    void CheatSetCallback(void *callback, void *data);
    void CheatUpdate(unsigned short key);
    void CheckForOrientationChange();
    void * ConfigGetKeysForAction(long long action);
    void ConfigRegisterAction(long long value, long long key);
    void ConvertTouchCoords(int *x, int *y);
    uint64_t GetActionState();
    void * GetApplicationModule(unsigned int id);
    uint64_t GetCurrentTimeMillis();
    uint64_t GetElapsedTimeMillis();
    void * GetEngine();
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
    ApplicationManager(void *engine);
};

#endif
