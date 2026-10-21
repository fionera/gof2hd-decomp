#ifndef GOF2_AESOUND_H
#define GOF2_AESOUND_H
#include "engine/audio/AESoundInterface.h"

namespace AbyssEngine {
    // Null sound backend. Every member is defined inline: the original exports them all WEAK.
    class AESound : public AESoundInterface {
    public:
        void loadSound(const char *name) override {
            (void) name;
        }

        void unloadSound() override {
        }

        void play() override {
        }

        void play(float volume) override {
            (void) volume;
        }

        void playLoop() override {
        }

        void pause() override {
        }

        void resume() override {
        }

        void stop() override {
        }

        bool isPlaying() override {
            return true;
        }

        int loaded() override {
            return 1;
        }

        void setGain(int gain) override {
            (void) gain;
        }

        void setVolume(int volume) override {
            (void) volume;
        }

        void setSoundVolume(int volume) override {
            (void) volume;
        }

        void setMusicVolume(int volume) override {
            (void) volume;
        }

        void release() override {
        }
    };
}

#endif
