#include "class.h"

extern "C" void *String_copy_ctor(void *dst, void *src, bool b);
extern "C" int AEFile_FileExist(void *s);
extern "C" void AEFile_FileDelete(void *s);
extern "C" void AEFile_OpenWrite(void *s, unsigned int *fd);
extern "C" void AEFile_Write(int n, signed char *buf, unsigned int fd);
extern "C" void AEFile_Close(unsigned int fd);
extern "C" void String_dtor(void *s);

// RecordHandler::writeByteArrayAsOptionsFile(signed char*, int)
extern "C" void RecordHandler_writeByteArrayAsOptionsFile(RecordHandler *self, signed char *buf, int n)
{
    char tmp[12];
    unsigned int fd;

    if (AEFile_FileExist(String_copy_ctor(tmp, (char *)self + 0x8, false)) != 0)
        AEFile_FileDelete(tmp);
    AEFile_OpenWrite(tmp, &fd);
    AEFile_Write(n, buf, fd);
    AEFile_Close(fd);
    String_dtor(tmp);
}
