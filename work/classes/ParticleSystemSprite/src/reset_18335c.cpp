#include "class.h"

struct ParticleSystemSprite {
    void reset();
};

extern "C" void PaintCanvas_SpriteSystemSetPosition(unsigned int handle, unsigned int sub,
                                                    unsigned short id, float x, float y, float z);
extern "C" void PaintCanvas_SpriteSystemSetSize(unsigned int handle, unsigned int sub,
                                                unsigned short id, int size);

void ParticleSystemSprite::reset()
{
    for (int i = 0; i < F<int>(this, 0x48); i++) {
        PaintCanvas_SpriteSystemSetPosition(F<unsigned int>(this, 0x8), F<unsigned int>(this, 0x54),
                                            (unsigned short)(F<unsigned int>(this, 0x58) + i),
                                            4294967296.0f, 4294967296.0f, 4294967296.0f);
        PaintCanvas_SpriteSystemSetSize(F<unsigned int>(this, 0x8), F<unsigned int>(this, 0x54),
                                        (unsigned short)(F<unsigned int>(this, 0x58) + i), 0);
        F<unsigned int *>(this, 0x68)[i] = 0xffffffff;
    }
    F<int>(this, 0x60) = 0;
    F<uint8_t>(this, 0x4) = 1;
}
