#include "class.h"

struct Station;
struct Ship;

extern "C" void String_int_ctor(void *dst, int v);
extern "C" void *String_concat(void *dst, void *a, void *b);
extern "C" void String_dtor(void *s);
extern "C" int AEFile_FileExist(void *s);
extern "C" void AEFile_FileDelete(void *s);
extern "C" void AEFile_OpenWrite(void *s, unsigned int *fd);
extern "C" void AEFile_Write_i64(long long v, unsigned int fd);
extern "C" void AEFile_Write_i32(int v, unsigned int fd);
extern "C" void AEFile_Write_str(void *s, unsigned int fd, int b);
extern "C" void AEFile_Write_f32(int v, unsigned int fd);
extern "C" void AEFile_Close(unsigned int fd);
extern "C" void *Station_getName(void *dst, void *st);
extern "C" int Ship_getIndex(void *sh);

// RecordHandler::recordStoreWritePreview(GameRecord*, int)
extern "C" int RecordHandler_recordStoreWritePreview(RecordHandler *self, void *rec, int slot)
{
    if (rec == 0)
        return 0;

    char path[12];
    char num[12];
    unsigned int fd;

    String_int_ctor(num, slot);
    String_concat(path, (char *)self + 0x20, num);
    String_dtor(num);

    if (AEFile_FileExist(path) != 0)
        AEFile_FileDelete(path);
    AEFile_OpenWrite(path, &fd);

    AEFile_Write_i64(*(long long *)((char *)rec + 0x10), fd);
    AEFile_Write_i32(I(rec, 0x8), fd);

    Station_getName(num, *(void **)((char *)rec + 0x138));
    AEFile_Write_str(num, fd, true);
    String_dtor(num);

    AEFile_Write_str((char *)rec + 0x188, fd, true);
    AEFile_Write_i32(I(rec, 0x40), fd);
    AEFile_Write_i32(I(rec, 0x20), fd);
    AEFile_Write_f32(I(rec, 0x11c), fd);
    AEFile_Write_i32(Ship_getIndex(*(void **)((char *)rec + 0x130)), fd);
    AEFile_Close(fd);
    String_dtor(path);
    return 1;
}
