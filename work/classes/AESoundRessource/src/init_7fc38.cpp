#include "class.h"

extern "C" void AESoundRessource_getSoundInfo(AbyssEngine::AESoundRessource *self, int id, AbyssEngine::AESoundInfo *info, int *index);
extern "C" void *operator_new(unsigned int size);
extern "C" char AESoundInterface_vtable[];
extern "C" int __stack_chk_guard;
extern "C" void __stack_chk_fail(int diff) __attribute__((noreturn));

static const char defaultSoundName[] = "";

typedef void (*SoundPointerMethod)(void *, const char *);
typedef void (*SoundIntMethod)(void *, int);

extern "C" void AESoundRessource_init_7fc38(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    const char *name = defaultSoundName;
    frame.cookie = __stack_chk_guard;
    *(const char * volatile *)(frame.info + 4) = name;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        char *sounds = *(char **)((char *)self + 8);
        void *sound = *(void **)(sounds + index * 4);
        if (sound == 0) {
            sound = operator_new(4);
            *(void **)sound = AESoundInterface_vtable + 8;
            *(void **)(sounds + index * 4) = sound;
            sound = *(void **)(*(char **)((char *)self + 8) + index * 4);
        }
        (*(SoundPointerMethod *)*(void **)sound)(sound, *(const char **)(frame.info + 4));
        sound = *(void **)(*(char **)((char *)self + 8) + index * 4);
        (*(SoundIntMethod *)((char *)*(void **)sound + 0x24))(sound, *(int *)(frame.info + 8));
    }
    int diff = __stack_chk_guard;
    diff -= frame.cookie;
    if (diff != 0) {
        __stack_chk_fail(diff);
    }
}
