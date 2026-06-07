#include "class.h"

extern "C" void AESoundRessource_getSoundInfo(AbyssEngine::AESoundRessource *self, int id, AbyssEngine::AESoundInfo *info, int *index);
extern "C" int __stack_chk_guard;
extern "C" void __stack_chk_fail(int diff) __attribute__((noreturn));

typedef int (*SoundIntReturnMethod)(void *);
typedef void (*SoundVoidMethod)(void *);

extern "C" void AESoundRessource_playMusicLoop_7fe88(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    frame.cookie = __stack_chk_guard;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = *(void **)(*(char **)((char *)self + 8) + index * 4);
        if (sound != 0 && (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound) == 0) {
            sound = *(void **)(*(char **)((char *)self + 8) + index * 4);
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x10))(sound);
        }
    }
    int diff = __stack_chk_guard;
    diff -= frame.cookie;
    if (diff != 0) {
        __stack_chk_fail(diff);
    }
}
