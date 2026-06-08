#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void AEString_int_ctor(void *dst, int v);
extern "C" void AEString_concat(void *dst, void *a);
extern "C" void AEString_dtor(void *s);
extern "C" int AEFile_FileExist(void *path);
extern "C" unsigned int AEFile_OpenRead(void *path, unsigned int *fd);
extern "C" void AEFile_Close(unsigned int fd);
extern "C" void AEFile_ReadInt(void *out, unsigned int fd);
extern "C" void AEFile_ReadBool(void *out, unsigned int fd);
extern "C" void AEFile_ReadLong(void *out, unsigned int fd);
extern "C" int RecordHandler_checkHash(unsigned int fd);
extern "C" void *RH_op_new(unsigned int sz);
extern "C" void GameRecord_ctor(void *self);
extern "C" void Array_bool_ctor(void *a);
extern "C" void ArraySetLength_bool(unsigned n, void *a);
extern "C" void *RecordHandler_readMission(RecordHandler *self, unsigned int fd);

// Deep GameRecord field read (ship, stations, standings, blueprints, pending products, agents,
// ships, wanteds, flags). A very long straight-line reader delegated to a helper so this
// translation preserves the path/hash/open/prefix/close flow while staying tractable.
extern "C" void RecordHandler_recordStoreRead_body(RecordHandler *self, void *rec, unsigned int fd);

__attribute__((visibility("hidden"))) extern int *g_RSR_guard;   // DAT_000dc094 -> guard holder

// RecordHandler::recordStoreRead(int slot)
extern "C" void RecordHandler_recordStoreRead(RecordHandler *self, int slot)
{
    int *guardP = g_RSR_guard;
    volatile int saved = *guardP;

    char num[12], path[12];
    AEString_int_ctor(num, slot);
    AEString_concat(path, (char *)self + 0x14);
    AEString_dtor(num);

    if (AEFile_FileExist(path) != 0) {
        unsigned int fd;
        unsigned int hashFd = AEFile_OpenRead(path, &fd);
        int valid = RecordHandler_checkHash(hashFd);
        AEFile_Close(fd);
        if (valid != 0) {
            AEFile_OpenRead(path, &fd);
            char *rec = (char *)RH_op_new(0x1c8);
            GameRecord_ctor(rec);

            // Visited-systems bitmap at rec+4 (count then booleans).
            char *visited = rec + 4;
            *(int *)visited = 0;
            AEFile_ReadInt(visited, fd);
            for (unsigned i = 0; i < *(unsigned *)visited; i++) {
                AEFile_ReadBool((bool *)(*(int *)rec + i), fd);
            }

            AEFile_ReadInt(rec + 8, fd);
            AEFile_ReadInt(rec + 0xc, fd);
            AEFile_ReadLong(rec + 0x10, fd);
            AEFile_ReadInt(rec + 0x18, fd);
            AEFile_ReadInt(rec + 0x1c, fd);
            AEFile_ReadInt(rec + 0x20, fd);
            AEFile_ReadInt(rec + 0x24, fd);
            AEFile_ReadInt(rec + 0x28, fd);
            AEFile_ReadInt(rec + 0x3c, fd);
            AEFile_ReadInt(rec + 0x40, fd);
            *(void **)(rec + 0x54) = RecordHandler_readMission(self, fd);
            *(void **)(rec + 0x58) = RecordHandler_readMission(self, fd);
            AEFile_ReadInt(rec + 0x30, fd);
            AEFile_ReadInt(rec + 0x34, fd);
            AEFile_ReadInt(rec + 0x38, fd);
            AEFile_ReadInt(rec + 0x2c, fd);
            AEFile_ReadInt(rec + 0x44, fd);
            AEFile_ReadInt(rec + 0x48, fd);
            AEFile_ReadInt(rec + 0x4c, fd);
            AEFile_ReadInt(rec + 0x50, fd);

            // Two boolean arrays (mission flags / equipment flags).
            void *flags0 = RH_op_new(0xc);
            Array_bool_ctor(flags0);
            *(void **)(rec + 0x68) = flags0;
            unsigned n0 = 0;
            AEFile_ReadInt(&n0, fd);
            ArraySetLength_bool(n0, flags0);
            for (int i = 0; i < (int)n0; i++) {
                AEFile_ReadBool((bool *)(*(int *)((char *)flags0 + 4) + i), fd);
            }
            void *flags1 = RH_op_new(0xc);
            Array_bool_ctor(flags1);
            *(void **)(rec + 0x6c) = flags1;
            unsigned n1 = 0;
            AEFile_ReadInt(&n1, fd);
            ArraySetLength_bool(n1, flags1);
            for (int i = 0; i < (int)n1; i++) {
                AEFile_ReadBool((bool *)(*(int *)((char *)flags1 + 4) + i), fd);
            }

            // Remaining object-graph read.
            RecordHandler_recordStoreRead_body(self, rec, fd);

            AEFile_Close(fd);
        }
    }

    AEString_dtor(path);
    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
