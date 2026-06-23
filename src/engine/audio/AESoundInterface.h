#ifndef GOF2_AESOUNDINTERFACE_H
#define GOF2_AESOUNDINTERFACE_H
#include "engine/core/Array.h"
#include "../core/AEString.h"
#include "fieldaccess.h"
#include "engine/audio/AESoundInfo.h"

namespace AbyssEngine {
class AESoundInterface {
    public:
        virtual void load(const char *name);

        virtual void suspend();

        virtual void play();

        virtual void play(float volume);

        virtual void playLooping();

        virtual void pause();

        virtual void resume();

        virtual void stop();

        virtual int isPlaying();

        virtual void setType(int type);

        virtual void setVolume(int volume);

        virtual void setSoundVolume(int volume);

        virtual void setMusicVolume(int volume);

        virtual void release();

        virtual int isLoaded();
    };
}
#endif
