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

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

struct ConfigReader;
struct PaintCanvas;
using String = AbyssEngine::String;

// Callback function types (used only via pointer below; PaintCanvas fwd-declared above).
// Signatures recovered from mangled symbols (unified_symbols.tsv) and src/ApplicationManager.cpp:
//   SetLoadingCallback(void (*)(PaintCanvas*, int, void*), void*)
//   SetResumeCallback (bool (*)(PaintCanvas*, void*),       void*)
typedef void LoadingCallback_t(PaintCanvas *, int, void *);
typedef bool ResumeCallback_t(PaintCanvas *, void *);

struct ApplicationManager {
    void        *field_0x0;             // +0x0  PaintCanvas* (object starts with the canvas ptr)
    uint32_t     field_0x8;             // +0x8
    uint32_t     field_0xc;             // +0xc
    char        *field_0x10;            // +0x10 key mapping table
    bool         field_0x14;            // +0x14 orientation-tracking flag
    void        *field_0x18;            // +0x18 current application module
    void        *field_0x1c;            // +0x1c QuitCallback*
    void        *field_0x20;            // +0x20 LoadingCallback*
    void        *field_0x24;            // +0x24 loading-callback data
    void        *field_0x28;            // +0x28 ResumeCallback*
    void        *field_0x2c;            // +0x2c resume-callback data
    void        *field_0x30;            // +0x30 CheatHandler*
    bool         field_0x34;            // +0x34 cheats-enabled flag
    void        *field_0x38;            // +0x38 ConfigReader*
    int          field_0x3c;            // +0x3c state machine state
    int          field_0x40;            // +0x40 saved state
    char        *field_0x48;            // +0x48 modules data ptr
    unsigned int field_0x50;            // +0x50 module-id count
    char        *field_0x54;            // +0x54 module-id data ptr
    unsigned int field_0x5c;            // +0x5c current module id
    void        *field_0x60;            // +0x60 pending application module
    uint64_t     field_0x68;            // +0x68 current time (ms)
    uint64_t     field_0x70;            // +0x70 frame time (ms)
    uint64_t     field_0x78;            // +0x78 previous frame time (ms)
    uint32_t     field_0x80;            // +0x80 key state (low word)
    uint32_t     field_0x84;            // +0x84 key state (high word)
    unsigned int field_0x88;            // +0x88 action table count
    char        *field_0x8c;            // +0x8c action table data ptr
    uint32_t     field_0x98;            // +0x98 action mask (low word)
    uint32_t     field_0x9c;            // +0x9c action mask (high word)
    uint32_t     field_0xa0;            // +0xa0 action state (low word)
    uint32_t     field_0xa4;            // +0xa4 action state (high word)
    void        *field_0xa8;            // +0xa8 Engine*
    void        *field_0xac;            // +0xac AESoundRessource*
    bool         field_0xb0;            // +0xb0 sound-fx enabled
    bool         field_0xb1;            // +0xb1 music enabled
    bool         field_0xb2;            // +0xb2 vibrate enabled
    int          field_0xb4;            // +0xb4 last touch x
    int          field_0xb8;            // +0xb8 last touch y

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
    ApplicationManager * ctor(void *engine);
};

#endif
