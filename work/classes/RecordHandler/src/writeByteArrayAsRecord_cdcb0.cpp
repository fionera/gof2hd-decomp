#include "class.h"

extern "C" void String_int_ctor(void *dst, int v);
extern "C" void *String_concat(void *dst, void *a, void *b);
extern "C" void String_dtor(void *s);
extern "C" int AEFile_FileExist(void *s);
extern "C" void AEFile_FileDelete(void *s);
extern "C" void AEFile_OpenWrite(void *s, unsigned int *fd);
extern "C" void AEFile_Write(int n, signed char *buf, unsigned int fd);
extern "C" void AEFile_Close(unsigned int fd);

// RecordHandler::writeByteArrayAsRecord(signed char*, int, int, bool)
extern "C" int RecordHandler_writeByteArrayAsRecord(RecordHandler *self, signed char *buf, int n, int slot, bool toBackup)
{
    char num[12];
    char path[12];
    unsigned int fd;

    String_int_ctor(num, slot);
    char *dir = (char *)self + (toBackup ? 0x20 : 0x14);
    String_concat(path, dir, num);
    String_dtor(num);

    if (AEFile_FileExist(path) != 0)
        AEFile_FileDelete(path);
    AEFile_OpenWrite(path, &fd);
    AEFile_Write(n, buf, fd);
    AEFile_Close(fd);
    String_dtor(path);
    return n;
}
