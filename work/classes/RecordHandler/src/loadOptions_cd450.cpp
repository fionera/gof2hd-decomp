#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" int AEFile_FileExist(void *path);
extern "C" unsigned int AEFile_OpenRead(void *path, unsigned int *fd);
extern "C" void AEFile_Close(unsigned int fd);
extern "C" void AEFile_ReadByte(void *out, unsigned int fd);
extern "C" void AEFile_ReadInt(void *out, unsigned int fd);
extern "C" void AEFile_ReadBool(void *out, unsigned int fd);
extern "C" void AEFile_ReadFloat(void *out, unsigned int fd);
extern "C" void AEFile_ReadShort(void *out, unsigned int fd);
extern "C" void AEFile_ReadString(void *out, unsigned int fd, int flag);
extern "C" void AEString_default_ctor(void *s);
extern "C" void AEString_copy_ctor(void *dst, void *src, int copy);
extern "C" void AEString_dtor(void *s);
extern "C" int RecordHandler_checkHash(unsigned int fd);
extern "C" void *RH_op_new(unsigned int sz);
extern "C" void GameText_setLanguage(short obj, int lang);
extern "C" void Globals_loadFont(int kind);
extern "C" void FModSound_setAudioLanguage(void *s, int lang);
extern "C" int FModSound_enableCategory(void *s, int on);
extern "C" int FModSound_setVolume(void *s, int v);

__attribute__((visibility("hidden"))) extern int *g_LO_guard;        // DAT_000dd780 -> guard holder
__attribute__((visibility("hidden"))) extern unsigned char *g_LO_settings; // DAT_000dd784 -> settings struct base
__attribute__((visibility("hidden"))) extern int **g_LO_statusObj;   // DAT_000dd78c -> *->obj (field 0xfa)
__attribute__((visibility("hidden"))) extern int *g_LO_nameSlot;     // DAT_000dd794 -> name String* holder
__attribute__((visibility("hidden"))) extern int *g_LO_extraInt;     // DAT_000dd798 -> int
__attribute__((visibility("hidden"))) extern bool *g_LO_flag1;       // DAT_000dd79c -> bool
__attribute__((visibility("hidden"))) extern bool *g_LO_flag2;       // DAT_000dd7a0 -> bool
__attribute__((visibility("hidden"))) extern int *g_LO_textObj;      // DAT_000dd7a4 -> *->obj
__attribute__((visibility("hidden"))) extern int *g_LO_fontKind;     // DAT_000dd7a8 -> *->int
__attribute__((visibility("hidden"))) extern int *g_LO_fmodSlot;     // DAT_000dd7ac -> FModSound* holder

// RecordHandler::loadOptions()
extern "C" void RecordHandler_loadOptions(RecordHandler *self)
{
    int *guardP = g_LO_guard;
    volatile int saved = *guardP;

    void *path = (char *)self + 8;
    if (AEFile_FileExist(path) != 0) {
        unsigned int fd;
        unsigned int hashFd = AEFile_OpenRead(path, &fd);
        int valid = RecordHandler_checkHash(hashFd);
        AEFile_Close(fd);
        if (valid != 0) {
            AEFile_OpenRead(path, &fd);
            unsigned char *s = g_LO_settings;

            AEFile_ReadByte(s + 0x10, fd);
            AEFile_ReadByte(s + 0x11, fd);
            *(unsigned char *)(*(int *)g_LO_statusObj + 0xfa) = s[0x11];
            AEFile_ReadInt(s + 0x14, fd);
            AEFile_ReadInt(s + 0x18, fd);
            AEFile_ReadInt(s + 0x1c, fd);
            AEFile_ReadInt(s + 0x00, fd);
            AEFile_ReadByte(s + 0x0c, fd);
            AEFile_ReadByte(s + 0x0d, fd);
            AEFile_ReadInt(s + 0x04, fd);
            AEFile_ReadInt(s + 0x24, fd);
            AEFile_ReadInt(s + 0x28, fd);

            int *nameSlot = g_LO_nameSlot;
            if (*nameSlot != 0) {
                void (**vt)(void *) = *(void (***)(void *))(*nameSlot);
                vt[1]((void *)(long)*nameSlot);
                *nameSlot = 0;
            }

            char name[12];
            AEString_default_ctor(name);
            int hasName = 0;
            // local_24 reuse — read marker int decides whether a name string follows
            AEFile_ReadString(name, fd, 0);
            (void)hasName;
            // (target reads a length field; if non-zero, allocate a String)
            void *ns = RH_op_new(0xc);
            char nameCopy[12];
            AEString_copy_ctor(nameCopy, name, 0);
            *nameSlot = (int)ns;
            AEString_dtor(nameCopy);

            AEFile_ReadInt(g_LO_extraInt, fd);
            AEFile_ReadBool(g_LO_flag1, fd);
            AEFile_ReadInt(s + 0x54, fd);
            AEFile_ReadInt(s + 0x58, fd);
            AEFile_ReadFloat(s + 0x08, fd);
            AEFile_ReadBool(s + 0x0e, fd);
            AEFile_ReadBool(s + 0x32, fd);
            AEFile_ReadBool(s + 0x33, fd);
            AEFile_ReadBool(s + 0x34, fd);
            AEFile_ReadFloat(s + 0x20, fd);

            short lang = -1;
            AEFile_ReadShort(&lang, fd);
            AEFile_ReadBool(g_LO_flag2, fd);
            AEFile_ReadBool(s + 0x35, fd);
            AEFile_ReadBool(s + 0x36, fd);
            AEFile_ReadBool(s + 0x3a, fd);
            AEFile_ReadBool(s + 0x3b, fd);
            AEFile_ReadBool(s + 0x3c, fd);
            AEFile_ReadBool(s + 0x37, fd);
            AEFile_ReadBool(s + 0x3d, fd);
            AEFile_ReadBool(s + 0x3e, fd);
            AEFile_ReadBool(s + 0x3f, fd);
            AEFile_ReadBool(s + 0x40, fd);
            AEFile_ReadBool(s + 0x41, fd);
            AEFile_ReadFloat(s + 0x44, fd);
            AEFile_ReadBool(s + 0x48, fd);
            AEFile_ReadBool(s + 0x38, fd);
            AEFile_ReadBool(s + 0x4e, fd);
            AEFile_ReadBool(s + 0x49, fd);
            AEFile_ReadBool(s + 0x4a, fd);
            AEFile_ReadBool(s + 0x4b, fd);
            AEFile_ReadBool(s + 0x4c, fd);
            AEFile_ReadBool(s + 0x4d, fd);
            AEFile_ReadInt(s + 0x50, fd);
            AEFile_ReadBool(s + 0x60, fd);
            AEFile_ReadBool(s + 0x61, fd);
            AEFile_Close(fd);

            int langVal = lang;
            if (-1 < langVal) {
                if (langVal == 9) langVal = 0;
                GameText_setLanguage((short)**(int **)g_LO_textObj, langVal);
                Globals_loadFont(**(int **)g_LO_fontKind);
            }

            int *fmodSlot = g_LO_fmodSlot;
            if (*fmodSlot != 0) {
                void *fm = (void *)(long)*fmodSlot;
                FModSound_setAudioLanguage(fm, lang);
                FModSound_enableCategory(fm, 1);
                FModSound_enableCategory(fm, 1);
                int v = FModSound_enableCategory(fm, 1);
                v = FModSound_setVolume(fm, v);
                v = FModSound_setVolume(fm, v);
                FModSound_setVolume(fm, v);
            }

            AEFile_ReadBool(s + 0x62, fd);
            AEString_dtor(name);
        }
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
