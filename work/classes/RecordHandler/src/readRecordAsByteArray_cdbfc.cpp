#include "class.h"

extern "C" void String_int_ctor(void *dst, int v);
extern "C" void *String_concat(void *dst, void *a, void *b);
extern "C" void String_dtor(void *s);
extern "C" int AEFile_FileExist(void *s);
extern "C" void AEFile_OpenRead(void *s, unsigned int *fd);
extern "C" int AEFile_GetFileSize(unsigned int fd);
extern "C" signed char *RH_op_new_arr(unsigned int n);
extern "C" void AEFile_Read(int n, signed char *buf, unsigned int fd);
extern "C" void AEFile_Close(unsigned int fd);

// RecordHandler::readRecordAsByteArray(signed char**, int, bool)
extern "C" int RecordHandler_readRecordAsByteArray(RecordHandler *self, signed char **out, int slot, bool fromBackup)
{
    char num[12];
    char path[12];
    unsigned int fd;
    int sz;

    String_int_ctor(num, slot);
    char *dir = (char *)self + (fromBackup ? 0x20 : 0x14);
    String_concat(path, dir, num);
    String_dtor(num);

    if (AEFile_FileExist(path) != 0) {
        AEFile_OpenRead(path, &fd);
        sz = AEFile_GetFileSize(fd);
        signed char *b = RH_op_new_arr(sz | (sz >> 31));
        *out = b;
        AEFile_Read(sz, b, fd);
        AEFile_Close(fd);
    } else {
        sz = -1;
    }
    String_dtor(path);
    return sz;
}
