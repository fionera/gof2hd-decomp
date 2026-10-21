#include "engine/audio/AESoundRessource.h"
#include "engine/audio/AESound.h"

namespace AbyssEngine {
    AESoundRessource::AESoundRessource() {
        this->soundInfoTable = nullptr;
        this->numSounds = 0;
    }

    AESoundRessource::~AESoundRessource() {
        freeAllRessources();
        for (uint32_t i = 0; i < this->sounds.size(); ++i) {
            if (this->sounds[i] != nullptr)
                delete this->sounds[i];
            this->sounds[i] = nullptr;
        }
        ArrayRemoveAll(this->sounds);
    }

    void AESoundRessource::freeAllRessources() {
        for (uint32_t i = 0; i < this->sounds.size(); ++i) {
            if (this->sounds[i] != nullptr) {
                delete this->sounds[i];
                this->sounds[i] = nullptr;
            }
        }
    }

    void AESoundRessource::SetSound(const AESoundInfo *info, int count) {
        this->soundInfoTable = info;
        this->numSounds = count;
        ArraySetLength(count, this->sounds);
        for (uint32_t i = 0; i < this->sounds.size(); ++i)
            this->sounds[i] = nullptr;
    }

    void AESoundRessource::getSoundInfo(int id, AESoundInfo &info, int &index) {
        index = -1;
        for (uint32_t i = 0; i < this->sounds.size(); ++i) {
            if (this->soundInfoTable[i].id == id) {
                index = i;
                info = this->soundInfoTable[i];
                return;
            }
        }
    }

    void AESoundRessource::init(int id) {
        int index;
        AESoundInfo info;
        info.name = "";
        this->getSoundInfo(id, info, index);
        if (index != -1) {
            if (this->sounds[index] == nullptr)
                this->sounds[index] = new AESound();
            this->sounds[index]->loadSound(info.name);
            this->sounds[index]->setGain(info.type);
        }
    }

    void AESoundRessource::checkLooping() {
    }

    void AESoundRessource::initWithoutLoading(int id) {
        int index;
        AESoundInfo info;
        info.name = "";
        this->getSoundInfo(id, info, index);
        if (index != -1 && this->sounds[index] == nullptr)
            this->sounds[index] = new AESound();
    }

    void AESoundRessource::play(int id, float volume) {
        int index;
        AESoundInfo info;
        this->getSoundInfo(id, info, index);
        if (index != -1) {
            if (!this->sounds[index]->loaded())
                this->init(id);
            if (this->sounds[index] != nullptr)
                this->sounds[index]->play(volume);
        }
    }

    void AESoundRessource::play(int id) {
        play(id, 0.0f);
    }

    void AESoundRessource::playLoop(int id) {
        int index;
        AESoundInfo info;
        this->getSoundInfo(id, info, index);
        if (index != -1 && this->sounds[index] != nullptr && !this->sounds[index]->isPlaying())
            this->sounds[index]->playLoop();
    }

    void AESoundRessource::playMusic(int id) {
        int index;
        AESoundInfo info;
        this->getSoundInfo(id, info, index);
        if (index != -1 && this->sounds[index] != nullptr && !this->sounds[index]->isPlaying())
            this->sounds[index]->play();
    }

    void AESoundRessource::playMusicLoop(int id) {
        int index;
        AESoundInfo info;
        this->getSoundInfo(id, info, index);
        if (index != -1 && this->sounds[index] != nullptr && !this->sounds[index]->isPlaying())
            this->sounds[index]->playLoop();
    }

    void AESoundRessource::stop(int id) {
        int index;
        AESoundInfo info;
        this->getSoundInfo(id, info, index);
        if (index != -1 && this->sounds[index] != nullptr)
            this->sounds[index]->stop();
    }

    void AESoundRessource::stop() {
        for (uint32_t i = 0; i < this->sounds.size(); ++i) {
            if (this->sounds[i] != nullptr)
                this->sounds[i]->stop();
        }
    }

    void AESoundRessource::pause(int id) {
        int index;
        AESoundInfo info;
        this->getSoundInfo(id, info, index);
        if (index != -1 && this->sounds[index] != nullptr)
            this->sounds[index]->pause();
    }

    void AESoundRessource::pause() {
        for (uint32_t i = 0; i < this->sounds.size(); ++i) {
            if (this->sounds[i] != nullptr)
                this->sounds[i]->pause();
        }
    }

    void AESoundRessource::resume(int id) {
        int index;
        AESoundInfo info;
        this->getSoundInfo(id, info, index);
        if (index != -1 && this->sounds[index] != nullptr)
            this->sounds[index]->resume();
    }

    void AESoundRessource::resume() {
        for (uint32_t i = 0; i < this->sounds.size(); ++i) {
            if (this->sounds[i] != nullptr)
                this->sounds[i]->resume();
        }
    }

    void AESoundRessource::supend() {
        for (uint32_t i = 0; i < this->sounds.size(); ++i) {
            if (this->sounds[i] != nullptr)
                this->sounds[i]->unloadSound();
        }
    }

    void AESoundRessource::release(int id) {
        int index;
        AESoundInfo info;
        this->getSoundInfo(id, info, index);
        if (index != -1)
            this->sounds[index]->release();
    }

    bool AESoundRessource::isPlaying(int id) {
        int index;
        AESoundInfo info;
        this->getSoundInfo(id, info, index);
        if (index == -1)
            return false;
        return this->sounds[index]->isPlaying();
    }

    void AESoundRessource::setVolume(int id, int volume) {
        int index;
        AESoundInfo info;
        this->getSoundInfo(id, info, index);
        if (index != -1 && this->sounds[index] != nullptr)
            this->sounds[index]->setVolume(volume);
    }

    void AESoundRessource::setVolume(int volume) {
        for (uint32_t i = 0; i < this->sounds.size(); ++i) {
            if (this->sounds[i] != nullptr)
                this->sounds[i]->setVolume(volume);
        }
    }

    void AESoundRessource::setSoundVolume(int volume) {
        for (uint32_t i = 0; i < this->sounds.size(); ++i) {
            if (this->sounds[i] != nullptr)
                this->sounds[i]->setSoundVolume(volume);
        }
    }

    void AESoundRessource::setMusicVolume(int volume) {
        for (uint32_t i = 0; i < this->sounds.size(); ++i) {
            if (this->sounds[i] != nullptr)
                this->sounds[i]->setMusicVolume(volume);
        }
    }
}
