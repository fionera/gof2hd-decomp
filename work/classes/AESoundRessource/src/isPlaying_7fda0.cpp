#include "class.h"

extern "C" void AESoundRessource_getSoundInfo(AbyssEngine::AESoundRessource *self, int id, AbyssEngine::AESoundInfo *info, int *index);
extern "C" int __stack_chk_guard;
extern "C" void __stack_chk_fail(int value) __attribute__((noreturn));

typedef int (*SoundIntReturnMethod)(void *);

extern "C" int AESoundRessource_isPlaying_7fda0(AbyssEngine::AESoundRessource *self, int id)
{
    AESoundStackFrame frame;
    frame.cookie = __stack_chk_guard;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int result;
    if (frame.index == -1) {
        result = 0;
    } else {
        void *sound = *(void **)(*(char **)((char *)self + 8) + frame.index * 4);
        result = (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound);
    }
    int diff = __stack_chk_guard - frame.cookie;
    if (diff != 0) {
        __stack_chk_fail(result);
    }
    return result;
}
