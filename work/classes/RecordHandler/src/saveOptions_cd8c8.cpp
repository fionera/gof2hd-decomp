#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" int AEFile_FileExist(void *path);
extern "C" void AEFile_FileDelete(void *path);
extern "C" void AEFile_OpenWrite(void *path, unsigned int *fd);
extern "C" void AEFile_WriteByte(int v, unsigned int fd);
extern "C" void AEFile_WriteInt(int v, unsigned int fd);
extern "C" void AEFile_WriteFloat(int v, unsigned int fd);
extern "C" void AEFile_WriteShort(int v, unsigned int fd);
extern "C" void AEFile_WriteString(void *s, unsigned int fd, int flag);
extern "C" void AEFile_Close(unsigned int fd);
extern "C" void AEString_cstr_ctor(void *dst, const char *s, int copy);
extern "C" void AEString_dtor(void *s);
extern "C" int GameText_getLanguage();
extern "C" void RecordHandler_addHashToOptions(RecordHandler *self);

__attribute__((visibility("hidden"))) extern int *g_SO_guard;        // DAT_000ddad4 -> guard holder
__attribute__((visibility("hidden"))) extern unsigned char *g_SO_settings; // DAT_000ddad8 -> settings struct
__attribute__((visibility("hidden"))) extern void **g_SO_playerName; // DAT_000ddae4 -> *->String*
__attribute__((visibility("hidden"))) extern const char g_SO_defName[]; // DAT_000ddae8
__attribute__((visibility("hidden"))) extern int *g_SO_extraInt;     // DAT_000ddaec -> *->int
__attribute__((visibility("hidden"))) extern unsigned char *g_SO_flag1; // DAT_000ddaf4 -> byte
__attribute__((visibility("hidden"))) extern unsigned char *g_SO_flag2; // DAT_000ddaf8 -> byte

// RecordHandler::saveOptions()
extern "C" void RecordHandler_saveOptions(RecordHandler *self)
{
    int *guardP = g_SO_guard;
    volatile int saved = *guardP;

    void *path = (char *)self + 8;
    if (AEFile_FileExist(path) != 0) {
        AEFile_FileDelete(path);
    }
    unsigned int fd;
    AEFile_OpenWrite(path, &fd);

    unsigned char *s = g_SO_settings;

    AEFile_WriteByte(s[0x10], fd);
    AEFile_WriteByte(s[0x11], fd);
    AEFile_WriteInt(*(int *)(s + 0x14), fd);
    AEFile_WriteInt(*(int *)(s + 0x18), fd);
    AEFile_WriteInt(*(int *)(s + 0x1c), fd);
    AEFile_WriteInt(*(int *)(s + 0x00), fd);
    AEFile_WriteByte(s[0x0c], fd);
    AEFile_WriteByte(s[0x0d], fd);
    AEFile_WriteInt(*(int *)(s + 0x04), fd);
    AEFile_WriteInt(*(int *)(s + 0x24), fd);
    AEFile_WriteInt(*(int *)(s + 0x28), fd);

    void *name = *g_SO_playerName;
    if (name == 0) {
        char def[12];
        AEString_cstr_ctor(def, g_SO_defName, 0);
        AEFile_WriteString(def, fd, 0);
        AEString_dtor(def);
    } else {
        AEFile_WriteString(name, fd, 0);
    }

    AEFile_WriteInt(*g_SO_extraInt, fd);
    AEFile_WriteByte(*g_SO_flag1, fd);
    AEFile_WriteInt(*(int *)(s + 0x54), fd);
    AEFile_WriteInt(*(int *)(s + 0x58), fd);
    AEFile_WriteInt(*(int *)(s + 0x08), fd);
    AEFile_WriteByte(s[0x0e], fd);
    AEFile_WriteByte(s[0x32], fd);
    AEFile_WriteByte(s[0x33], fd);
    AEFile_WriteByte(s[0x34], fd);
    AEFile_WriteInt(*(int *)(s + 0x20), fd);

    AEFile_WriteShort(GameText_getLanguage(), fd);
    AEFile_WriteByte(*g_SO_flag2, fd);
    AEFile_WriteByte(s[0x35], fd);
    AEFile_WriteByte(s[0x36], fd);
    AEFile_WriteByte(s[0x3a], fd);
    AEFile_WriteByte(s[0x3b], fd);
    AEFile_WriteByte(s[0x3c], fd);
    AEFile_WriteByte(s[0x37], fd);
    AEFile_WriteByte(s[0x3d], fd);
    AEFile_WriteByte(s[0x3e], fd);
    AEFile_WriteByte(s[0x3f], fd);
    AEFile_WriteByte(s[0x40], fd);
    AEFile_WriteByte(s[0x41], fd);
    AEFile_WriteInt(*(int *)(s + 0x44), fd);
    AEFile_WriteByte(s[0x48], fd);
    AEFile_WriteByte(s[0x38], fd);
    AEFile_WriteByte(s[0x4e], fd);
    AEFile_WriteByte(s[0x49], fd);
    AEFile_WriteByte(s[0x4a], fd);
    AEFile_WriteByte(s[0x4b], fd);
    AEFile_WriteByte(s[0x4c], fd);
    AEFile_WriteByte(s[0x4d], fd);
    AEFile_WriteInt(*(int *)(s + 0x50), fd);
    AEFile_WriteByte(s[0x60], fd);
    AEFile_WriteByte(s[0x61], fd);
    AEFile_WriteByte(s[0x62], fd);
    AEFile_Close(fd);
    RecordHandler_addHashToOptions(self);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
