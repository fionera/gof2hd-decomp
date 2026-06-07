#include "class.h"

// Global holder: g -> P, *P -> the record count.
__attribute__((visibility("hidden"))) extern int *g_RH_csd_count;
// Flag holder: g -> P, *P -> the flag object (byte set to 1).
__attribute__((visibility("hidden"))) extern char **g_RH_csd_flag;

extern "C" void *RH_op_new(unsigned int sz);
extern "C" void Array_SC_ctor(void *a);
extern "C" void ArraySetLength_SC(unsigned int n, void *a);
extern "C" signed char *RH_op_new_arr(unsigned int n);
extern "C" int RecordHandler_readRecordAsByteArray(RecordHandler *self, signed char **out, int slot, bool fromBackup);
extern "C" void RecordHandler_writeByteArrayAsRecord(RecordHandler *self, signed char *buf, int n, int slot, bool toBackup);
extern "C" void RecordHandler_addHash(RecordHandler *self, int slot);
extern "C" void ArrayReleaseArrays_SC(void *a);
extern "C" void RH_op_delete_arr(void *p);
// Tail veneer at 0x1ab108: (ptr, 1, count, arr2).
extern "C" void RecordHandler_csd_tail(void *p, int one, int count, void *arr2);

// RecordHandler::changeSaveDirectoryToBackupDirectory()
extern "C" void RecordHandler_changeSaveDirectoryToBackupDirectory(RecordHandler *self)
{
    void *a0 = RH_op_new(0xc);
    Array_SC_ctor(a0);
    int *cnt = g_RH_csd_count;
    ArraySetLength_SC(*cnt, a0);
    void *a1 = RH_op_new(0xc);
    Array_SC_ctor(a1);
    ArraySetLength_SC(*cnt, a1);

    unsigned int n = *cnt;
    unsigned long long bytes = (unsigned long long)n * 4;
    unsigned int sz = (int)(bytes >> 32) != 0 ? 0xffffffff : (unsigned int)bytes;
    signed char *sizes0 = RH_op_new_arr(sz);
    signed char *sizes1 = RH_op_new_arr(sz);

    for (int i = 0; i < (int)n; i++) {
        ((int *)sizes0)[i] = RecordHandler_readRecordAsByteArray(self, (signed char **)(I(a0, 4) + i * 4), i, false);
        ((int *)sizes1)[i] = RecordHandler_readRecordAsByteArray(self, (signed char **)(I(a1, 4) + i * 4), i, true);
        n = *cnt;
    }

    **g_RH_csd_flag = 1;
    int i = 0;
    while (i < (int)n) {
        signed char *rec = *(signed char **)(I(a0, 4) + i * 4);
        if (rec == 0) {
            i++;
        } else {
            RecordHandler_writeByteArrayAsRecord(self, rec, ((int *)sizes0)[i], i, false);
            int next = i + 1;
            RecordHandler_addHash(self, next);
            RecordHandler_writeByteArrayAsRecord(self, *(signed char **)(I(a1, 4) + i * 4), ((int *)sizes1)[i], i, true);
            n = *cnt;
            i = next;
        }
    }

    ArrayReleaseArrays_SC(a0);
    ArrayReleaseArrays_SC(a1);
    RH_op_delete_arr(sizes0);
    RecordHandler_csd_tail(sizes1, 1, n, a1);
}
