#include "class.h"

// setMatrix(index, m): tail-call the engine matrix-assign with the per-index
// slot (matrices[index], each 0x3c bytes) and the source matrix.
extern "C" void MeshMerger_setMatrix_tail(void *dst, const Matrix &m);

void MeshMerger::setMatrix(int index, const Matrix &m)
{
    char *base = (char *)pp(this, 0x1c);
    return MeshMerger_setMatrix_tail(base + index * 0x3c, m);
}
