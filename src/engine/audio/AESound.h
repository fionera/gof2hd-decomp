#ifndef GOF2_AESOUND_H
#define GOF2_AESOUND_H

namespace AbyssEngine {
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
