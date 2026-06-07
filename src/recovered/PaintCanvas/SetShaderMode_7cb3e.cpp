#include "class.h"

void SetShaderMode(AbyssEngine::PaintCanvas *self, int mode)
{
    *(int *)(*(char **)((char *)self + 0x34) + 0x4a8) = mode;
}
