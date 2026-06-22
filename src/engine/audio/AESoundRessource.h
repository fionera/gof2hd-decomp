#ifndef GOF2_AESOUNDRESSOURCE_H
#define GOF2_AESOUNDRESSOURCE_H
#include "engine/core/Array.h"
#include "../core/AEString.h"
#include "fieldaccess.h"

namespace AbyssEngine {
    struct AESoundInfo {
        int id;
        const char *name;
        int type;
    };

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

    class AESoundRessource {
    public:
        const AESoundInfo *soundInfoTable;
        Array<AESoundInterface *> *sounds;
        volatile int numSounds;

        AESoundRessource();

        ~AESoundRessource();

        void freeAllRessources();

        void SetSound(const AESoundInfo *info, int count);

        void getSoundInfo(int id, AESoundInfo &info, int &index);

        void init(int id);

        void checkLooping();

        void initWithoutLoading(int id);

        void play(int id, float volume);

        void play(int id);

        void playLoop(int id);

        void playMusic(int id);

        void playMusicLoop(int id);

        void stop(int id);

        void stop();

        void pause(int id);

        int pause();

        void resume(int id);

        void resume();

        void supend();

        void release(int id);

        bool isPlaying(int id);

        void setVolume(int id, int volume);

        void setVolume(int volume);

        void setSoundVolume(int volume);

        void setMusicVolume(int volume);
    };
}

#endif
