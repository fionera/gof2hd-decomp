#include "class.h"

struct PaintCanvas {
    static void MeshCreate(PaintCanvas *self, unsigned short mesh, unsigned int *dst, bool flag);
    static void TextureCreate(PaintCanvas *self, unsigned short id, unsigned int *dst, bool flag);
};

struct SolarSystem {
    int getTextureIndex();
};

struct Status {
    SolarSystem *getSystem();
};

__attribute__((visibility("hidden"))) extern PaintCanvas **g_paintCanvas_snr;
__attribute__((visibility("hidden"))) extern Status **g_status_snr;

void Level::switchSkyboxForSupernovaReversal() {
    PaintCanvas **pc = g_paintCanvas_snr;
    Status **st = g_status_snr;
    PaintCanvas *canvas = *pc;
    int tex = (*st)->getSystem()->getTextureIndex();
    PaintCanvas::MeshCreate(canvas, (unsigned short)(tex + 0x4588), (unsigned int *)((char *)this + 4), false);
    PaintCanvas *canvas2 = *pc;
    int tex2 = (*st)->getSystem()->getTextureIndex();
    PaintCanvas::TextureCreate(canvas2, (unsigned short)(tex2 + 0x2751), (unsigned int *)((char *)this + 0x198), false);
    skyboxTexture = -1;
}
