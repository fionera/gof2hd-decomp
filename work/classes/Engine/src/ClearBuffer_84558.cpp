#include "class.h"

extern "C" void glClearColor(float red, float green, float blue, float alpha);
extern "C" void glClear(unsigned int mask);

extern "C" void Engine_ClearBuffer(Engine *, uint32_t color)
{
    const double scale = 255.0;
    double red = (double)(color >> 24) / scale;
    double green = (double)((color >> 16) & 0xff) / scale;
    double blue = (double)((color >> 8) & 0xff) / scale;
    double alpha = (double)(color & 0xff) / scale;
    glClearColor((float)red, (float)green, (float)blue, (float)alpha);
    return glClear(0x4100);
}
