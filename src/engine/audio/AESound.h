#ifndef GOF2_AESOUND_H
#define GOF2_AESOUND_H

namespace AbyssEngine {

// AESound -- the platform's stub sound voice.
//
// On this build the audio backend is supplied elsewhere (FModSound), so the
// generic AESound transport is compiled in as a do-nothing implementation: the
// loaders are no-ops, the transport calls do nothing, and the state queries
// report a sound that is always loaded and always playing. It carries no state
// of its own.
class AESound {
public:
    // Open/prepare the named sample. The stub simply hands the name back.
    char *loadSound(const char *name);
    // Free the backing resource.
    void unloadSound();

    // Transport.
    void play();
    void play(float volume);
    void playLoop();
    void pause();
    void resume();
    void stop();

    // State queries -- the stub always reports loaded/playing.
    int isPlaying();
    int loaded();

    // Mixer parameters.
    void setGain(int gain);
    void setVolume(int volume);
    void setSoundVolume(int volume);
    void setMusicVolume(int volume);

    // Free the voice.
    void release();
};

} // namespace AbyssEngine

#endif
