#include "class.h"

void MeshClear2DMask(AbyssEngine::PaintCanvas *self)
{
    *(int *)((char *)self + 0x20) = 0;
}
