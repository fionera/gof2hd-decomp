#include "class.h"

extern "C" void operator_delete_array(void *p);  // 0x6ebfc  (operator delete[])
extern "C" void operator_delete(void *p);        // 0x6eb48

// Per-array-slot release thunk fetched from a hidden global (called for the
// lod/enabled/visible byte arrays at 0x24/0x28/0x2c).
__attribute__((visibility("hidden"))) extern void (**g_freeFn)(void *);

MeshMerger::~MeshMerger()
{
    if (pp(this, 0x8) != 0) operator_delete_array(pp(this, 0x8));
    pp(this, 0x8) = 0;

    int i = 0;         // element index (iVar4)
    int idx = 0;       // byte offset (iVar5)
    void *slots;
    for (;;) {
        slots = pp(this, 0x18);
        if (i >= i32(this, 0x0) * i32(this, 0x30)) break;

        void **cell = (void **)((char *)slots + idx);  // slots[i]
        void **m4 = (void **)((char *)*cell + 4);
        if (*m4 != 0) {
            operator_delete_array(*m4);
            cell = (void **)((char *)pp(this, 0x18) + idx);
            m4 = (void **)((char *)*cell + 4);
        }
        *m4 = 0;
        void **m10 = (void **)((char *)*cell + 0x10);
        if (*m10 != 0) {
            operator_delete_array(*m10);
            cell = (void **)((char *)pp(this, 0x18) + idx);
            m10 = (void **)((char *)*cell + 0x10);
        }
        *m10 = 0;
        if (*cell != 0) {
            operator_delete(*cell);
            cell = (void **)((char *)pp(this, 0x18) + idx);
        }
        *cell = 0;

        idx += 4;
        i += 1;
    }
    if (slots != 0) operator_delete_array(slots);
    pp(this, 0x18) = 0;

    void (*freeFn)(void *) = *g_freeFn;
    freeFn(pp(this, 0x24));
    pp(this, 0x24) = 0;
    freeFn(pp(this, 0x28));
    pp(this, 0x28) = 0;
    freeFn(pp(this, 0x2c));
    pp(this, 0x2c) = 0;

    if (pp(this, 0x1c) != 0) operator_delete_array(pp(this, 0x1c));
    pp(this, 0x1c) = 0;
}
