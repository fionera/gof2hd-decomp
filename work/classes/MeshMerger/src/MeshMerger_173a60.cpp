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

    f32(this, 0x18) = 0.0f;
    f32(this, 0x1c) = 0.0f;
    f32(this, 0x20) = 0.0f;
    f32(this, 0x24) = 0.0f;
    pp(this, 0xc) = canvas;
    i32(this, 0x00) = rows;
    i32(this, 0x30) = cols;

    int bytes = cols * rows * 4;
    long long need = (long long)(unsigned)(cols * rows) * 4;
    int req = (int)((need >> 32) != 0 ? 0xffffffff : (unsigned)need);

    pp(this, 0x8) = alloc(req);
    aeabi_memclr4(pp(this, 0x8), bytes);
    pp(this, 0x18) = alloc(req);
    aeabi_memclr4(pp(this, 0x18), bytes);

    pp(this, 0x24) = alloc(rows | (rows >> 31));
    aeabi_memclr(pp(this, 0x24), rows);

    long long mneed = (long long)(unsigned)rows * 0x3c;
    int mreq = (int)((mneed >> 32) != 0 ? 0xffffffff : (unsigned)mneed);
    char *matrices = (char *)alloc(mreq);
    for (int off = 0; rows != 0 && off != rows * 0x3c; off += 0x3c)
        new ((void *)(matrices + off)) Matrix();
    pp(this, 0x1c) = matrices;

    Matrix ident;   // engine identity matrix
    int n = i32(this, 0x00);
    char *mp = (char *)pp(this, 0x1c);
    for (int i = 0, off = 0; i < n; i++, off += 0x3c)
        *(Matrix *)(mp + off) = ident;

    n = i32(this, 0x00);
    uint8_t *en = (uint8_t *)operator_new_array(n | (n >> 31));
    pp(this, 0x28) = en;
    for (int i = 0; i < n; i++) en[i] = 1;

    uint8_t *vis = (uint8_t *)operator_new_array(n | (n >> 31));
    pp(this, 0x2c) = vis;
    for (int i = 0; i < n; i++) vis[i] = 1;

    u8(this, 0x34) = 0;
    u16(this, 0x4) = flags;
    u8(this, 0x6) = 0;
}
