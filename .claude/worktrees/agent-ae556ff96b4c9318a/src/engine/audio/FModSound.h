#ifndef GOF2_FMODSOUND_H
#define GOF2_FMODSOUND_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// FMOD designer-API handle types (opaque wrappers manipulated through the FMOD_* shims).
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

// FMOD result/error code returned by every FMOD_* runtime call.
enum FMOD_RESULT {
    FMOD_OK = 0
};

// FModSound -- FMOD designer-API sound manager.
//
// Three indexed regions are modelled as fixed C arrays: the per-category enabled
// flags, the big event-handle array (one slot per system id), and the four
// EventCategory* slots.
class FModSound {
public:
    int      currentMusicEvent;    // current music event id
    int      fadeTargetMusicEvent; // music event id being faded to
    int      downPitch;            // down-pitch flag
    char    *appRootDir;           // app root dir / .fev base path
    uint8_t  lowMemory;            // low-memory flag (selects _low.fev)
    uint8_t  categoryEnabled[4];   // [0]=master, [1..3]=per-category enabled
    FMOD::Event *events[0x8f5];    // event handle array (slot per system id)
    void    *category[4];          // EventCategory* per category
    void    *system;               // FMOD::EventSystem*
    void    *music;                // music Event*
    int      initialized;          // init/ready flag
    int      reverbPreset;         // current reverb preset index
    int      propSlot;             // armed property slot id
    int      fxSlots[5];           // active sound-fx slot indices (-1 = free)
    Vector  *listenerPos;          // cached listener position
    Vector  *listenerVel;          // cached listener velocity
    Vector  *listenerForward;      // cached listener forward
    Vector  *listenerUp;           // cached listener up
    Vector  *eventPos;             // cached event 3D position
    Vector  *eventVel;             // cached event 3D velocity

    FModSound();
    ~FModSound();

    void setAudioLanguage(int p1);
    void updateEvent3DAttributes(int idx, Vector *a, Vector *b, bool c);
    FMOD::Event *updateEvent3DAttributes(FMOD::Event *event, int idx, Vector *pos, Vector *vel, bool restart);
    void stopAll();
    void resumeAll();
    void promptMusicCue(int p1);
    void fadeOutNow();
    void release();
    FModSound *stop(FMOD::Event *e);
    void stop(int p1);
    void setVolume(int p1, float vol);
    void pauseAll();
    void setDownPitch(bool down);
    void setMusicParamValue(int p1, float p2);
    void setSoundVolume(int p1, float vol);
    void pauseAllPlaying();
    void resume(int p1);
    bool resume(FMOD::Event *e);
    int getEventPauseLength(int idx);
    void enableReverb(int p1);
    float getPlayingProgress(int idx);
    void play(int idx, Vector *pos, Vector *vel, float pitch);
    void setParamValue(FMOD::Event *e, int paramIdx, float val);
    void setParamValue(int paramIdx, int idx, float val);
    void setParamValue(const char *name, int idx, float val);
    void stopAllSoundFXEvents();
    int pause(int p1);
    int pause(FMOD::Event *e);
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

    // Reports whether wired headphones are connected. The shipped build has no
    // headset-detection backend, so it always reports "not plugged in".
    bool isHeadsetPluggedIn();

    // Central error handler for FMOD return codes. The shipped build swallows the
    // result and does nothing (release builds drop the diagnostic logging).
    void ERRCHECK(FMOD_RESULT result);
};
#endif
