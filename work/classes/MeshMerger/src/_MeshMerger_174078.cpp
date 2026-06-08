#include "class.h"

extern "C" void operator_delete_array(void *p);  // 0x6ebfc  (operator delete[])
extern "C" void operator_delete(void *p);        // 0x6eb48

// Per-array-slot release thunk fetched from a hidden global (called for the
// lod/enabled/visible byte arrays at 0x24/0x28/0x2c).
__attribute__((visibility("hidden"))) extern void (**g_freeFn)(void *);

MeshMerger::~MeshMerger()
{
    if (this->f_8 != 0) operator_delete_array(this->f_8);
    this->f_8 = 0;

    int i = 0;         // element index (iVar4)
    int idx = 0;       // byte offset (iVar5)
    void *slots;
    for (;;) {
        slots = this->f_18;
        if (i >= this->f_0 * this->f_30) break;

        void **cell = (void **)((char *)slots + idx);  // slots[i]
        void **m4 = (void **)((char *)*cell + 4);
        if (*m4 != 0) {
            operator_delete_array(*m4);
            cell = (void **)((char *)this->f_18 + idx);
            m4 = (void **)((char *)*cell + 4);
        }
        *m4 = 0;
        void **m10 = (void **)((char *)*cell + 0x10);
        if (*m10 != 0) {
            operator_delete_array(*m10);
            cell = (void **)((char *)this->f_18 + idx);
            m10 = (void **)((char *)*cell + 0x10);
        }
        *m10 = 0;
        if (*cell != 0) {
            operator_delete(*cell);
            cell = (void **)((char *)this->f_18 + idx);
        }
        *cell = 0;

        idx += 4;
        i += 1;
    }
    if (slots != 0) operator_delete_array(slots);
    this->f_18 = 0;

    void (*freeFn)(void *) = *g_freeFn;
    freeFn(this->f_24);
    this->f_24 = 0;
    freeFn(this->f_28);
    this->f_28 = 0;
    freeFn(this->f_2c);
    this->f_2c = 0;

    if (this->f_1c != 0) operator_delete_array(this->f_1c);
    this->f_1c = 0;
}
