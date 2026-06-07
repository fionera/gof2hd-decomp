#include "class.h"

// FModSound::init()
extern "C" void FMOD_EventSystem_Create(void *out);
extern "C" int FMOD_EventSystem_init(void *system, int maxch, void *extdriver, int flags);
extern "C" int GameText_getLanguage();
extern "C" void *__aeabi_memclr8(void *d, uint32_t n);
extern "C" char *strcpy(char *d, const char *s);
extern "C" uint32_t strlen(const char *s);
extern "C" int FMOD_EventSystem_load(void *system, const char *name, void *proj);
extern "C" void FMOD_EventSystem_getCategory(void *system, void *out);
extern "C" void FMOD_EventSystem_getProjectByIndex(void *system, void *out);

__attribute__((visibility("hidden"))) static const char kSuffixA[16] = ".fev";
__attribute__((visibility("hidden"))) static const char kSuffixB[24] = "_low.fev";
__attribute__((visibility("hidden"))) static void *kCats[4];

struct FModSound {
    void setAudioLanguage(int lang);
    int init();
};
int FModSound::init()
{
    for (int i = 0; i != 5; i++)
        u32(this, i * 4 + 0x2410) = 0xffffffff;
    FMOD_EventSystem_Create((char *)this + OFF_SYSTEM);
    FMOD_EventSystem_init(pp(this, OFF_SYSTEM), 0x20, (void *)0x82, 0);
    setAudioLanguage(GameText_getLanguage());

    char path[0x400];
    __aeabi_memclr8(path, 0x400);
    strcpy(path, (const char *)pp(this, 0xc));
    uint8_t lowFlag = u8(this, 0x10);
    char *end = path + strlen(path);
    if (lowFlag == 0) {
        __builtin_memcpy(end, kSuffixB, 16);
    } else {
        __builtin_memcpy(end, kSuffixA, 14);
    }
    FMOD_EventSystem_load(pp(this, OFF_SYSTEM), path, 0);
    for (int i = 0; i != 0x10; i += 4) {
        u32(this, i + OFF_CATEGORY) = 0;
        FMOD_EventSystem_getCategory(pp(this, OFF_SYSTEM), kCats[i / 4]);
    }
    FMOD_EventSystem_getProjectByIndex(pp(this, OFF_SYSTEM), 0);
    u32(this, 0x240c) = 0;
    u32(this, 0x2408) = 0xffffffff;
    u32(this, 0) = 0xffffffff;
    u32(this, 4) = 0xffffffff;
    u8(this, 8) = 0;
    return 0;
}
