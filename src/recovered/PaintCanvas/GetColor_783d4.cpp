#include "class.h"

unsigned int GetColor(AbyssEngine::PaintCanvas *self)
{
    float a = *(float *)((char *)self + 0x1fc);
    float r = *(float *)((char *)self + 0x200);
    float g = *(float *)((char *)self + 0x204);
    float b = *(float *)((char *)self + 0x208);
    return ((unsigned int)(int)(r * 255.0f) << 16) +
           ((unsigned int)(int)(a * 255.0f) << 24) +
           ((unsigned int)(int)(g * 255.0f) << 8) +
           (unsigned int)(int)(b * 255.0f);
}
