#include "class.h"

extern "C" void operator_delete_array(void *p);  // 0x6ebfc  (operator delete[])
extern "C" void operator_delete(void *p);        // 0x6eb48

// Per-array-slot release thunk read from a hidden PC-relative global (used to
// free the lod/enabled/visible byte arrays at 0x2c/0x30/0x34).
__attribute__((visibility("hidden"))) extern void (*const g_freeFn)(void *);

// Tail-call into the engine base destructor.
extern "C" void LodMeshMerger_base_dtor(void *self);  // 0x1ac888

LodMeshMerger::~LodMeshMerger()
{
    int i = 0;     // element index (r5)
    int idx = 0;   // byte offset (r6)
    void *slots;
    for (;;) {
        slots = pp(this, 0x24);
        if (i >= i32(this, 0x0) * i32(this, 0x38)) break;

        void **cell = (void **)((char *)slots + idx);  // slots[idx]
        void **m4 = (void **)((char *)*cell + 4);
        if (*m4 != 0) {
            operator_delete_array(*m4);
            cell = (void **)((char *)pp(this, 0x24) + idx);
            m4 = (void **)((char *)*cell + 4);
        }
        *m4 = 0;
        void **m10 = (void **)((char *)*cell + 0x10);
        if (*m10 != 0) {
            operator_delete_array(*m10);
            cell = (void **)((char *)pp(this, 0x24) + idx);
            m10 = (void **)((char *)*cell + 0x10);
        }
        *m10 = 0;
        if (*cell != 0) {
            operator_delete(*cell);
            cell = (void **)((char *)pp(this, 0x24) + idx);
        }
        *cell = 0;

        idx += 4;
        i += 1;
    }
    if (slots != 0) operator_delete_array(slots);
    pp(this, 0x24) = 0;

    g_freeFn(pp(this, 0x2c));
    pp(this, 0x2c) = 0;
    g_freeFn(pp(this, 0x30));
    pp(this, 0x30) = 0;
    g_freeFn(pp(this, 0x34));
    pp(this, 0x34) = 0;

    if (pp(this, 0x28) != 0) operator_delete_array(pp(this, 0x28));
    pp(this, 0x28) = 0;
    LodMeshMerger_base_dtor(this);
}
