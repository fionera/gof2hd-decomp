#include "class.h"

// Global holder: g -> P, *P -> the record count.
__attribute__((visibility("hidden"))) extern int *g_RH_recordCount;

extern "C" void *RH_op_new(unsigned int sz);
extern "C" void Array_GR_ctor(void *a);
extern "C" void ArraySetLength_GR(unsigned int n, void *a);
extern "C" void *RecordHandler_recordStoreRead(RecordHandler *self, int idx);

// RecordHandler::readAllRecords() -> Array<GameRecord*>*
extern "C" void *RecordHandler_readAllRecords(RecordHandler *self)
{
    void *arr = RH_op_new(0xc);
    Array_GR_ctor(arr);
    int *cnt = g_RH_recordCount;
    ArraySetLength_GR(*cnt, arr);
    for (int i = 0; i < *cnt; i++) {
        void *r = RecordHandler_recordStoreRead(self, i);
        ((void **)I(arr, 4))[i] = r;
    }
    return arr;
}
