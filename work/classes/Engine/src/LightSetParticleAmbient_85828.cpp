#include "class.h"

extern "C" void Engine_LightSetParticleAmbient(Engine *self, float red, float green, float blue)
{
    float *ambient = (float *)((char *)self + 0x314);
    *ambient++ = red;
    *ambient++ = green;
    *ambient++ = blue;
}
