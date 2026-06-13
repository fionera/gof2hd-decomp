#include "gof2/engine/audio/AESoundRessource.h"

extern "C" char AESoundInterface_vtable[];

void AESoundRessource_freeAllRessources_7fb0c(AbyssEngine::AESoundRessource *self)
{
    for (uint32_t i = 0; i < self->sounds->size(); ++i) {
        void *sound = (*self->sounds)[i];
        if (sound != 0) {
            ::operator delete(sound);
            (*self->sounds)[i] = nullptr;
        }
    }
}

void AESoundRessource_SetSound_7fb74(AbyssEngine::AESoundRessource *self, AbyssEngine::AESoundInfo *info, int count)
{
    self->numSounds = count;
    self->soundInfoTable = (char *)info;
    self->sounds->resize(count);
    for (uint32_t i = 0; i < self->sounds->size(); ++i) {
        (*self->sounds)[i] = nullptr;
    }
}

typedef void (*SoundIntMethod)(void *, int);

void AESoundRessource_setVolume_7fd14(AbyssEngine::AESoundRessource *self, int volume)
{
    for (uint32_t i = 0; i < self->sounds->size(); ++i) {
        void *sound = (*self->sounds)[i];
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x28))(sound, volume);
        }
    }
}

void AESoundRessource_play_7fee4(AbyssEngine::AESoundRessource *self, int sound)
{
    return ((AbyssEngine::AESoundRessource *)(self))->play_impl(sound, 0);
}

AbyssEngine::AESoundRessource *AESoundRessource_7fa78(AbyssEngine::AESoundRessource *self)
{
    self->sounds = new Array<AbyssEngine::AESoundInterface *>();
    self->numSounds = 0;
    self->soundInfoTable = 0;
    return self;
}

typedef void (*SoundIntMethod)(void *, int);

void AESoundRessource_setMusicVolume_7fd70(AbyssEngine::AESoundRessource *self, int volume)
{
    for (uint32_t i = 0; i < self->sounds->size(); ++i) {
        void *sound = (*self->sounds)[i];
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x30))(sound, volume);
        }
    }
}

typedef void (*SoundIntMethod)(void *, int);

void AESoundRessource_setSoundVolume_7fd42(AbyssEngine::AESoundRessource *self, int volume)
{
    for (uint32_t i = 0; i < self->sounds->size(); ++i) {
        void *sound = (*self->sounds)[i];
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x2c))(sound, volume);
        }
    }
}

typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_stop_7fff4(AbyssEngine::AESoundRessource *self)
{
    for (uint32_t i = 0; i < self->sounds->size(); ++i) {
        void *sound = (*self->sounds)[i];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x1c))(sound);
        }
    }
}

typedef void (*SoundVoidMethod)(void *);

int AESoundRessource_pause_80080(AbyssEngine::AESoundRessource *self)
{
    for (uint32_t i = 0; i < self->sounds->size(); ++i) {
        void *sound = (*self->sounds)[i];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x14))(sound);
        }
    }
    return self->sounds->size();
}

typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_resume_800e8(AbyssEngine::AESoundRessource *self)
{
    for (uint32_t i = 0; i < self->sounds->size(); ++i) {
        void *sound = (*self->sounds)[i];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x18))(sound);
        }
    }
}

void AESoundRessource_getSoundInfo_7fa30(AbyssEngine::AESoundRessource *self, int id, AbyssEngine::AESoundInfo *info, int *index)
{
    *index = -1;
    uint32_t count = self->sounds->size();
    for (uint32_t i = 0; i < count; ++i) {
        char *base = self->soundInfoTable;
        AbyssEngine::AESoundInfo *entry = (AbyssEngine::AESoundInfo *)(base + i * 12);
        if (entry->id == id) {
            *index = i;
            *info = *entry;
            return;
        }
    }
}

typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_supend_80018(AbyssEngine::AESoundRessource *self)
{
    for (uint32_t i = 0; i < self->sounds->size(); ++i) {
        void *sound = (*self->sounds)[i];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 4))(sound);
        }
    }
}

typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_stop_7ffb0(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    ((AbyssEngine::AESoundRessource *)(self))->getSoundInfo(id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = (*self->sounds)[frame.index];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x1c))(sound);
        }
    }
    
}

typedef int (*SoundIntReturnMethod)(void *);
typedef void (*SoundFloatMethod)(void *, float);

void AESoundRessource_play_7feec(AbyssEngine::AESoundRessource *self, int id, float volume)
{
    float savedVolume = volume;
    int savedId = id;
    AESoundStackFrame frame;
    ((AbyssEngine::AESoundRessource *)(self))->getSoundInfo(id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = (*self->sounds)[index];
        if ((*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x38))(sound) == 0) {
            ((AbyssEngine::AESoundRessource *)(self))->init(savedId);
        }
        sound = (*self->sounds)[index];
        if (sound != 0) {
            (*(SoundFloatMethod *)((char *)*(void **)sound + 0xc))(sound, savedVolume);
        }
    }
    
}

typedef int (*SoundIntReturnMethod)(void *);
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_playMusicLoop_7fe88(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    ((AbyssEngine::AESoundRessource *)(self))->getSoundInfo(id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = (*self->sounds)[index];
        if (sound != 0 && (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound) == 0) {
            sound = (*self->sounds)[index];
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x10))(sound);
        }
    }
    
}

typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_release_7fde8(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    ((AbyssEngine::AESoundRessource *)(self))->getSoundInfo(id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = (*self->sounds)[frame.index];
        (*(SoundVoidMethod *)((char *)*(void **)sound + 0x34))(sound);
    }
    
}

static const char defaultSoundName[] = "";

typedef void (*SoundPointerMethod)(void *, const char *);
typedef void (*SoundIntMethod)(void *, int);

void AESoundRessource_init_7fc38(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    const char *name = defaultSoundName;
    *(const char * volatile *)(frame.info + 4) = name;
    ((AbyssEngine::AESoundRessource *)(self))->getSoundInfo(id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = (*self->sounds)[index];
        if (sound == 0) {
            sound = ::operator new(4);
            *(void **)sound = AESoundInterface_vtable + 8;
            (*self->sounds)[index] = (AbyssEngine::AESoundInterface *)sound;
            sound = (*self->sounds)[index];
        }
        (*(SoundPointerMethod *)*(void **)sound)(sound, *(const char **)(frame.info + 4));
        sound = (*self->sounds)[index];
        (*(SoundIntMethod *)((char *)*(void **)sound + 0x24))(sound, *(int *)(frame.info + 8));
    }
    
}

typedef int (*SoundIntReturnMethod)(void *);
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_playMusic_7fe2c(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    ((AbyssEngine::AESoundRessource *)(self))->getSoundInfo(id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = (*self->sounds)[index];
        if (sound != 0 && (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound) == 0) {
            sound = (*self->sounds)[index];
            (*(SoundVoidMethod *)((char *)*(void **)sound + 8))(sound);
        }
    }
    
}

AbyssEngine::AESoundRessource::~AESoundRessource()
{
    freeAllRessources();
    for (uint32_t i = 0; i < sounds->size(); ++i) {
        void *sound = (*sounds)[i];
        if (sound != nullptr)
            ::operator delete(sound);
        (*sounds)[i] = nullptr;
    }
    delete sounds;
}

typedef int (*SoundIntReturnMethod)(void *);

int AESoundRessource_isPlaying_7fda0(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    ((AbyssEngine::AESoundRessource *)(self))->getSoundInfo(id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int result;
    if (frame.index == -1) {
        result = 0;
    } else {
        void *sound = (*self->sounds)[frame.index];
        result = (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound);
    }
    
    return result;
}

typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_resume_800a4(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    ((AbyssEngine::AESoundRessource *)(self))->getSoundInfo(id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = (*self->sounds)[frame.index];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x18))(sound);
        }
    }
    
}

typedef int (*SoundIntReturnMethod)(void *);
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_playLoop_7ff54(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    ((AbyssEngine::AESoundRessource *)(self))->getSoundInfo(id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = (*self->sounds)[index];
        if (sound != 0 && (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound) == 0) {
            sound = (*self->sounds)[index];
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x10))(sound);
        }
    }
    
}

void AESoundRessource_initWithoutLoading_7fbd0(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    const char *name = defaultSoundName;
    *(const char * volatile *)(frame.info + 4) = name;
    ((AbyssEngine::AESoundRessource *)(self))->getSoundInfo(id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        if ((*self->sounds)[index] == nullptr) {
            void *sound = ::operator new(4);
            *(void **)sound = AESoundInterface_vtable + 8;
            (*self->sounds)[index] = (AbyssEngine::AESoundInterface *)sound;
        }
    }
    
}

typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_pause_8003c(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    ((AbyssEngine::AESoundRessource *)(self))->getSoundInfo(id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = (*self->sounds)[frame.index];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x14))(sound);
        }
    }
    
}

typedef void (*SoundIntMethod)(void *, int);

void AESoundRessource_setVolume_7fcc4(AbyssEngine::AESoundRessource *self, int id, int volume)
{
    int savedVolume = volume;
    AESoundStackFrame frame;
    ((AbyssEngine::AESoundRessource *)(self))->getSoundInfo(id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = (*self->sounds)[frame.index];
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x28))(sound, savedVolume);
        }
    }

}

// ---- class methods (real member wrappers over the recovered bodies above) ----
namespace AbyssEngine {

// Constructs the AESoundInterface array, then zeroes the count and info-table pointer.
AESoundRessource::AESoundRessource()
{
    this->sounds = new Array<AESoundInterface *>();
    this->numSounds = 0;
    this->soundInfoTable = 0;
}

void AESoundRessource::freeAllRessources()
{
    AESoundRessource_freeAllRessources_7fb0c(this);
}

void AESoundRessource::SetSound(AESoundInfo *info, int count)
{
    AESoundRessource_SetSound_7fb74(this, info, count);
}

void AESoundRessource::getSoundInfo(int id, AESoundInfo *info, int *index)
{
    AESoundRessource_getSoundInfo_7fa30(this, id, info, index);
}

void AESoundRessource::init(int id)
{
    AESoundRessource_init_7fc38(this, id);
}

void AESoundRessource::play_impl(int id, int volume)
{
    AESoundRessource_play_7feec(this, id, (float)volume);
}

// Empty in this build (the looping bookkeeping was compiled out).
void AESoundRessource::checkLooping()
{
}

// ---- one-shot / loop playback ----------------------------------------------
void AESoundRessource::initWithoutLoading(int id)
{
    AESoundRessource_initWithoutLoading_7fbd0(this, id);
}

void AESoundRessource::play(int id, float volume)
{
    AESoundRessource_play_7feec(this, id, volume);
}

void AESoundRessource::playLoop(int id)
{
    AESoundRessource_playLoop_7ff54(this, id);
}

void AESoundRessource::playMusic(int id)
{
    AESoundRessource_playMusic_7fe2c(this, id);
}

void AESoundRessource::playMusicLoop(int id)
{
    AESoundRessource_playMusicLoop_7fe88(this, id);
}

// ---- stop / pause / resume / suspend ---------------------------------------
void AESoundRessource::stop(int id)
{
    AESoundRessource_stop_7ffb0(this, id);
}

void AESoundRessource::stopAll()
{
    AESoundRessource_stop_7fff4(this);
}

void AESoundRessource::pause(int id)
{
    AESoundRessource_pause_8003c(this, id);
}

int AESoundRessource::pauseAll()
{
    return AESoundRessource_pause_80080(this);
}

void AESoundRessource::resume(int id)
{
    AESoundRessource_resume_800a4(this, id);
}

void AESoundRessource::resumeAll()
{
    AESoundRessource_resume_800e8(this);
}

void AESoundRessource::suspendAll()
{
    AESoundRessource_supend_80018(this);
}

void AESoundRessource::release(int id)
{
    AESoundRessource_release_7fde8(this, id);
}

bool AESoundRessource::isPlaying(int id)
{
    return AESoundRessource_isPlaying_7fda0(this, id) != 0;
}

// ---- volume control --------------------------------------------------------
void AESoundRessource::setVolume(int id, int volume)
{
    AESoundRessource_setVolume_7fcc4(this, id, volume);
}

void AESoundRessource::setVolume(int volume)
{
    AESoundRessource_setVolume_7fd14(this, volume);
}

void AESoundRessource::setSoundVolume(int volume)
{
    AESoundRessource_setSoundVolume_7fd42(this, volume);
}

void AESoundRessource::setMusicVolume(int volume)
{
    AESoundRessource_setMusicVolume_7fd70(this, volume);
}

} // namespace AbyssEngine
