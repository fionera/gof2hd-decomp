#ifndef GOF2_AESOUNDRESSOURCE_H
#define GOF2_AESOUNDRESSOURCE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

namespace AbyssEngine {

// One entry of the sound-info table: a sound id, the name of the sample to load,
// and the playback type/channel the backend should use.
struct AESoundInfo {
    int id;
    const char *name;
    int type;
};

// One platform-mixer sound voice. The engine's audio backend provides the concrete
// implementation (its vtable lives in the runtime); the object itself carries no state
// beyond its vptr, so AESoundRessource allocates bare instances and drives them through
// these virtuals.
class AESoundInterface {
public:
    virtual void load(const char *name);      // open/prepare the named sample
    virtual void suspend();                   // engine focus-loss hook
    virtual void play();                      // start (music / no explicit volume)
    virtual void play(float volume);          // start at the given volume
    virtual void playLooping();               // start looping
    virtual void pause();
    virtual void resume();
    virtual void stop();
    virtual int  isPlaying();
    virtual void setType(int type);           // configured from AESoundInfo.type at load
    virtual void setVolume(int volume);
    virtual void setSoundVolume(int volume);  // SFX-channel volume
    virtual void setMusicVolume(int volume);  // music-channel volume
    virtual void release();                   // free the backing resource
    virtual int  isLoaded();
};

class AESoundRessource {
public:
    const AESoundInfo *soundInfoTable;       // sound-info table base
    Array<AESoundInterface*>* sounds;        // loaded-sound slots
    volatile int numSounds;                  // configured sound count

    AESoundRessource();
    ~AESoundRessource();

    // Release every loaded AESoundInterface and clear its slot.
    void freeAllRessources();
    // Install the sound-info table (`info`, `count` entries) and size the sound array.
    void SetSound(const AESoundInfo *info, int count);
    // Look up the entry whose id == `id`; copies it to `info` and reports the slot in `index`.
    void getSoundInfo(int id, AESoundInfo &info, int &index);
    // Lazily create + load the AESoundInterface for sound `id`.
    void init(int id);
    // No-op in this build (kept for the loop-mode bookkeeping hook).
    void checkLooping();

    // Lazily create the AESoundInterface slot for `id` without loading its data.
    void initWithoutLoading(int id);
    // Play sound `id` (one-shot) at the given floating-point volume.
    void play(int id, float volume);
    // Play sound `id` (one-shot) at zero volume.
    void play(int id);
    // Start sound `id` looping.
    void playLoop(int id);
    // Start music track `id` (one-shot).
    void playMusic(int id);
    // Start music track `id` looping.
    void playMusicLoop(int id);
    // Stop sound `id`.
    void stop(int id);
    // Stop every loaded sound.
    void stop();
    // Pause sound `id`.
    void pause(int id);
    // Pause every loaded sound; returns the number of slots iterated.
    int pause();
    // Resume sound `id`.
    void resume(int id);
    // Resume every loaded sound.
    void resume();
    // Suspend every loaded sound (engine focus-loss hook). [sic: original misspells "suspend"]
    void supend();
    // Release the backing resource for sound `id`.
    void release(int id);
    // True if sound `id` is currently playing.
    bool isPlaying(int id);
    // Set the volume of a single sound `id`.
    void setVolume(int id, int volume);
    // Set the volume of every loaded sound.
    void setVolume(int volume);
    // Set the per-sound (SFX-channel) volume of every loaded sound.
    void setSoundVolume(int volume);
    // Set the music-channel volume of every loaded sound.
    void setMusicVolume(int volume);
};

} // namespace AbyssEngine

#endif
