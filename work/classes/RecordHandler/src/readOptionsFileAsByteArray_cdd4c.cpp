#include "class.h"

extern "C" void *String_copy_ctor(void *dst, void *src, bool b);
extern "C" int AEFile_FileExist(void *s);
extern "C" void AEFile_OpenRead(void *s, unsigned int *fd);
extern "C" int AEFile_GetFileSize(unsigned int fd);
extern "C" signed char *RH_op_new_arr(unsigned int n);
extern "C" void AEFile_Read(int n, signed char *buf, unsigned int fd);
extern "C" void AEFile_Close(unsigned int fd);
extern "C" void String_dtor(void *s);

// RecordHandler::readOptionsFileAsByteArray(signed char**)
extern "C" int RecordHandler_readOptionsFileAsByteArray(RecordHandler *self, signed char **out)
{
    char tmp[12];
    unsigned int fd;
    int sz;

    String_copy_ctor(tmp, (char *)self + 0x8, false);
    if (AEFile_FileExist(tmp) != 0) {
        AEFile_OpenRead(tmp, &fd);
        sz = AEFile_GetFileSize(fd);
        signed char *buf = RH_op_new_arr(sz | (sz >> 31));
        *out = buf;
        AEFile_Read(sz, buf, fd);
        AEFile_Close(fd);
    } else {
        sz = -1;
    }
    String_dtor(tmp);
    return sz;
}
