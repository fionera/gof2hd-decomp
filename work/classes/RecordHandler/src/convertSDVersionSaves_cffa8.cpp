#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void *RH_op_new(unsigned int sz);
extern "C" void Array_SC_ctor(void *a);
extern "C" void ArraySetLength_SC(unsigned int n, void *a);
extern "C" signed char *RH_op_new_arr(unsigned int n);
extern "C" void RH_op_delete_arr(void *p);
extern "C" void ArrayReleaseArrays_SC(void *a);
extern "C" int RecordHandler_readRecordAsByteArray(RecordHandler *self, signed char **out, int slot, bool fromBackup);
extern "C" void RecordHandler_writeByteArrayAsRecord(RecordHandler *self, signed char *buf, int n, int slot, bool toBackup);
extern "C" void RecordHandler_addHash(RecordHandler *self, int slot);
extern "C" void AEString_int_ctor(void *dst, int v);
extern "C" void AEString_concat(void *dst, void *a);
extern "C" void AEString_dtor(void *s);
extern "C" void AEFile_FileDelete(void *path);

__attribute__((visibility("hidden"))) extern int *g_CSV_guard;   // DAT_000e01b8 -> guard holder
__attribute__((visibility("hidden"))) extern int *g_CSV_count;   // DAT_000e01bc -> P, *P = record count

// RecordHandler::convertSDVersionSaves()
extern "C" void RecordHandler_convertSDVersionSaves(RecordHandler *self)
{
    int *guardP = g_CSV_guard;
    volatile int saved = *guardP;

    void *a0 = RH_op_new(0xc);
    Array_SC_ctor(a0);
    int *cnt = g_CSV_count;
    ArraySetLength_SC(*cnt, a0);
    void *a1 = RH_op_new(0xc);
    Array_SC_ctor(a1);
    ArraySetLength_SC(*cnt, a1);

    unsigned n = *cnt;
    unsigned long long bytes = (unsigned long long)n * 4;
    unsigned int sz = (int)(bytes >> 32) != 0 ? 0xffffffff : (unsigned int)bytes;
    signed char *sizes0 = RH_op_new_arr(sz);
    signed char *sizes1 = RH_op_new_arr(sz);

    for (int i = 0; i < (int)n; i++) {
        ((int *)sizes0)[i] = RecordHandler_readRecordAsByteArray(
            self, (signed char **)(I(a0, 4) + i * 4), i, false);
        ((int *)sizes1)[i] = RecordHandler_readRecordAsByteArray(
            self, (signed char **)(I(a1, 4) + i * 4), i, true);

        char num[12], path[12];
        AEString_int_ctor(num, i);
        AEString_concat(path, (char *)self + 0x14);
        AEFile_FileDelete(path);
        AEString_dtor(path);
        AEString_dtor(num);

        AEString_int_ctor(num, i);
        AEString_concat(path, (char *)self + 0x20);
        AEFile_FileDelete(path);
        AEString_dtor(path);
        AEString_dtor(num);
        n = *cnt;
    }

    int i = 0;
    while (true) {
        int last = (int)n - 1;
        int j = i;
        signed char *rec = 0;
        bool found = false;
        while (j < last) {
            rec = *(signed char **)(I(a0, 4) + j * 4);
            if (rec != 0) { found = true; break; }
            j++;
        }
        if (!found) {
            // process the final slot (index last) then finish
            signed char *lastRec = *(signed char **)(I(a0, 4) + last * 4);
            if (lastRec != 0) {
                RecordHandler_writeByteArrayAsRecord(self, lastRec, ((int *)sizes0)[last], 0,
                                                     false);
                RecordHandler_addHash(self, 0);
                int idx = (int)(*cnt) * 4 - 4;
                RecordHandler_writeByteArrayAsRecord(
                    self, *(signed char **)(I(a1, 4) + idx), *(int *)(sizes1 + idx), 0, true);
            }
            break;
        }
        int next = j + 1;
        RecordHandler_writeByteArrayAsRecord(self, rec, ((int *)sizes0)[j], next, false);
        RecordHandler_addHash(self, next);
        RecordHandler_writeByteArrayAsRecord(self, *(signed char **)(I(a1, 4) + j * 4),
                                             ((int *)sizes1)[j], next, true);
        i = next;
        n = *cnt;
    }

    ArrayReleaseArrays_SC(a0);
    ArrayReleaseArrays_SC(a1);
    RH_op_delete_arr(sizes0);
    RH_op_delete_arr(sizes1);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
