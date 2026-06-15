#ifndef GOF2_FMODSOUND_H
#define GOF2_FMODSOUND_H
#include "gof2/common.h"

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
    void    *events[0x8f5];        // event handle array (slot per system id)
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
    // the live FMOD event handle lives on it at Player::engineEvent.
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
