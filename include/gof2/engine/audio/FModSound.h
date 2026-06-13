#ifndef GOF2_FMODSOUND_H
#define GOF2_FMODSOUND_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field naming)
// Galaxy on Fire 2 -- FModSound (Android libgof2hdaa.so, armv7 Thumb).
// FMOD wrapper. FMOD_* funcs are modeled as extern "C". Soft-float ABI: float args in core regs.
#include <new>

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

// FModSound -- FMOD designer-API sound manager.
//
// The on-disk (32-bit ARM) layout packs three indexed regions as 4-byte-per-slot
// arrays: the per-category enabled bytes (0x11), the big event-handle array (0x18),
// and the four EventCategory* slots (0x23ec). They are modelled as fixed C arrays so
// the original idx-based accesses stay natural while every member is named & typed.
// Offset comments below are the recovered 32-bit byte offsets.
class FModSound {
public:
    int       currentMusicEvent;      // +0x00 current music event id
    int       fadeTargetMusicEvent;   // +0x04 music event id being faded to
    int       downPitch;              // +0x08 down-pitch flag
    char     *appRootDir;             // +0x0c app root dir / .fev base path
    uint8_t   lowMemory;              // +0x10 low-memory flag (selects _low.fev)
    uint8_t   categoryEnabled[4];     // +0x11 [0]=master, [1..3]=per-category enabled
    void     *events[0x8f5];          // +0x18 event handle array (slot per system id)
    void     *category[4];            // +0x23ec EventCategory* per category
    void     *system;                 // +0x23fc FMOD::EventSystem*
    void     *music;                  // +0x2400 music Event*
    int       initialized;            // +0x2404 init/ready flag
    int       reverbPreset;           // +0x2408 current reverb preset index
    int       propSlot;               // +0x240c armed property slot id
    int       fxSlots[5];             // +0x2410 active sound-fx slot indices (-1 = free)
    Vector   *listenerPos;            // +0x2424 cached listener position
    Vector   *listenerVel;            // +0x2428 cached listener velocity
    Vector   *listenerForward;        // +0x242c cached listener forward
    Vector   *listenerUp;             // +0x2430 cached listener up
    Vector   *eventPos;               // +0x2434 cached event 3D position
    Vector   *eventVel;               // +0x2438 cached event 3D velocity

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

    // Hook used when entering a context that wants its own background music; the
    // shipped build is a no-op that always reports "nothing stopped".
    int tryToStopMusicForBGMusic();

    // Looping engine-sound transport for a ship. The argument is the owning Player;
    // the live FMOD event handle lives on it at +0xf0 (Player::engineEvent).
    static void pauseEvent(void *player);
    static void resumeEvent(void *player, int immediate);
    static void stopEvent(void *player);
    static void playEvent(void *player, int eventId, int mode);

    // Arm a property slot for the next event start (cinematic launch sound) and
    // return the pitch to start it with.
    static float setProp(int snd, int id);

    // Resume every live event after a scene that had suspended audio.
    static void restoreState();

    // Process-wide instance the legacy code reached through a fixed global slot.
    static FModSound *g_instance;
    static FModSound *active();

    // Base pitch applied to freshly started events (0.0 = use authored pitch).
    static float defaultPitch;
};
#endif
