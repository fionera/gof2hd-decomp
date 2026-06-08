#include "class.h"

extern "C" void *operator_new_array(uint32_t n);                   // 0x6ec08 family
extern "C" void *aeabi_memclr4(void *p, uint32_t n);              // 0x6ec14
extern "C" void *aeabi_memclr(void *p, uint32_t n);              // 0x6ec20
// Raw allocator thunk fetched from a hidden global (DAT_00183bf4).
__attribute__((visibility("hidden"))) extern void *(**g_allocFn)(int);

// MeshMerger::MeshMerger(int rows, int cols, PaintCanvas*, unsigned short flags)
//   Allocates the per-cell mesh/transformed-mesh tables, the per-row LOD/matrix
//   arrays (matrices seeded to identity), and the enabled/visible byte arrays.
MeshMerger::MeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags)
{
    void *(*alloc)(int) = *g_allocFn;

    this->f_18 = 0.0f;
    this->f_1c = 0.0f;
    this->f_20 = 0.0f;
    this->f_24 = 0.0f;
    this->f_c = canvas;
    this->f_0 = rows;
    this->f_30 = cols;

    int bytes = cols * rows * 4;
    long long need = (long long)(unsigned)(cols * rows) * 4;
    int req = (int)((need >> 32) != 0 ? 0xffffffff : (unsigned)need);

    this->f_8 = alloc(req);
    aeabi_memclr4(this->f_8, bytes);
    this->f_18 = alloc(req);
    aeabi_memclr4(this->f_18, bytes);

    this->f_24 = alloc(rows | (rows >> 31));
    aeabi_memclr(this->f_24, rows);

    long long mneed = (long long)(unsigned)rows * 0x3c;
    int mreq = (int)((mneed >> 32) != 0 ? 0xffffffff : (unsigned)mneed);
    char *matrices = (char *)alloc(mreq);
    for (int off = 0; rows != 0 && off != rows * 0x3c; off += 0x3c)
        new ((void *)(matrices + off)) Matrix();
    this->f_1c = matrices;

    Matrix ident;   // engine identity matrix
    int n = this->f_0;
    char *mp = (char *)this->f_1c;
    for (int i = 0, off = 0; i < n; i++, off += 0x3c)
        *(Matrix *)(mp + off) = ident;

    n = this->f_0;
    uint8_t *en = (uint8_t *)operator_new_array(n | (n >> 31));
    this->f_28 = en;
    for (int i = 0; i < n; i++) en[i] = 1;

    uint8_t *vis = (uint8_t *)operator_new_array(n | (n >> 31));
    this->f_2c = vis;
    for (int i = 0; i < n; i++) vis[i] = 1;

    this->f_34 = 0;
    this->f_4 = flags;
    this->f_6 = 0;
}
