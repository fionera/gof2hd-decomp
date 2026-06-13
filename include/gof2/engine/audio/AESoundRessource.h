#ifndef GOF2_AESOUNDRESSOURCE_H
#define GOF2_AESOUNDRESSOURCE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

struct AESoundInfo {
    int id;
    int field_4;
    int field_8;
};
struct AESoundInterface;

class AESoundRessource {
public:
    char* soundInfoTable;                    // +0x0  AESoundInfo* table base (byte addressed)
    uint32_t soundCount;                 // +0x4  count
    char* sounds;                    // +0x8  Array<AESoundInterface*> data
    volatile int numSounds;            // +0x10

    AESoundRessource();
    ~AESoundRessource();

    // Release every loaded AESoundInterface and clear its slot.
    void freeAllRessources();
    // Install the sound-info table (`info`, `count` entries) and size the sound array.
    void SetSound(AESoundInfo *info, int count);
    // Look up the entry whose id == `id`; copies it to `info` and reports the slot in `index`.
    void getSoundInfo(int id, AESoundInfo *info, int *index);
    // Lazily create + load the AESoundInterface for sound `id`.
    void init(int id);
    // Play sound `id` at the given volume.
    void play_impl(int id, int volume);
    // No-op in this build (kept for the loop-mode bookkeeping hook).
    void checkLooping();

    // Lazily create the AESoundInterface slot for `id` without loading its data.
    void initWithoutLoading(int id);
    // Play sound `id` (one-shot) at the given floating-point volume.
    void play(int id, float volume);
    // Start sound `id` looping.
    void playLoop(int id);
    // Start music track `id` (one-shot).
    void playMusic(int id);
    // Start music track `id` looping.
    void playMusicLoop(int id);
    // Stop sound `id`.
    void stop(int id);
    // Stop every loaded sound.
    void stopAll();
    // Pause sound `id`.
    void pause(int id);
    // Pause every loaded sound; returns the number of slots iterated.
    int pauseAll();
    // Resume sound `id`.
    void resume(int id);
    // Resume every loaded sound.
    void resumeAll();
    // Suspend every loaded sound (engine focus-loss hook).
    void suspendAll();
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

struct AESoundStackFrame {
    char info[12];
    int index;
    volatile int cookie;
};
#endif
