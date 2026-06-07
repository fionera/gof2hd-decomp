#include "class.h"

// AutoPilotList::~AutoPilotList() - release the owned String* array, free it, null it.
// Mangled name so the demangled symbol contains "~AutoPilotList".
extern "C" void ArrayReleaseClasses_String(void *arr);   // blx 0x6facc
extern "C" void *Array_String_dtor(void *arr);            // blx 0x6f64c
extern "C" void operator_delete(void *p);                 // blx 0x6eb48

extern "C" AutoPilotList *_ZN13AutoPilotListD1Ev(AutoPilotList *self) {
    ArrayReleaseClasses_String(F<void *>(self, 0x10));
    void *a = F<void *>(self, 0x10);
    if (a != 0)
        operator_delete(Array_String_dtor(a));
    F<void *>(self, 0x10) = 0;
    return self;
}
