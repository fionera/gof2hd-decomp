#ifndef GOF2_FMODSOUND_H
#define GOF2_FMODSOUND_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field naming)
// Galaxy on Fire 2 -- FModSound (Android libgof2hdaa.so, armv7 Thumb).
// FMOD wrapper. FMOD_* funcs are modeled as extern "C". Soft-float ABI: float args in core regs.
#include <new>

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

// FMOD designer-API handle types (opaque wrappers, manipulated through the FMOD_* shims).
namespace FMOD {
struct Event;
struct EventParameter;
struct EventProject;
struct EventGroup;
struct EventCategory;
struct EventSystem;
}

// 3D math vector used for event positions/velocities.
using AbyssEngine::AEMath::Vector;

// Layout offsets ---------------------------------------------------------------
static const uint32_t OFF_EVENTS    = 0x18;    // event handle array (idx*4)
static const uint32_t OFF_ENABLED   = 0x11;    // per-category enabled bytes (0x11..0x14)
static const uint32_t OFF_CATEGORY  = 0x23ec;  // 4 EventCategory* (idx*4)
static const uint32_t OFF_SYSTEM    = 0x23fc;  // EventSystem*
static const uint32_t OFF_MUSIC     = 0x2400;  // music Event*
static const uint32_t OFF_FLAG2404  = 0x2404;

// Field accessors --------------------------------------------------------------

// FModSound — FMOD designer-API sound manager. Named fields live at recovered byte offsets;
// the large event-handle array (0x18..0x23ec) and category/system slots are reached via the
// i32/u32/u8/pp byte-offset helpers above.
struct FModSound {
    int     f_0;                 // 0x00 current music event id
    int     f_4;                 // 0x04
    int     f_8;                 // 0x08
    void*   f_c;                 // 0x0c name / path string
    uint8_t f_10;                // 0x10 low-memory flag
    char    _pad_0x11[0x2400 - 0x11];
    void*   f_2400;              // 0x2400 music Event*
    int     f_2404;              // 0x2404
    int     f_2408;              // 0x2408
    int     f_240c;              // 0x240c
    char    _pad_0x2410[0x2424 - 0x2410];
    void*   f_2424;              // 0x2424 cached position vector
    void*   f_2428;              // 0x2428 cached velocity vector
    void*   f_242c;              // 0x242c cached forward vector
    void*   f_2430;              // 0x2430 cached up vector
    void*   f_2434;              // 0x2434 cached position vector (event 3D)
    void*   f_2438;              // 0x2438 cached velocity vector (event 3D)

    FModSound();
    ~FModSound();

    void setAudioLanguage(int p1);
    void updateEvent3DAttributes(int idx, Vector *a, Vector *b, bool c);
    void *updateEvent3DAttributes(void *event, int idx, Vector *pos, Vector *vel, bool restart);
    void stopAll();
    void resumeAll();
    void promptMusicCue(int p1);
    void fadeOutNow();
    void release();
    FModSound *stop(void *e);
    void stop(int p1);
    void setVolume(int p1, float vol);
    void pauseAll();
    void setDownPitch(bool down);
    void setMusicParamValue(int p1, float p2);
    void setSoundVolume(int p1, float vol);
    void pauseAllPlaying();
    void resume(int p1);
    bool resume(void *e);
    int getEventPauseLength(int idx);
    void enableReverb(int p1);
    float getPlayingProgress(int idx);
    void play(int idx, Vector *pos, Vector *vel, float pitch);
    void setParamValue(FMOD::Event *e, int paramIdx, float val);
    void setParamValue(int paramIdx, int idx, float val);
    void setParamValue(const char *name, int idx, float val);
    void stopAllSoundFXEvents();
    int pause(int p1);
    int pause(void *e);
    void disableReverb();
    int init();
    unsigned isChannelActive(int p1);
    void playMusicFadeOutCurrent(int p1);
    void getParam(const char *name, int idx);
    void updateAll(Vector *pos, Vector *vel, Vector *forward, Vector *up);
    unsigned isPlaying(int p1);
    void freeAllEvents();
    uint8_t IsCategoryEnabled(int p1);
    void pauseAllPlayingSoundFXEvents();
    void enableCategory(int p1, bool enable);
};
#endif
