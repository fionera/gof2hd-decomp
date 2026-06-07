#include "class.h"

extern "C" void String_int_ctor(void *dst, int v);
extern "C" void *String_concat(void *dst, void *a, void *b);
extern "C" void String_dtor(void *s);
extern "C" int AEFile_FileExist(void *s);
extern "C" void AEFile_OpenRead(void *s, unsigned int *fd);
extern "C" void *GR_op_new(unsigned int sz);
extern "C" void GameRecord_ctor(void *gr);
extern "C" void AEFile_Read_i64(void *dst, unsigned int fd);
extern "C" void AEFile_Read_i32(void *dst, unsigned int fd);
extern "C" void AEFile_Read_bool(void *dst, unsigned int fd, int b);
extern "C" void AEFile_Read_f32(void *dst, unsigned int fd);
extern "C" void AEFile_Close(unsigned int fd);

// RecordHandler::recordStoreReadPreview(int)
extern "C" void *RecordHandler_recordStoreReadPreview(RecordHandler *self, int slot)
{
    char path[12];
    char num[12];

    String_int_ctor(num, slot);
    String_concat(path, (char *)self + 0x20, num);
    String_dtor(num);

    unsigned int &fd = *(unsigned int *)num;  // reuse the now-dead String temp slot
    void *gr = 0;
    if (AEFile_FileExist(path) != 0) {
        AEFile_OpenRead(path, &fd);
        gr = GR_op_new(0x1c8);
        GameRecord_ctor(gr);
        AEFile_Read_i64((char *)gr + 0x10, fd);
        AEFile_Read_i32((char *)gr + 0x8, fd);
        AEFile_Read_bool((char *)gr + 0x194, fd, true);
        AEFile_Read_bool((char *)gr + 0x188, fd, true);
        AEFile_Read_i32((char *)gr + 0x40, fd);
        AEFile_Read_i32((char *)gr + 0x20, fd);
        AEFile_Read_f32((char *)gr + 0x11c, fd);
        AEFile_Read_i32((char *)gr + 0x1a0, fd);
        AEFile_Close(fd);
    }
    String_dtor(path);
    return gr;
}
