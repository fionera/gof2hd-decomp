#include "gof2/AESoundRessource.h"


extern "C" void operator_delete(void *ptr);
extern "C" void ArraySetLength_AESoundInterface(int size, void *array);
extern "C" void AESoundRessource_play_impl(AbyssEngine::AESoundRessource *self, int sound, int volume);
extern "C" void Array_AESoundInterface_ctor(void *array);
extern "C" void AESoundRessource_getSoundInfo(AbyssEngine::AESoundRessource *self, int id, AbyssEngine::AESoundInfo *info, int *index);
extern "C" int AESoundRessource_init(AbyssEngine::AESoundRessource *self, int id);
extern "C" void *operator_new(unsigned int size);
extern "C" char AESoundInterface_vtable[];
extern "C" void AESoundRessource_freeAllRessources(AbyssEngine::AESoundRessource *self);
extern "C" void ArrayRemoveAll_AESoundInterface(void *array);
extern "C" void Array_AESoundInterface_dtor(void *array);

// ---- freeAllRessources_7fb0c.cpp ----
void AESoundRessource_freeAllRessources_7fb0c(AbyssEngine::AESoundRessource *self)
{
    for (uint32_t i = 0; i < self->soundCount; ++i) {
        void *sound = *(void **)(self->sounds + i * 4);
        if (sound != 0) {
            operator_delete(sound);
            *(int *)(self->sounds + i * 4) = 0;
        }
    }
}

// ---- SetSound_7fb74.cpp ----
void AESoundRessource_SetSound_7fb74(AbyssEngine::AESoundRessource *self, AbyssEngine::AESoundInfo *info, int count)
{
    self->numSounds = count;
    AbyssEngine::AESoundRessource *saved = self;
    *(AbyssEngine::AESoundInfo * volatile *)((char *)saved + 0) = info;
    ArraySetLength_AESoundInterface(count, (char *)saved + 4);
    int length = saved->soundCount;
    int zero = 0;
    for (int i = 0; length != i; ++i) {
        *(int *)(saved->sounds + i * 4) = zero;
    }
}

// ---- setVolume_7fd14.cpp ----
typedef void (*SoundIntMethod)(void *, int);

void AESoundRessource_setVolume_7fd14(AbyssEngine::AESoundRessource *self, int volume)
{
    for (uint32_t i = 0; i < self->soundCount; ++i) {
        void *sound = *(void **)(self->sounds + i * 4);
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x28))(sound, volume);
        }
    }
}

// ---- play_7fee4.cpp ----
void AESoundRessource_play_7fee4(AbyssEngine::AESoundRessource *self, int sound)
{
    return AESoundRessource_play_impl(self, sound, 0);
}

// ---- AESoundRessource_7fa78.cpp ----
AbyssEngine::AESoundRessource *AESoundRessource_7fa78(AbyssEngine::AESoundRessource *self)
{
    Array_AESoundInterface_ctor((char *)self + 4);
    self->numSounds = 0;
    self->soundInfoTable = 0;
    return self;
}

// ---- setMusicVolume_7fd70.cpp ----
typedef void (*SoundIntMethod)(void *, int);

void AESoundRessource_setMusicVolume_7fd70(AbyssEngine::AESoundRessource *self, int volume)
{
    for (uint32_t i = 0; i < self->soundCount; ++i) {
        void *sound = *(void **)(self->sounds + i * 4);
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x30))(sound, volume);
        }
    }
}

// ---- setSoundVolume_7fd42.cpp ----
typedef void (*SoundIntMethod)(void *, int);

void AESoundRessource_setSoundVolume_7fd42(AbyssEngine::AESoundRessource *self, int volume)
{
    for (uint32_t i = 0; i < self->soundCount; ++i) {
        void *sound = *(void **)(self->sounds + i * 4);
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x2c))(sound, volume);
        }
    }
}

// ---- stop_7fff4.cpp ----
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_stop_7fff4(AbyssEngine::AESoundRessource *self)
{
    for (uint32_t i = 0; i < self->soundCount; ++i) {
        void *sound = *(void **)(self->sounds + i * 4);
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x1c))(sound);
        }
    }
}

// ---- pause_80080.cpp ----
typedef void (*SoundVoidMethod)(void *);

int AESoundRessource_pause_80080(AbyssEngine::AESoundRessource *self)
{
    for (uint32_t i = 0; i < self->soundCount; ++i) {
        void *sound = *(void **)(self->sounds + i * 4);
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x14))(sound);
        }
    }
    return self->soundCount;
}

// ---- resume_800e8.cpp ----
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_resume_800e8(AbyssEngine::AESoundRessource *self)
{
    for (uint32_t i = 0; i < self->soundCount; ++i) {
        void *sound = *(void **)(self->sounds + i * 4);
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x18))(sound);
        }
    }
}

// ---- getSoundInfo_7fa30.cpp ----
void AESoundRessource_getSoundInfo_7fa30(AbyssEngine::AESoundRessource *self, int id, AbyssEngine::AESoundInfo *info, int *index)
{
    *index = -1;
    uint32_t count = self->soundCount;
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

// ---- supend_80018.cpp ----
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_supend_80018(AbyssEngine::AESoundRessource *self)
{
    for (uint32_t i = 0; i < self->soundCount; ++i) {
        void *sound = *(void **)(self->sounds + i * 4);
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 4))(sound);
        }
    }
}

// ---- stop_7ffb0.cpp ----
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_stop_7ffb0(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = *(void **)(self->sounds + frame.index * 4);
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x1c))(sound);
        }
    }
    
}

// ---- play_7feec.cpp ----
typedef int (*SoundIntReturnMethod)(void *);
typedef void (*SoundFloatMethod)(void *, float);

void AESoundRessource_play_7feec(AbyssEngine::AESoundRessource *self, int id, float volume)
{
    float savedVolume = volume;
    int savedId = id;
    AESoundStackFrame frame;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = *(void **)(self->sounds + index * 4);
        if ((*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x38))(sound) == 0) {
            AESoundRessource_init(self, savedId);
        }
        sound = *(void **)(self->sounds + index * 4);
        if (sound != 0) {
            (*(SoundFloatMethod *)((char *)*(void **)sound + 0xc))(sound, savedVolume);
        }
    }
    
}

// ---- playMusicLoop_7fe88.cpp ----
typedef int (*SoundIntReturnMethod)(void *);
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_playMusicLoop_7fe88(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = *(void **)(self->sounds + index * 4);
        if (sound != 0 && (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound) == 0) {
            sound = *(void **)(self->sounds + index * 4);
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x10))(sound);
        }
    }
    
}

// ---- release_7fde8.cpp ----
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_release_7fde8(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = *(void **)(self->sounds + frame.index * 4);
        (*(SoundVoidMethod *)((char *)*(void **)sound + 0x34))(sound);
    }
    
}

// ---- init_7fc38.cpp ----
static const char defaultSoundName[] = "";

typedef void (*SoundPointerMethod)(void *, const char *);
typedef void (*SoundIntMethod)(void *, int);

void AESoundRessource_init_7fc38(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    const char *name = defaultSoundName;
    *(const char * volatile *)(frame.info + 4) = name;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        char *sounds = self->sounds;
        void *sound = *(void **)(sounds + index * 4);
        if (sound == 0) {
            sound = operator_new(4);
            *(void **)sound = AESoundInterface_vtable + 8;
            *(void **)(sounds + index * 4) = sound;
            sound = *(void **)(self->sounds + index * 4);
        }
        (*(SoundPointerMethod *)*(void **)sound)(sound, *(const char **)(frame.info + 4));
        sound = *(void **)(self->sounds + index * 4);
        (*(SoundIntMethod *)((char *)*(void **)sound + 0x24))(sound, *(int *)(frame.info + 8));
    }
    
}

// ---- playMusic_7fe2c.cpp ----
typedef int (*SoundIntReturnMethod)(void *);
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_playMusic_7fe2c(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = *(void **)(self->sounds + index * 4);
        if (sound != 0 && (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound) == 0) {
            sound = *(void **)(self->sounds + index * 4);
            (*(SoundVoidMethod *)((char *)*(void **)sound + 8))(sound);
        }
    }
    
}

// ---- _AESoundRessource_7faaa.cpp ----
AbyssEngine::AESoundRessource::~AESoundRessource()
{
    AbyssEngine::AESoundRessource *self = this;
    AESoundRessource_freeAllRessources(self);
    void *array = (char *)self + 4;
    int zero = 0;
    int byteOffset = 0;
    uint32_t i = 0;
    while (i < *(uint32_t *)array) {
        char *data = self->sounds;
        void *sound = *(void **)(data + byteOffset);
        int *slot;
        if (sound == 0) {
            slot = (int *)(data + i * 4);
        } else {
            operator_delete(sound);
            slot = (int *)(self->sounds + byteOffset);
        }
        *slot = zero;
        byteOffset += 4;
        ++i;
    }
    ArrayRemoveAll_AESoundInterface(array);
    Array_AESoundInterface_dtor(array);
}

// ---- isPlaying_7fda0.cpp ----
typedef int (*SoundIntReturnMethod)(void *);

int AESoundRessource_isPlaying_7fda0(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int result;
    if (frame.index == -1) {
        result = 0;
    } else {
        void *sound = *(void **)(self->sounds + frame.index * 4);
        result = (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound);
    }
    
    return result;
}

// ---- resume_800a4.cpp ----
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_resume_800a4(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = *(void **)(self->sounds + frame.index * 4);
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x18))(sound);
        }
    }
    
}

// ---- playLoop_7ff54.cpp ----
typedef int (*SoundIntReturnMethod)(void *);
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_playLoop_7ff54(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = *(void **)(self->sounds + index * 4);
        if (sound != 0 && (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound) == 0) {
            sound = *(void **)(self->sounds + index * 4);
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x10))(sound);
        }
    }
    
}

// ---- initWithoutLoading_7fbd0.cpp ----

void AESoundRessource_initWithoutLoading_7fbd0(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    const char *name = defaultSoundName;
    *(const char * volatile *)(frame.info + 4) = name;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        char *sounds = self->sounds;
        if (*(void **)(sounds + index * 4) == 0) {
            void *sound = operator_new(4);
            *(void **)sound = AESoundInterface_vtable + 8;
            *(void **)(sounds + index * 4) = sound;
        }
    }
    
}

// ---- pause_8003c.cpp ----
typedef void (*SoundVoidMethod)(void *);

void AESoundRessource_pause_8003c(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = *(void **)(self->sounds + frame.index * 4);
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x14))(sound);
        }
    }
    
}

// ---- setVolume_7fcc4.cpp ----
typedef void (*SoundIntMethod)(void *, int);

void AESoundRessource_setVolume_7fcc4(AbyssEngine::AESoundRessource *self, int id, int volume)
{
    int savedVolume = volume;
    AESoundStackFrame frame;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = *(void **)(self->sounds + frame.index * 4);
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x28))(sound, savedVolume);
        }
    }
    
}
