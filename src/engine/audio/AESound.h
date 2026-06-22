#ifndef GOF2_AESOUND_H
#define GOF2_AESOUND_H

namespace AbyssEngine {
    class AESound {
    public:
        char *loadSound(const char *name);

        void unloadSound();

        void play();

        void play(float volume);

        void playLoop();

        void pause();

        void resume();

        void stop();

        int isPlaying();

        int loaded();

        void setGain(int gain);

        void setVolume(int volume);

        void setSoundVolume(int volume);

        void setMusicVolume(int volume);

        void release();
    };
}

#endif
