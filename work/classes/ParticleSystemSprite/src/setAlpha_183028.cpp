#include "class.h"

struct ParticleSystemSprite {
    void setAlpha(int param_1, unsigned int param_2, float param_3);
};

extern "C" void PaintCanvas_SpriteSystemSetRGBA(unsigned int handle, unsigned int sub,
                                                unsigned short id, float a, float b, float c, float d);

void ParticleSystemSprite::setAlpha(int param_1, unsigned int param_2, float param_3)
{
    float c0 = (float)(param_2 & 0xff);
    float c1 = (float)((param_2 >> 8) & 0xff);
    float c2 = (float)((param_2 >> 16) & 0xff);
    float c3 = (float)(param_2 >> 24);

    c0 = c0 * (1.0f / 255.0f);
    c1 = c1 * (1.0f / 255.0f);
    c2 = c2 * (1.0f / 255.0f);
    c3 = c3 * (1.0f / 255.0f);

    if (F<char>(this, 0x45) == 0) {
        c0 = c0 * param_3;
    } else {
        c1 = c1 * param_3;
        c2 = c2 * param_3;
        c3 = c3 * param_3;
    }

    PaintCanvas_SpriteSystemSetRGBA(F<unsigned int>(this, 0x8), F<unsigned int>(this, 0x54),
                                    (unsigned short)(F<unsigned int>(this, 0x58) + param_1),
                                    c3, c2, c1, c0);
}
