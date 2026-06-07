#include "class.h"

struct Status;
// Status singleton holder-of-holder: g -> P (one load), *P -> Status object.
__attribute__((visibility("hidden"))) extern Status **g_ModStation_statusHolder;
extern "C" long long Status_getPlayingTime(Status *s);
extern "C" void *ModStation_op_new(unsigned int sz);
extern "C" void RecordHandler_ctor(void *rh);
extern "C" void RecordHandler_recordStoreWrite(void *rh, int slot);
extern "C" void RecordHandler_recordStoreWritePreview(void *rh, int slot);
extern "C" void *RecordHandler_dtor(void *rh);
extern "C" void ModStation_op_delete(void *p);
// Tail veneer (0x1ac0f8).
extern "C" void ModStation_autosaveTail();

// ModStation::autosave()
extern "C" void ModStation_autosave(ModStation *self)
{
    Status *st = *g_ModStation_statusHolder;
    if (Status_getPlayingTime(st) - 1LL < 0)
        return;
    void *rh = ModStation_op_new(0x2c);
    RecordHandler_ctor(rh);
    RecordHandler_recordStoreWrite(rh, 0);
    RecordHandler_recordStoreWritePreview(rh, 0);
    ModStation_op_delete(RecordHandler_dtor(rh));
    UC(self, 0xb1) = 1;
    if (I(self, 0x50) != 0)
        ModStation_autosaveTail();
}
