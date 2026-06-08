#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" int AEFile_FileExist(void *path);
extern "C" void AEFile_OpenRead(void *path, unsigned int *fd);
extern "C" void AEFile_ReadByte(void *out, unsigned int fd);
extern "C" void AEFile_ReadInt(void *out, unsigned int fd);
extern "C" void AEFile_ReadBool(void *out, unsigned int fd);
extern "C" void AEFile_ReadShort(void *out, unsigned int fd);
extern "C" void AEFile_ReadFloat(void *out, unsigned int fd);
extern "C" void AEFile_ReadString(void *out, unsigned int fd, int flag);
extern "C" void AEFile_Close(unsigned int fd);
extern "C" void AEString_default_ctor(void *s);
extern "C" void AEString_dtor(void *s);

__attribute__((visibility("hidden"))) extern int *g_LRV_guard;   // DAT_000dd444 -> guard holder

// RecordHandler::loadResolutionValue() — reads the options file at self+8 into a settings
// scratch struct. self is in r0; the path String lives at self+8.
extern "C" void RecordHandler_loadResolutionValue(RecordHandler *self)
{
    int *guardP = g_LRV_guard;
    volatile int saved = *guardP;

    void *path = (char *)self + 8;
    if (AEFile_FileExist(path) != 0) {
        unsigned int fd;
        AEFile_OpenRead(path, &fd);

        // Scratch settings record (mirrors the on-stack layout the target fills in).
        unsigned char buf[0x4c];
        char *r = (char *)buf;

        AEFile_ReadByte(r + 0x10, fd);
        AEFile_ReadByte(r + 0x11, fd);
        AEFile_ReadInt(r + 0x14, fd);
        AEFile_ReadInt(r + 0x18, fd);
        AEFile_ReadInt(r + 0x1c, fd);
        AEFile_ReadInt(r + 0x00, fd);
        AEFile_ReadByte(r + 0x0c, fd);
        AEFile_ReadByte(r + 0x0d, fd);
        AEFile_ReadInt(r + 0x04, fd);
        AEFile_ReadInt(r + 0x24, fd);
        AEFile_ReadInt(r + 0x28, fd);

        char name[12];
        AEString_default_ctor(name);
        AEFile_ReadString(name, fd, 0);

        int iv9c;
        bool b9d;
        AEFile_ReadInt(&iv9c, fd);
        AEFile_ReadBool(&b9d, fd);
        AEFile_ReadInt(r + 0x54, fd);
        AEFile_ReadInt(r + 0x58, fd);
        AEFile_ReadFloat(r + 0x08, fd);
        AEFile_ReadBool(r + 0x0e, fd);
        AEFile_ReadBool(r + 0x32, fd);
        AEFile_ReadBool(r + 0x33, fd);
        AEFile_ReadBool(r + 0x34, fd);
        AEFile_ReadFloat(r + 0x20, fd);

        short resVal = -1;
        AEFile_ReadShort(&resVal, fd);
        AEFile_ReadBool(&b9d, fd);
        AEFile_ReadBool(r + 0x35, fd);
        AEFile_ReadBool(r + 0x36, fd);
        AEFile_ReadBool(r + 0x3a, fd);
        AEFile_ReadBool(r + 0x3b, fd);
        AEFile_ReadBool(r + 0x3c, fd);
        AEFile_ReadBool(r + 0x37, fd);
        AEFile_ReadBool(r + 0x3d, fd);
        AEFile_ReadBool(r + 0x3e, fd);
        AEFile_ReadBool(r + 0x3f, fd);
        AEFile_ReadBool(r + 0x40, fd);
        AEFile_ReadBool(r + 0x41, fd);
        AEFile_ReadFloat(r + 0x44, fd);

        AEFile_Close(fd);
        AEString_dtor(name);
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
